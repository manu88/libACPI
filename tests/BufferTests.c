//
//  BufferTests.c
//  Tests
//
//  Created by Manuel Deneu on 15/02/2019.
//  Copyright © 2019 Manuel Deneu. All rights reserved.
//

#include <assert.h>
#include <string.h>
#include "BufferTests.h"

#include "../src/AMLBufferInterpreter.h"

static void resTemplate1(void);
static void resTemplate2(void);
static void resTemplate3(void);

static void invalidTests()
{
    AMLBufferAnalysisResults res = { 0};
    assert(DecodeBufferObject( NULL, 10, &res) == AMLParserError_BufferTooShort );
    assert(res.numItems == 0);
    
    uint8_t c = 0;
    assert(DecodeBufferObject( &c, 0, &res) == AMLParserError_None);
    assert(res.numItems == 0);
}


static void simpleBuffer()
{
    const uint8_t b[] = {0xD,0xE,0xA,0xD,0xB,0xE,0xE,0xF};
    AMLBufferAnalysisResults res = { 0};
    assert(DecodeBufferObject( b, sizeof(b) , &res) == AMLParserError_None);
    assert(res.numItems == 0);
}

static void simpleString()
{
    const uint8_t b[] = {'H' , 'E','L' , 'L' , 'O' ,0};
    AMLBufferAnalysisResults res = { 0};
    assert(DecodeBufferObject( b, sizeof(b) ,&res) == AMLParserError_None);
    assert(res.numItems == 0);
}

void Buffers_Tests()
{
    
    invalidTests();
    simpleBuffer();
    simpleString();
    resTemplate1();
    resTemplate2();
    resTemplate3();
}

static void resTemplate2()
{
    const uint8_t b[] =
    {
        0x88,  0xd,  0x0,  0x2, 0xc,  0x0,  0x0,  0x0,
        0x0,  0x0,  0xff,  0x0, 0x0,  0x0,  0x0,  0x1,0x79,  0x0
        
    };
    
    AMLBufferAnalysisResults res = { 0};
    memset(res.entries, 0, sizeof(ItemEntry) * MaxItemsPerBuffer);
    AMLParserError err = DecodeBufferObject( b, sizeof(b),&res);
    
    assert( err == AMLParserError_None);
    assert(res.numItems == 2);
    assert( res.entries[0].bufferPos == b);
    assert(res.entries[0].size == 13);
    assert( res.entries[0].type == 0xF088);
    
    
    assert( res.entries[1].type == 0x79);
    assert( res.entries[1].size == 1);
    assert( res.entries[2].bufferPos == NULL); // because set by memset
    
    
}

static void resTemplate1()
{
    /*
     WordBusNumber (ResourceProducer, MinFixed, MaxFixed, PosDecode,
     0x0000,             // Granularity
     0x0000,             // Range Minimum
     0x00FF,             // Range Maximum
     0x0000,             // Translation Offset
     0x0100,             // Length
     ,, )
     IO (Decode16,
     0x0CF8,             // Range Minimum
     0x0CF8,             // Range Maximum
     0x01,               // Alignment
     0x08,               // Length
     )
     */
    const uint8_t b[] = {
        0x88 , 0xd,  0x0,  0x2,0xc,  0x0,  0x0,  0x0,
        0x0,  0x0,  0xff,  0x0, 0x0,  0x0,  0x0,  0x1,
        0x47,  0x1,  0xf8,  0xc, 0xf8,  0xc,  0x1,  0x8,0x79,  0x0
    };
    
    AMLBufferAnalysisResults res = { 0};
    memset(res.entries, 0, sizeof(ItemEntry) * MaxItemsPerBuffer);
    
    AMLParserError err = DecodeBufferObject( b, sizeof(b) , &res);
    
    assert( err == AMLParserError_None);
    assert(res.numItems == 3);
    
    assert( res.entries[0].bufferPos == b);
    assert( res.entries[0].type == 0xF088);
    assert( res.entries[0].isa != NULL);
    
    assert( res.entries[1].bufferPos == b+16);
    assert( res.entries[1].type == 0x0047);
    assert( res.entries[1].isa != NULL);
    
    assert( res.entries[2].type == 0x0079);//end
    assert( res.entries[2].size == 1);
    assert( res.entries[2].isa != NULL);
    
    assert( res.entries[3].bufferPos == NULL); // because set by memset
    
}


