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
#include "AMLHelpers.h"
#include "AMLByteCode.h"

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

int ExtractName(const uint8_t *buff, size_t size ,char* outChar)
{
    strncpy(outChar, (const char*)buff, 4);
    outChar[4] = 0;
    
    if (outChar[3] == '_')
    {
        outChar[3] = 0;
    }
    return 1;
}

// taken from https://github.com/tadryanom/lux/blob/master/kernel/acpi/eval.c
// acpins_eval_integer(): Evaluates an integer object
// Param:    uint8_t *object - pointer to object
// Param:    uint64_t *integer - destination
// Return:    size_t - size of object in bytes, 0 if it's not an integer

size_t GetInteger( const uint8_t *object, uint64_t *integer)
{
    uint8_t *byte = (uint8_t*)(object + 1);
    uint16_t *word = (uint16_t*)(object + 1);
    uint32_t *dword = (uint32_t*)(object + 1);
    uint64_t *qword = (uint64_t*)(object + 1);
    
    switch(object[0])
    {
        case AML_OP_ZeroOp:
            *integer = 0;
            return 1;
        case AML_OP_OneOp:
            *integer = 1;
            return 1;
        case AML_OP_OnesOp:
            *integer = 0xFFFFFFFFFFFFFFFF;
            return 1;
        case AML_OP_BytePrefix:
            *integer = (uint64_t)byte[0];
            return 2;
        case AML_OP_WordPrefix:
            *integer = (uint64_t)word[0];
            return 3;
        case AML_OP_DWordPrefix:
            *integer = (uint64_t)dword[0];
            return 5;
        case AML_OP_QWordPrefix:
            *integer = qword[0];
            return 9;
        default:
            return 0;
    }
}
