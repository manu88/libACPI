//
//  AMLName_Tests.c
//  Tests
//
//  Created by Manuel Deneu on 16/03/2019.
//  Copyright © 2019 Manuel Deneu. All rights reserved.
//

#include <string.h>
#include <assert.h>
#include "AMLName_Tests.h"
#include "AMLHelpers.h"

static int NameIsInvalid( const AMLName* name)
{
    return name->originalBuf == NULL && name->originalBufSize == 0;
}

static int NameIsValid( const AMLName* name)
{
    return !NameIsInvalid(name);
}

void doAMLNameTests()
{
    
    
    {
        AMLName name = {0};
        const uint8_t b[] = {0x0 , 0x1};
        uint8_t ret = AMLNameCreateFromBuffer(&name, b, sizeof(b));
        assert(ret == 0);
        assert(NameIsInvalid(&name));
        assert(AMLNameCountParents(&name) == 0);
        assert(AMLNameHasPrefixRoot(&name) == 0);
        
    }
    {
        AMLName name = {0};
        const uint8_t b[] = {'A' , 'M' , 'L'};
        uint8_t ret = AMLNameCreateFromBuffer(&name, b, sizeof(b));
        assert(ret == 1);
        assert( NameIsValid(&name));
        assert(AMLNameCountParents(&name) == 0);
        assert(AMLNameHasPrefixRoot(&name) == 0);
        assert(AMLNameCountSegments(&name) == 1);
    }
    {
        // VALID cause only '\'
        AMLName name = {0};
        const uint8_t b[] = {'\\' };
        uint8_t ret = AMLNameCreateFromBuffer(&name, b, sizeof(b));
        assert(ret == 1);
        assert( NameIsValid(&name));
        assert(AMLNameCountParents(&name) == 0);
        assert(AMLNameHasPrefixRoot(&name) == 1);
    }
    {
        // invalid cause only '^'
        AMLName name = {0};
        const uint8_t b[] = {'^' };
        uint8_t ret = AMLNameCreateFromBuffer(&name, b, sizeof(b));
        assert(ret == 0);
        assert( NameIsInvalid(&name));
        assert(AMLNameCountParents(&name) == 0);
        assert(AMLNameHasPrefixRoot(&name) == 0);
    }
    {
        // invalid cause only '^^'
        AMLName name = {0};
        const uint8_t b[] = {'^' , '^' };
        uint8_t ret = AMLNameCreateFromBuffer(&name, b, sizeof(b));
        assert(ret == 0);
        assert( NameIsInvalid(&name));
        assert(AMLNameCountParents(&name) == 0);
        assert(AMLNameHasPrefixRoot(&name) == 0);
    }
    {
        AMLName name = {0};
        const uint8_t b[] = {'\\' , 'A' , 'M' , 'L'};
        uint8_t ret = AMLNameCreateFromBuffer(&name, b, sizeof(b));
        assert(ret == 1);
        assert( NameIsValid(&name));
        assert(AMLNameCountParents(&name) == 0);
        assert(AMLNameHasPrefixRoot(&name) == 1);
        assert(AMLNameCountSegments(&name) == 1);
        
        
        char segName[5] = {0};
        assert(AMLNameGetSegment(&name, 0, segName));
        assert(strcmp(segName, "AML" ) == 0);
    }
    {
        AMLName name = {0};
        const uint8_t b[] = {'^' , 'A' , 'M' , 'L'};
        uint8_t ret = AMLNameCreateFromBuffer(&name, b, sizeof(b));
        assert(ret == 1);
        assert( NameIsValid(&name));
        assert(AMLNameCountParents(&name) == 1);
        assert(AMLNameHasPrefixRoot(&name) == 0);
        assert(AMLNameCountSegments(&name) == 1);
        
        
        char segName[5] = {0};
        assert(AMLNameGetSegment(&name, 0, segName));
        assert(strcmp(segName, "AML" ) == 0);
    }
    {
        AMLName name = {0};
        const uint8_t b[] = {'^' , '^' , 'A' , 'M' , 'L'};
        uint8_t ret = AMLNameCreateFromBuffer(&name, b, sizeof(b));
        assert(ret == 1);
        assert( NameIsValid(&name));
        assert(AMLNameCountParents(&name) == 2);
        assert(AMLNameHasPrefixRoot(&name) == 0);
        assert(AMLNameCountSegments(&name) == 1);
        
        char segName[5] = {0};
        assert(AMLNameGetSegment(&name, 0, segName));
        assert(strcmp(segName, "AML" ) == 0);
    }
    {
        AMLName name = {0};
        const uint8_t b[] = {'^' , '^' ,'^' , 'A' , 'M' , 'L'};
        uint8_t ret = AMLNameCreateFromBuffer(&name, b, sizeof(b));
        assert(ret == 1);
        assert( NameIsValid(&name));
        assert(AMLNameCountParents(&name) == 3);
        assert(AMLNameHasPrefixRoot(&name) == 0);
        assert(AMLNameCountSegments(&name) == 1);
        
        char segName[5] = {0};
        assert(AMLNameGetSegment(&name, 0, segName));
        assert(strcmp(segName, "AML" ) == 0);
    }
    {
        AMLName name = {0};
        const uint8_t b[] = { '^' , '^' , 0x2E  , 'P' ,'C' , 'I' , '_'    , 'S' ,'B' , 'S', '_'};
        uint8_t ret = AMLNameCreateFromBuffer(&name, b, sizeof(b));
        assert(ret == 1);
        assert( NameIsValid(&name));
        assert(AMLNameCountParents(&name) == 2);
        assert(AMLNameHasPrefixRoot(&name) == 0);
        assert(AMLNameCountSegments(&name) == 2);
        
        char segName[5] = {0};
        assert(AMLNameGetSegment(&name, 0, segName));
        assert(strcmp(segName, "PCI_" ) == 0);
        
        assert(AMLNameGetSegment(&name, 1, segName));
        assert(strcmp(segName, "SBS_" ) == 0);
    }
    {
        //^^^S2.MEM.SET
        // multi name
        AMLName name = {0};
        const uint8_t b[] = { '^','^' , '^' , 0x2F , 3  , 'S' ,'2' , '_', '_'     , 'M' ,'E' , 'M' , '_' , 'S' ,'E' , 'T' , '_' };
        uint8_t ret = AMLNameCreateFromBuffer(&name, b, sizeof(b));
        assert(ret == 1);
        assert( NameIsValid(&name));
        assert(AMLNameCountParents(&name) == 3);
        assert(AMLNameHasPrefixRoot(&name) == 0);
        assert(AMLNameCountSegments(&name) == 3);
        
        
        char segName[5] = {0};
        assert(AMLNameGetSegment(&name, 0, segName));
        assert(strcmp(segName, "S2__" ) == 0);
        
        assert(AMLNameGetSegment(&name, 1, segName));
        assert(strcmp(segName, "MEM_" ) == 0);
        
        assert(AMLNameGetSegment(&name, 2, segName));
        assert(strcmp(segName, "SET_" ) == 0);
    }
    {
        //^^^S2.MEM.SET
        // multi name, but troncated
        AMLName name = {0};
        const uint8_t b[] = { '^','^' , '^' , 0x2F };//, 3  , 'S' ,'2' , '_', '_'     , 'M' ,'E' , 'M' , '_' , 'S' ,'E' , 'T' , '_' };
        uint8_t ret = AMLNameCreateFromBuffer(&name, b, sizeof(b));
        assert(ret == 0);
        assert( NameIsInvalid(&name));
        /*
        assert(AMLNameCountParents(&name) == 3);
        assert(AMLNameHasPrefixRoot(&name) == 0);
        assert(AMLNameCountSegments(&name) == 3);
         */
    }
    {
        //^^^S2.MEM.SET
        // multi name, but troncated
        AMLName name = {0};
        const uint8_t b[] = { '^','^' , '^' , 0x2F, 3 };//  , 'S' ,'2' , '_', '_'     , 'M' ,'E' , 'M' , '_' , 'S' ,'E' , 'T' , '_' };
        uint8_t ret = AMLNameCreateFromBuffer(&name, b, sizeof(b));
        assert(ret == 0);
        assert( NameIsInvalid(&name));
        /*
         assert(AMLNameCountParents(&name) == 3);
         assert(AMLNameHasPrefixRoot(&name) == 0);
         assert(AMLNameCountSegments(&name) == 3);
         */
    }
    
}
