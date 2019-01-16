//
//  Helper_Tests.c
//  Tests
//
//  Created by Manuel Deneu on 16/01/2019.
//  Copyright © 2019 Manuel Deneu. All rights reserved.
//

#include "Helper_Tests.h"
#include "AMLHelpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "AMLByteCode.h"

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
void Helper_Tests()
{
    ExtractNameTests();
    
    {
        char name[512] = {0};
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
