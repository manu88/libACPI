/*
 * This file is part of the libACPI project
 * Copyright (c) 2018 Manuel Deneu.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "AMLHelpers.h"
#include "AMLByteCode.h"
#include "AMLParser.h"
#include "AMLTypes.h"

// acpins_is_name(): Evaluates a name character
// Param:    char character - character from name
// Return:    int - 1 if it's a name, 0 if it's not

int IsName(char character)
{
    if((character >= '0' && character <= 'Z') || character == '_' || character == '\\')
        return 1;
    
    else
        return 0;
}


uint8_t GetNameSize(const uint8_t *buff, uint8_t maxSize )
{
    uint8_t ret = 0;
    while (ret <maxSize && IsName(*(buff++) )  )
    {
        ret++;
    }
    
    return ret;
}

uint8_t ExtractName(const uint8_t *buff, size_t size ,char* outChar)
{
    const uint8_t computedSize = GetNameSize(buff,size);
    assert( computedSize <= size);
    strncpy(outChar, (const char*)buff, computedSize);
    
    uint8_t strSize = 4;
    if (outChar[3] == '_')
    {
        
        outChar[3] = 0;
        strSize--;
        
        if (outChar[2] == '_')
        {
            outChar[2] = 0;
            strSize--;
            
            if (outChar[1] == '_')
            {
                outChar[1] = 0;
                strSize--;
            }
        }
        
    }
    return strSize;
}

int GetDWord(const uint8_t* buffer , ACPIDWord* word)
{
    assert(word);
    assert(buffer);
    
    
    union
    {
        uint8_t b[4];
        int32_t v;
    } val;
    
    
    val.b[0] = buffer[0];
    val.b[1] = buffer[1];
    val.b[2] = buffer[2];
    val.b[3] = buffer[3];
    
    *word = val.v;
    return 1;
}

// taken from https://github.com/tadryanom/lux/blob/master/kernel/acpi/eval.c
// acpins_eval_integer(): Evaluates an integer object
// Param:    uint8_t *object - pointer to object
// Param:    uint64_t *integer - destination
// Return:    size_t - size of object in bytes, 0 if it's not an integer

size_t GetInteger( const uint8_t *object,size_t objectSize, uint64_t *integer)
{
    uint8_t *byte = (uint8_t*)(object + 1);
    uint16_t *word = (uint16_t*)(object + 1);
    uint32_t *dword = (uint32_t*)(object + 1);
    uint64_t *qword = (uint64_t*)(object + 1);
    
    switch(object[0])
    {
        case AML_OP_ZeroOp:
            assert(objectSize >=1);
            *integer = 0;
            return 1;
        case AML_OP_OneOp:
            assert(objectSize >=1);
            *integer = 1;
            return 1;
        case AML_OP_OnesOp:
            assert(objectSize >=1);
            *integer = 0xFFFFFFFFFFFFFFFF;
            return 1;
        case AML_OP_BytePrefix:
            assert(objectSize >=2);
            *integer = (uint64_t)byte[0];
            return 2;
        case AML_OP_WordPrefix:
            assert(objectSize >=3);
            *integer = (uint64_t)word[0];
            return 3;
        case AML_OP_DWordPrefix:
            assert(objectSize >=5);
            *integer = (uint64_t)dword[0];
            return 5;
        case AML_OP_QWordPrefix:
            assert(objectSize >=9);
            *integer = qword[0];
            return 9;
        default:
            return 0;
    }
}


size_t ResolvePath(char *fullpath, const uint8_t *path)
{
    size_t name_size = 0;
    size_t multi_count = 0;
    size_t current_count = 0;
    
    memset(fullpath, 0, SCOPE_STR_SIZE);
    
    if(path[0] == AML_OP_RootChar)
    {
        name_size = 1;
        fullpath[0] = AML_OP_RootChar;
        fullpath[1] = 0;
        path++;
        if(IsName(path[0]) || path[0] == AML_OP_DualNamePrefix || path[0] == AML_OP_MultiNamePrefix)
        {
            fullpath[1] = '.';
            fullpath[2] = 0;
            goto start;
        } else
            return name_size;
    }
    
    //strcpy(fullpath, acpins_path);
    fullpath[strlen(fullpath)] = '.';
    
start:
    while(path[0] == AML_OP_ParentPrefixChar)
    {
        path++;
        if(strlen(fullpath) <= 2)
            break;
        
        name_size++;
        fullpath[strlen(fullpath) - 5] = 0;
        memset(fullpath + strlen(fullpath), 0, 32);
    }
    
    if(path[0] == AML_OP_DualNamePrefix)
    {
        name_size += 9;
        path++;
        
        //memcpy(fullpath + strlen(fullpath), path, 4);
        ExtractName(path, 4, fullpath + strlen(fullpath));
        
        fullpath[strlen(fullpath)] = '.';
        //memcpy(fullpath + strlen(fullpath), path + 4, 4);
        ExtractName(path+4, 4, fullpath + strlen(fullpath));
        
        
    } else if(path[0] == AML_OP_MultiNamePrefix)
    {
        // skip MULTI_PREFIX and name count
        name_size += 2;
        path++;
        
        // get name count here
        multi_count = (size_t)path[0];
        path++;
        
        current_count = 0;
        while(current_count < multi_count)
        {
            name_size += 4;
            ExtractName(path, 4, fullpath + strlen(fullpath));
            //memcpy(fullpath + strlen(fullpath), path, 4);
            path += 4;
            current_count++;
            if(current_count >= multi_count)
                break;
            
            fullpath[strlen(fullpath)] = '.';
        }
    } else
    {
        name_size += 4;
        
        ExtractName(path, 4, fullpath + strlen(fullpath));
        //memcpy(fullpath + strlen(fullpath), path, 4);
        //const char* p = path;
        //printf("Path '%s' \n" , p);
        
    }
    
    return name_size;
}
