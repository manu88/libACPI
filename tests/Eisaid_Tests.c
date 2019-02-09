//
//  Eisaid_Tests.c
//  Tests
//
//  Created by Manuel Deneu on 09/02/2019.
//  Copyright © 2019 Manuel Deneu. All rights reserved.
//

#include <string.h>
#include <assert.h>
#include "EISAID.h"

#include "Eisaid_Tests.h"

static void WorkingTests()
{
    {
        char eisaid[8] = {0};
        assert(getEisaidString(PNP0103 , eisaid));
        assert(strcmp(eisaid, "PNP0103") == 0);
        
        uint64_t v = 0;
        assert(getValueFromEisaidString(eisaid, &v));
        assert(v == PNP0103);
    }
    {
        char eisaid[8] = {0};
        assert(getEisaidString(PNP0A03 , eisaid));
        assert(strcmp(eisaid, "PNP0A03") == 0);
        
        uint64_t v = 0;
        assert(getValueFromEisaidString(eisaid, &v));
        assert(v == PNP0A03);
    }
    {
        char eisaid[8] = {0};
        assert(getEisaidString(PNP0A01 , eisaid));
        assert(strcmp(eisaid, "PNP0A01") == 0);
        
        uint64_t v = 0;
        assert(getValueFromEisaidString(eisaid, &v));
        assert(v == PNP0A01);
    }
    {
        char eisaid[8] = {0};
        assert(getEisaidString(PNP0B00 , eisaid));
        assert(strcmp(eisaid, "PNP0B00") == 0);
        
        uint64_t v = 0;
        assert(getValueFromEisaidString(eisaid, &v));
        assert(v == PNP0B00);
    }
    {
        char eisaid[8] = {0};
        assert(getEisaidString(PNP0303 , eisaid));
        assert(strcmp(eisaid, "PNP0303") == 0);
        
        uint64_t v = 0;
        assert(getValueFromEisaidString(eisaid, &v));
        assert(v == PNP0303);
    }
    {
        char eisaid[8] = {0};
        assert(getEisaidString(PNP0F13 , eisaid));
        assert(strcmp(eisaid, "PNP0F13") == 0);
        
        uint64_t v = 0;
        assert(getValueFromEisaidString(eisaid, &v));
        assert(v == PNP0F13);
    }
    {
        char eisaid[8] = {0};
        assert(getEisaidString(PNP0700 , eisaid));
        assert(strcmp(eisaid, "PNP0700") == 0);
        
        uint64_t v = 0;
        assert(getValueFromEisaidString(eisaid, &v));
        assert(v == PNP0700);
    }
    {
        char eisaid[8] = {0};
        assert(getEisaidString(PNP0400 , eisaid));
        assert(strcmp(eisaid, "PNP0400") == 0);
        
        uint64_t v = 0;
        assert(getValueFromEisaidString(eisaid, &v));
        assert(v == PNP0400);
    }
    {
        char eisaid[8] = {0};
        assert(getEisaidString(PNP0501 , eisaid));
        assert(strcmp(eisaid, "PNP0501") == 0);
        
        uint64_t v = 0;
        assert(getValueFromEisaidString(eisaid, &v));
        assert(v == PNP0501);
    }
    {
        char eisaid[8] = {0};
        assert(getEisaidString(PNP0C0F , eisaid));
        assert(strcmp(eisaid, "PNP0C0F") == 0);
        
        uint64_t v = 0;
        assert(getValueFromEisaidString(eisaid, &v));
        assert(v == PNP0C0F);
    }
}


static void NotWorkingTests()
{
    uint64_t v = 0;
    assert(getValueFromEisaidString( "", &v) == 0);
    assert(v == 0);
    
    
    assert(getValueFromEisaidString( "123456", &v) == 0);
    assert(v == 0);
}

void Eisaid_Tests()
{
    NotWorkingTests();
    WorkingTests();
    

    
}
