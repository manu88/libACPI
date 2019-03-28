//
//  AMLName_Tests.c
//  Tests
//
//  Created by Manuel Deneu on 16/03/2019.
//  Copyright © 2019 Manuel Deneu. All rights reserved.
//

#include <stdlib.h>
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
        const uint8_t b[] = { '\\',0  , 0x2F , 4  , 'S' ,'2' , '_', '_'     , 'M' ,'E' , 'M' , '_' ,  'S' ,'E' , 'T' , '_' , 'T','E','S','T' ,0,0,0};
        
        ssize_t num = AMLNameCreateFromBuffer(&name, b, sizeof(b));
        
        
        char* ret = AMLNameConstructNormalized(&name);
        assert( num == 2);
        assert(ret);
        assert(strcmp(ret, "\\")==0);
        free(ret);
    }
    

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
        assert(AMLNameCreateFromBuffer(&name, b, sizeof(b)) == -2);// not enough buffer size
        assert(NameIsInvalid(&name));
        /*
        assert(AMLNameCountParents(&name) == 0);
        assert(AMLNameHasPrefixRoot(&name) == 0);
        assert(AMLNameCountSegments(&name) == 1);
         */
    }
    {
        AMLName name = {0};
        const uint8_t b[] = {'A' , 'M' , 'L' , '_'};
        assert(AMLNameCreateFromBuffer(&name, b, sizeof(b)) == 4);
        assert(NameIsValid(&name));
        
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
        const uint8_t b[] = {'\\' , 'A' , 'M' , 'L' ,'_'  /*Junk*/ ,0x1 , 0x10  };
        assert(AMLNameCreateFromBuffer(&name, b, sizeof(b)) == 5);
        
        assert( NameIsValid(&name));
        assert(AMLNameCountParents(&name) == 0);
        assert(AMLNameHasPrefixRoot(&name) == 1);
        assert(AMLNameCountSegments(&name) == 1);
        
        
        char segName[5] = {0};
        assert(AMLNameGetSegment(&name, 0, segName));
        assert(strcmp(segName, "AML_" ) == 0);
    }
    {
        AMLName name = {0};
        const uint8_t b[] = {'^' ,'_', 'A' , 'M' , 'L'};
        assert(AMLNameCreateFromBuffer(&name, b, sizeof(b) ) == 5);
        
        assert( NameIsValid(&name));
        assert(AMLNameCountParents(&name) == 1);
        assert(AMLNameHasPrefixRoot(&name) == 0);
        assert(AMLNameCountSegments(&name) == 1);
        
        
        char segName[5] = {0};
        assert(AMLNameGetSegment(&name, 0, segName));
        assert(strcmp(segName, "_AML" ) == 0);
    }
    {
        AMLName name = {0};
        const uint8_t b[] = {'^' , '^' , 'A' , 'M' , 'L' , '_'};
        assert(AMLNameCreateFromBuffer(&name, b, sizeof(b)) == 6);
        
        assert( NameIsValid(&name));
        assert(AMLNameCountParents(&name) == 2);
        assert(AMLNameHasPrefixRoot(&name) == 0);
        assert(AMLNameCountSegments(&name) == 1);
        
        char segName[5] = {0};
        assert(AMLNameGetSegment(&name, 0, segName));
        assert(strcmp(segName, "AML_" ) == 0);
    }
    {
        AMLName name = {0};
        const uint8_t b[] = {'^' , '^' ,'^' , 'A' , 'M' , 'L' , '_'};
        assert(AMLNameCreateFromBuffer(&name, b, sizeof(b)) == 7);
        
        assert( NameIsValid(&name));
        assert(AMLNameCountParents(&name) == 3);
        assert(AMLNameHasPrefixRoot(&name) == 0);
        assert(AMLNameCountSegments(&name) == 1);
        
        char segName[5] = {0};
        assert(AMLNameGetSegment(&name, 0, segName));
        assert(strcmp(segName, "AML_" ) == 0);
    }
    {
        AMLName name = {0};
        const uint8_t b[] = { '^' , '^' , 0x2E  , 'P' ,'C' , 'I' , '_'    , 'S' ,'B' , 'S', '_'};
        assert( AMLNameCreateFromBuffer(&name, b, sizeof(b)) == 11);
        
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
        assert(AMLNameCreateFromBuffer(&name, b, sizeof(b)) == 17);
        
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
        assert(AMLNameCreateFromBuffer(&name, b, sizeof(b)) == -1);
        
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
        assert(AMLNameCreateFromBuffer(&name, b, sizeof(b)) == -1);
        
        assert( NameIsInvalid(&name));
        /*
         assert(AMLNameCountParents(&name) == 3);
         assert(AMLNameHasPrefixRoot(&name) == 0);
         assert(AMLNameCountSegments(&name) == 3);
         */
    }
   
    
}


