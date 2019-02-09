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

#include "Helper_Tests.h"
#include "AMLHelpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "AMLTypes.h"
#include "AMLByteCode.h"
#include "AMLParser.h"

static void ExtractNameTests(void)
{
    {
        char n[4];
        uint8_t ret = ExtractName( (const uint8_t *)"DBB_", 4, n);
        assert(ret == 3);
        assert(strcmp("DBB", n) == 0);
    }
    {
        char n[4];
        uint8_t ret = ExtractName( (const uint8_t *)"__B_", 4, n);
        assert(ret == 3);
        assert(strcmp("__B", n) == 0);
    }
    {
        char n[4];
        uint8_t ret = ExtractName( (const uint8_t *)"___B", 4, n);
        assert(ret == 4);
        assert( strncmp( "___B", n , 4) == 0);
    }
    {
        char n[4];
        uint8_t ret = ExtractName( (const uint8_t *)"DB__", 4, n);
        assert(ret == 2);
        assert(strcmp("DB", n) == 0);
    }
    
}

static void ResolvePath_Tests()
{
    {
        char name[SCOPE_STR_SIZE] = {0};
        size_t ret = ResolvePath(name, (const uint8_t *)"");
        
        assert( strcmp(name, ".") == 0);
        assert( ret == 4);
        for(int i=0;i<strlen(name);i++)
        {
            assert(name[i] != '\0');
        }
    }
    {
        char name[512] = {0};
        
        uint8_t b[] = {'_' , 'D' , 'B', '_'};
        size_t ret = ResolvePath(name, b);
        
        assert( ret == 4);
        assert( strcmp(name, "._DB") == 0);
        for(int i=0;i<strlen(name);i++)
        {
            assert(name[i] != '\0');
        }
    }
    {
        char name[512] = {0};
        
        uint8_t b[] = {'_' , 'D' , '_', '_'};
        size_t ret = ResolvePath(name, b);
        
        assert( ret == 4);
        assert( strcmp(name, "._D") == 0);
        for(int i=0;i<strlen(name);i++)
        {
            assert(name[i] != '\0');
        }
    }
    {
        char name[512] = {0};
        
        uint8_t b[] = {'P' , '_' , '_', '_'};
        size_t ret = ResolvePath(name, b);
        
        assert( ret == 4);
        assert( strcmp(name, ".P") == 0);
        for(int i=0;i<strlen(name);i++)
        {
            assert(name[i] != '\0');
        }
    }
    
    
    // dual name prefix
    {
        char name[512] = {0};
        
        uint8_t b[] = {AML_OP_DualNamePrefix , '_' ,'S' , 'B','_'     , 'P', 'C' , 'I' , '_'};
        size_t ret = ResolvePath(name, b);
        
        assert( ret == sizeof(b));
        assert( strcmp(name, "._SB.PCI") == 0);
        for(int i=0;i<strlen(name);i++)
        {
            assert(name[i] != '\0');
        }
    }
    {
        char name[512] = {0};
        
        uint8_t b[] = {AML_OP_DualNamePrefix , '_' ,'S' , 'B','_'     , 'P', 'C' , 'I' , '0'};
        size_t ret = ResolvePath(name, b);
        
        assert( ret == sizeof(b));
        assert( strcmp(name, "._SB.PCI0") == 0);
        for(int i=0;i<strlen(name);i++)
        {
            assert(name[i] != '\0');
        }
    }
    {
        char name[512] = {0};
        
        uint8_t b[] = {AML_OP_DualNamePrefix , '_' ,'S' , '_','_'     , 'P', 'C' , 'I' , '0'};
        size_t ret = ResolvePath(name, b);
        
        assert( ret == sizeof(b));
        assert( strcmp(name, "._S.PCI0") == 0);
        
        for(int i=0;i<strlen(name);i++)
        {
            assert(name[i] != '\0');
        }
    }
    
    
    // multi name prefix
    {
        char name[512] = {0};
        
        uint8_t b[] = {AML_OP_MultiNamePrefix , 3,
            '_' ,'S' , 'B','_',
            'P', 'C' , 'I' , '0',
            'T', 'E' , 'C' , '_',
            
        };
        size_t ret = ResolvePath(name, b);
        
        assert( ret == sizeof(b));
        assert( strcmp(name, "._SB.PCI0.TEC") == 0);
        
        for(int i=0;i<strlen(name);i++)
        {
            assert(name[i] != '\0');
        }
    }
}