static void resTemplate3()
{
    // This one is a buffer with the same bytes as a ResourceTemplate from test 1
    const uint8_t b[] = {
        0x88,  0xd,  0x0,  0x2, 0xc,  0x0,  0x0,  0x0,
        0x0,  0x0,  0xff,  0x0, 0x0,  0x0,  0x0,  0x1
    };
    
    AMLBufferAnalysisResults res = { 0};
    memset(res.entries, 0, sizeof(ItemEntry) * MaxItemsPerBuffer);
    
    AMLParserError err = DecodeBufferObject( b, sizeof(b) , &res);
    
    assert( err == AMLParserError_None);
    assert(res.numItems == 0);
    
    assert( res.entries[0].bufferPos == NULL);
    
}

/*
 0x44,  0x53,  0x44,  0x54,  0x54,  0x0,  0x0,  0x0,
 0x64,  0x45,  0x42,  0x58,  0x50,  0x43,  0x0,  0x0,
 0x42,  0x58,  0x44,  0x53,  0x44,  0x54,  0x0,  0x0,
 0x2b,  0x0,  0x0,  0x0,  0x49,  0x4e,  0x54,  0x4c,
 0x13,  0x12,  0x18,  0x20,  0x10,  0x2f,  0x5f,  0x53,
 0x42,  0x5f,  0x5b,  0x82,  0x28,  0x50,  0x43,  0x49,
 0x30,  0x8,  0x5f,  0x48,  0x49,  0x44,  0xc,  0x41,
 0xd0,  0xa,  0x3,  0x8,  0x43,  0x52,  0x45,  0x53,
 0x11,  0x13,  0xa,  0x10,  0x88,  0xd,  0x0,  0x2,
 0xc,  0x0,  0x0,  0x0,  0x0,  0x0,  0xff,  0x0,
 0x0,  0x0,  0x0,  0x1
 */

/*
 0x44,  0x53,  0x44,  0x54,  0x56,  0x0,  0x0,  0x0,
 0x64,  0xc2,  0x42,  0x58,  0x50,  0x43,  0x0,  0x0,
 0x42,  0x58,  0x44,  0x53,  0x44,  0x54,  0x0,  0x0,
 0x2b,  0x0,  0x0,  0x0,  0x49,  0x4e,  0x54,  0x4c,
 0x13,  0x12,  0x18,  0x20,  0x10,  0x31,  0x5f,  0x53,
 0x42,  0x5f,  0x5b,  0x82,  0x2a,  0x50,  0x43,  0x49,
 0x30,  0x8,  0x5f,  0x48,  0x49,  0x44,  0xc,  0x41,
 0xd0,  0xa,  0x3,  0x8,  0x43,  0x52,  0x45,  0x53,
 0x11,  0x15,  0xa,  0x12,  0x88,  0xd,  0x0,  0x2,
 0xc,  0x0,  0x0,  0x0,  0x0,  0x0,  0xff,  0x0,
 0x0,  0x0,  0x0,  0x1,  0x79,  0x0
 */
/*
 0x44,  0x53,  0x44,  0x54,  0x5e,  0x0,  0x0,  0x0,
 0x64,  0x41,  0x42,  0x58,  0x50,  0x43,  0x0,  0x0,
 0x42,  0x58,  0x44,  0x53,  0x44,  0x54,  0x0,  0x0,
 0x2b,  0x0,  0x0,  0x0,  0x49,  0x4e,  0x54,  0x4c,
 0x13,  0x12,  0x18,  0x20,  0x10,  0x39,  0x5f,  0x53,
 0x42,  0x5f,  0x5b,  0x82,  0x32,  0x50,  0x43,  0x49,
 0x30,  0x8,  0x5f,  0x48,  0x49,  0x44,  0xc,  0x41,
 0xd0,  0xa,  0x3,  0x8,  0x43,  0x52,  0x45,  0x53,
 0x11,  0x1d,  0xa,  0x1a,  0x88,  0xd,  0x0,  0x2,
 0xc,  0x0,  0x0,  0x0,  0x0,  0x0,  0xff,  0x0,
 0x0,  0x0,  0x0,  0x1,  0x47,  0x1,  0xf8,  0xc,
 0xf8,  0xc,  0x1,  0x8,  0x79,  0x0,
 */