void doAMLNameNormalizedTests()
{
    
    
    {
        AMLName name = {0};
        const uint8_t b[] = {'A' , 'M' , 'L','_' /*Junk*/ ,0x1 , 0x10  };
        assert(AMLNameCreateFromBuffer(&name, b, sizeof(b))) ;
        
        char* ret = AMLNameConstructNormalized(&name);
        assert(ret);
        assert(strcmp(ret, "AML_")==0);
        free(ret);
    }
    {
        AMLName name = {0};
        const uint8_t b[] = {'\\' ,'A' , 'M' , 'L' ,'_' /*Junk*/ ,0x6 , 0x10  };
        assert(AMLNameCreateFromBuffer(&name, b, sizeof(b))) ;
        
        char* ret = AMLNameConstructNormalized(&name);
        assert(ret);
        assert(strcmp(ret, "\\AML_")==0);
        free(ret);
    }
    {
        AMLName name = {0};
        const uint8_t b[] = {'^' ,'A' , 'M' , 'L' ,'A' /*Junk*/ ,0x6 , 0x10  };
        assert(AMLNameCreateFromBuffer(&name, b, sizeof(b))) ;
        
        char* ret = AMLNameConstructNormalized(&name);
        assert(ret);
        assert(strcmp(ret, "^AMLA")==0);
        free(ret);
    }
    {
        AMLName name = {0};
        const uint8_t b[] = {'^','^','^','^' ,'A' , 'M' , 'L' ,'A' /*Junk*/ ,0x6 , 0x10  };
        assert(AMLNameCreateFromBuffer(&name, b, sizeof(b))) ;
        
        char* ret = AMLNameConstructNormalized(&name);
        assert(ret);
        assert(strcmp(ret, "^^^^AMLA")==0);
        free(ret);
    }
    {
        AMLName name = {0};
        const uint8_t b[] = { '^' , '^' , 0x2E  , 'P' ,'C' , 'I' , '_'    , 'S' ,'B' , 'S', '_'};
        assert( AMLNameCreateFromBuffer(&name, b, sizeof(b)) );
        
        char* ret = AMLNameConstructNormalized(&name);
        assert(ret);
        assert(strcmp(ret, "^^PCI_.SBS_")==0);
        free(ret);
    }
    {
        AMLName name = {0};
        const uint8_t b[] = { '^','^','^','^' , '^' , 0x2E  , 'P' ,'C' , 'I' , '_'    , 'S' ,'B' , 'S', '_'};
        assert( AMLNameCreateFromBuffer(&name, b, sizeof(b)) );
        
        char* ret = AMLNameConstructNormalized(&name);
        assert(ret);
        assert(strcmp(ret, "^^^^^PCI_.SBS_")==0);
        free(ret);
    }
    {
        AMLName name = {0};
        const uint8_t b[] = { '^','^' , '^' , 0x2F , 3  , 'S' ,'2' , '_', '_'     , 'M' ,'E' , 'M' , '_' , 'S' ,'E' , 'T' , '_' };
        assert( AMLNameCreateFromBuffer(&name, b, sizeof(b)) );
        
        char* ret = AMLNameConstructNormalized(&name);
        assert(ret);
        assert(strcmp(ret, "^^^S2__.MEM_.SET_")==0);
        free(ret);
    }
    {
        AMLName name = {0};
        const uint8_t b[] = { '^','^' , '^' , 0x2F , 4  , 'S' ,'2' , '_', '_'     , 'M' ,'E' , 'M' , '_' ,  'S' ,'E' , 'T' , '_' , 'T','E','S','T' ,0,0,0};
        
        assert( AMLNameCreateFromBuffer(&name, b, sizeof(b)) == 21);
        
        char* ret = AMLNameConstructNormalized(&name);
        assert(ret);
        assert(strcmp(ret, "^^^S2__.MEM_.SET_.TEST")==0);
        free(ret);
    }
    
}
