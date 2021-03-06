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
//#include "AMLTypes.h"
#include "AMLHelpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "AMLByteCode.h"
#include "AMLParser.h"


static void ExtractNameSizeTests(void)
{
    /*
     STP1
     ^GET
     ^^PCI0
     ^^PCI0.SBS -> ParentPrefixChar ParentPrefixChar DualNamePrefix 'PCI0' 'SBS_'
     \S2
     \SB.ISA.COM1
     */

    assert(ExtractMaxNameSize(NULL, 0)   == 0);
    assert(ExtractMaxNameSize((const uint8_t*)"lol", 0)   == 0);
    assert(ExtractMaxNameSize((const uint8_t*)"STP1", 4)   == 4);
    assert(ExtractMaxNameSize((const uint8_t*)"^GET", 5)   == 5);
    assert(ExtractMaxNameSize((const uint8_t*)"^^GET", 5)  == 6);
    assert(ExtractMaxNameSize((const uint8_t*)"^^PCI0", 5) == 6);
    
    assert(ExtractMaxNameSize((const uint8_t*)"\\S2", 3)   == 5);
    
    uint8_t bb[] = { '\\' , 0 , 0x5d};
    assert(ExtractMaxNameSize(bb, sizeof(bb))   == 2);
    
    {
        // dual name
        const uint8_t b[] = { '^' , '^' , 0x2E  , 'P' ,'C' , 'I', '0'    , 'S' ,'B' , 'S'};
        ssize_t s = ExtractMaxNameSize( b , sizeof(b) );
        assert(s == 11);
    }
    {
        // dual name
        const uint8_t b[] = { '^' , '^' , 0x2E  , 'P' ,'C' , 'I'    , 'S' ,'B' , 'S'};
        ssize_t s = ExtractMaxNameSize( b , sizeof(b) );
        assert(s == 11);
    }
    //^^S2.MEM.SET
    {
        // multi name
        const uint8_t b[] = { '^' , '^' , 0x2F , 3  , 'S' ,'2' , '_', '_'     , 'M' ,'E' , 'M' , '_' , 'S' ,'E' , 'T' , '_' };
        ssize_t s = ExtractMaxNameSize( b , sizeof(b) );
        assert(s == 16);
    }
    //^^^S2.MEM.SET
    {
        // multi name
        const uint8_t b[] = { '^','^' , '^' , 0x2F , 3  , 'S' ,'2' , '_', '_'     , 'M' ,'E' , 'M' , '_' , 'S' ,'E' , 'T' , '_' };
        ssize_t s = ExtractMaxNameSize( b , sizeof(b) );
        assert(s == 17);
    }
    
    // invalid names

    assert(ExtractMaxNameSize((const uint8_t*)"^/", 2)   == -1);
    assert(ExtractMaxNameSize((const uint8_t*)"\\^", 2) == 0);
    
    {
        const uint8_t b[] = { '^','^' , '^' , 0x2F, 3 };
        assert(ExtractMaxNameSize(b, sizeof(b)) == -1);
    }
    
    {
        const uint8_t buff[] = { '^','^' , '^' , 0x2F , 4  , 'S' ,'2' , '_', '_'     , 'M' ,'E' , 'M' , '_' ,  'S' ,'E' , 'T' , '_' , 'T','E','S','T' };
        assert(ExtractMaxNameSize(buff, sizeof(buff)) == 21);
        
    }
    
    
}
static void ExtractNameTests(void)
{
    {
        char n[4];
        uint8_t ret = ExtractName( (const uint8_t *)"DBB_", 4, n , NULL);
        assert(ret == 3);
        assert(strcmp("DBB", n) == 0);
    }
    {
        char n[4];
        uint8_t ret = ExtractName( (const uint8_t *)"__B_", 4, n, NULL);
        assert(ret == 3);
        assert(strcmp("__B", n) == 0);
    }
    {
        char n[4];
        uint8_t ret = ExtractName( (const uint8_t *)"___B", 4, n, NULL);
        assert(ret == 4);
        assert( strncmp( "___B", n , 4) == 0);
    }
    {
        char n[4];
        uint8_t ret = ExtractName( (const uint8_t *)"DB__", 4, n, NULL);
        assert(ret == 2);
        assert(strcmp("DB", n) == 0);
    }
    
    
    {
        char n[4];
        uint8_t ret = ExtractNameString( (const uint8_t *)"DBB_", 4, n );
        assert(ret == 4);
        assert(strlen(n) == 3);
        assert(strcmp("DBB", n) == 0);
    }
    {
        char n[4];
        uint8_t ret = ExtractNameString( (const uint8_t *)"__B_", 4, n);
        assert(ret == 4);
        assert(strlen(n) == 3);
        assert(strcmp("__B", n) == 0);
    }
    {
        char n[5];
        uint8_t ret = ExtractNameString( (const uint8_t *)"___B", 4, n);
        assert(ret == 4);
        printf("%i\n", strlen(n));
        assert(strlen(n) == 5);
        assert( strncmp( "___B", n , 4) == 0);
    }
    {
        char n[4];
        uint8_t ret = ExtractNameString( (const uint8_t *)"DB__", 4, n);
        assert(ret == 4);
        assert(strlen(n) == 2);
        assert(strcmp("DB", n) == 0);
    }
    {
        const uint8_t buff[] =     "\\PHO_APBP_\x01PBW_\x01\xbe";
        
        char c[18] = "";
        const uint8_t nameS =  ExtractNameString(buff, sizeof(buff), c);
        assert(strcmp(c, "\\PHO") == 0);
        assert(strlen(c) == 4);
        assert(nameS == 5);
        
        
    }
    {
        
        const uint8_t buff[] = "^PX13.P13C";
        
        char c[18] = "";
        const uint8_t nameS =  ExtractNameString(buff, sizeof(buff), c);
        assert(nameS == 10);
        assert(strcmp(c, "^PX13.P13C") == 0);
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
    ExtractNameSizeTests();
    //ExtractNameTests();
    
    GetInteger_Tests();
    
    //DeviceIds_Tests();
    
}