static void GetInteger_Tests()
{
    {
        uint8_t buf[] = {0};
        
        uint64_t out;
        assert(GetInteger(buf,sizeof(buf), &out) == 1);
        assert(out == 0);
    }
    {
        uint8_t buf[] = {1};
        
        uint64_t out;
        assert(GetInteger(buf,sizeof(buf), &out) == 1);
        assert(out == 1);
    }
    {
        uint8_t buf[] = { AML_OP_BytePrefix, 120};
        
        uint64_t out;
        assert(GetInteger(buf,sizeof(buf), &out) == 2);
        assert(out == 120);
    }
    {
        uint8_t buf[] = { AML_OP_WordPrefix, 0xAB,0xCD};
        
        uint64_t out;
        assert(GetInteger(buf,sizeof(buf), &out) == 3);
        assert(out == 0xcdab);
    }
    {
        uint8_t buf[] = { AML_OP_DWordPrefix, 0xAB,0xCD, 0xEF,0X12};
        
        uint64_t out;
        assert(GetInteger(buf,sizeof(buf), &out) == 5);
        assert(out == 0x12efcdab);
    }
    {
        uint8_t buf[] = { AML_OP_DWordPrefix, 0xAB,0xCD, 0xEF,0X12};
        
        uint64_t out;
        assert(GetInteger(buf,sizeof(buf), &out) == 5);
        assert(out == 0x12efcdab);
    }
    {
        uint8_t buf[] = { AML_OP_QWordPrefix, 0xAB,0xCD, 0xEF,0X12,0x34,0x56, 0x78,0X90};
        
        uint64_t out;
        assert(GetInteger(buf,sizeof(buf), &out) == 9);
        assert(out == 0x9078563412efcdab);
    }
    {
        uint8_t buf[] = {AML_OP_ZeroOp};
        
        uint64_t out;
        assert(GetInteger(buf,sizeof(buf), &out) == 1);
        assert(out == 0);
    }
    {
        uint8_t buf[] = {AML_OP_OneOp};
        
        uint64_t out;
        assert(GetInteger(buf,sizeof(buf), &out) == 1);
        assert(out == 1);
    }
    {
        uint8_t buf[] = {AML_OP_OnesOp};
        
        uint64_t out;
        assert(GetInteger(buf,sizeof(buf), &out) == 1);
        assert(out == 0xFFFFFFFFFFFFFFFF);
    }
    {
        uint8_t buf[] = {AML_OP_NameOp};
        
        uint64_t out = 12;
        assert(GetInteger(buf,sizeof(buf), &out) == 0);
        assert(out == 12);
    }
    
}

/*
static void DeviceIds_Tests()
{
    {
        const uint8_t b[] = {0,0,0,0};
    
        assert(GetDeviceIDFromStr(b) == _UnknownDeviceIdentifier);
    }
    {
        const uint8_t b[] = {'_' , 'A' , 'D' , 'R'};
        
        assert(GetDeviceIDFromStr(b) == _ADR);
    }
    {
        const uint8_t b[] = {'P' , 'A' , 'D' , 'R'};
        
        assert(GetDeviceIDFromStr(b) == _UnknownDeviceIdentifier);
    }
    {
        const uint8_t b[] = {'_' , 'U' , 'I' , 'D'};
        
        assert(GetDeviceIDFromStr(b) == _UID);
    }
    {
        const uint8_t b[] = {'_' , 'C' , 'I' , 'D'};
        
        assert(GetDeviceIDFromStr(b) == _CID);
    }
    {
        const uint8_t b[] = {'_' , 'D' , 'D' , 'N'};
        
        assert(GetDeviceIDFromStr(b) == _DDN);
    }
    {
        const uint8_t b[] = {'_' , 'H' , 'I' , 'D'};
        
        assert(GetDeviceIDFromStr(b) == _HID);
    }
    {
        const uint8_t b[] = {'_' , 'M' , 'L' , 'S'};
        
        assert(GetDeviceIDFromStr(b) == _MLS);
    }
    {
        const uint8_t b[] = {'_' , 'P' , 'L' , 'D'};
        
        assert(GetDeviceIDFromStr(b) == _PLD);
    }
    {
        const uint8_t b[] = {'_' , 'S' , 'U' , 'N'};
        
        assert(GetDeviceIDFromStr(b) == _SUN);
    }
    {
        const uint8_t b[] = {'_' , 'S' , 'T' , 'R'};
        
        assert(GetDeviceIDFromStr(b) == _STR);
    }
}
*/
void Helper_Tests()
{
    ExtractNameTests();
    ResolvePath_Tests();
    GetInteger_Tests();
    
    //DeviceIds_Tests();
    
}
