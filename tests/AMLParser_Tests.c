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
#include <string.h>
#include <assert.h>
#include "ACPIDesc.h"
#include "AMLParser_Tests.h"
#include "AMLRouter.h"
#include "AMLHelpers.h"



static void AML_PackageLengthTests()
{
    
    {
        const uint8_t b = 0;
        size_t advanced = 0;
        size_t ret =  GetPackageLength(&b, 1, &advanced);
        assert( ret == 0);
        assert( advanced == 1);
    }
    {
        const uint8_t b = 63;
        size_t advanced = 0;
        size_t ret =  GetPackageLength(&b, 1, &advanced);
        assert( ret == 63);
        assert( advanced == 1);
    }

    {
        const uint8_t b[] = {0x20 ,0x00};
        size_t advanced = 0;
        size_t ret =  GetPackageLength(b, sizeof(b), &advanced);
        assert(advanced == 1);
        assert(ret == 0x20);
    }
    {
        const uint8_t b[] = {0x41, 0x07};
        size_t advanced = 0;
        size_t ret =  GetPackageLength(b, sizeof(b), &advanced);
        assert(advanced == 2);
        assert(ret == 0x71);
    }
    

    
    {
        const uint8_t b[] = {0x4F ,0x0F};
        size_t advanced = 0;
        size_t ret =  GetPackageLength(b, sizeof(b), &advanced);
        assert(advanced == 2);
        assert(ret == 0xFF);
    }
    {
        const uint8_t b[] = {0x42 ,0x07};
        size_t advanced = 0;
        size_t ret =  GetPackageLength(b, sizeof(b), &advanced);
        assert(advanced == 2);
        assert(ret == 0x72);
    }
    {
        const uint8_t b[] = {0x46 ,0x07};
        size_t advanced = 0;
        size_t ret =  GetPackageLength(b, sizeof(b), &advanced);
        assert(advanced == 2);
        assert(ret == 0x76);
    }
    {
        const uint8_t b[] = {0x48 ,0x07};
        size_t advanced = 0;
        size_t ret =  GetPackageLength(b, sizeof(b), &advanced);
        assert(advanced == 2);
        assert(ret == 0x78);
    }
    {
        const uint8_t b[] = {0x48 ,0x09};
        size_t advanced = 0;
        size_t ret =  GetPackageLength(b, sizeof(b), &advanced);
        assert(advanced == 2);
        assert(ret == 0x98);
    }
    {
        const uint8_t b[] = {0x80, 0xf4, 0x07};
        size_t advanced = 0;
        size_t ret =  GetPackageLength(b, sizeof(b), &advanced);
        assert(advanced == 3);
        assert(ret == 0x7f40); // 0xFEC
    }
    
    {
        const uint8_t b[] = {0x4A ,0x05};
        size_t advanced = 0;
        size_t ret =  GetPackageLength(b, sizeof(b), &advanced);
        assert(advanced == 2);
        assert(ret == 0x5A);
    }
    {
        const uint8_t b[] = {0x40, 0x0F};
        size_t advanced = 0;
        size_t ret =  GetPackageLength(b, sizeof(b), &advanced);
        assert(advanced == 2);
        assert(ret == 0xF0);
    }
    
    
}
static void AML_InternalTests()
{
    {
        uint8_t b[4] = {0};
        
        ACPIDWord w = 12;
        
        GetDWord(b, &w);
        assert(w == 0);
    }
    {
        uint8_t b[4] = {0xab , 0xcd , 0xef , 0x12};
        
        ACPIDWord w = 12;
        
        GetDWord(b, &w);
        assert(w == 0x12efcdab);
    }
    {
        uint8_t b[4] = {0xab , 0x00 , 0xef , 0x00};
        
        ACPIDWord w = 12;
        
        GetDWord(b, &w);
        assert(w == 0x00ef00ab);
    }
    {
        const uint8_t b[] = { 'H' , 'I' , 'D' , 'A'};
        char out[5] = {0};

        assert (ExtractNameString(b, 4, out) == 4);

        assert( strcmp(out , "HIDA") == 0);
    }
    {
        const uint8_t b[] = { '_' , 'I' , 'D' , 'A'};
        char out[5] = {0};

        const uint8_t sSize =  ExtractNameString(b, 4, out );
        assert(sSize == 4);

        assert( strcmp(out , "_IDA") == 0);
    }
    {
        const uint8_t b[] = { 'H' , 'I' , 'D' , '_'};
        char out[5] = {0};

        const uint8_t sSize =  ExtractNameString(b, 4, out);

        assert(sSize == 4);
        assert( strcmp(out , "HID") == 0);
    }
    {
        const uint8_t b[] = { 'H' , 'I' , '_' , '_'};
        char out[5] = {0};

        const uint8_t sSize =  ExtractNameString(b, 4, out);

        assert(sSize == 4);
        assert( strcmp(out , "HI") == 0);
    }
    {
        const uint8_t b[] = { 'H' , '_' , '_' , '_'};
        char out[5] = {0};

        const uint8_t sSize =  ExtractNameString(b, 4, out);
        assert(sSize == 4);
        assert( strcmp(out , "H") == 0);
    }
}

void AMLParserTests()
{
    AML_InternalTests();
    AML_PackageLengthTests();
    
    {
        const uint8_t b1[] = {'P'};
        size_t advancedByte = 0;
        AMLOperation op = _GetNextOp( b1, sizeof(b1), &advancedByte ,  0/*offset*/ );
        assert(op == AML_Char);
        assert(advancedByte == 1);
    }
    {
        size_t pos = 0;
        
        const uint8_t b1[] = {0x5B ,0x82 , 'P' , 'R'};
        
        size_t size = sizeof(b1);
        size_t advancedByte = 0;
        AMLOperation op = _GetNextOp( b1, size, &advancedByte ,  0/*offset*/ );
        assert(op == AML_DeviceOpList);
        assert(advancedByte == 2);
        
        pos += advancedByte;
        size -= advancedByte;
        
        op = _GetNextOp( b1+pos, size, &advancedByte ,  0/*offset*/ );
        assert(op == AML_Char);
        assert(advancedByte == 1);
        
        pos += advancedByte;
        size -= advancedByte;
        
        op = _GetNextOp( b1+pos, size, &advancedByte ,  0/*offset*/ );
        assert(op == AML_Char);
        assert(advancedByte == 1);
        
    }
}
