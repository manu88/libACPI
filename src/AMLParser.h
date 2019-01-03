//
//  AMLParser.h
//  KernelTaskV1
//
//  Created by Manuel Deneu on 25/12/2018.
//  Copyright © 2018 Manuel Deneu. All rights reserved.
//

#ifndef AMLParser_h
#define AMLParser_h

#include "ACPIDesc.h"
#include "TreeRef.h"

typedef enum
{
    AMLParserError_None = 0,
    AMLParserError_BufferTooShort,
    AMLParserError_UnexpectedToken,
    AMLParserError_ElementAllocError,
    AMLParserUserAbord, // might be returned from Callbacks
    
} AMLParserError;

typedef struct _AMLParserState AMLParserState;

typedef struct
{
    AMLParserError (*DidReadDefBlock)(AMLParserState* parser,const ACPIDefinitionBlock* block);
    AMLParserError (*DidReadObject)(AMLParserState* parser  ,const ACPIDevice*device  );
    TreeElement*   (*AllocateElement)(AMLParserState* parser , ACPIObject_Type forObjectType , TreeElement*parent);
} AMLParserCallbacks;

struct _AMLParserState
{
    AMLParserCallbacks callbacks;
    void *userData;
    
    const uint8_t* startBuffer;
    size_t   totalSize;
} ;

typedef struct _AMLParserState AMLParserState;

int AMLParserInit(AMLParserState* state);

AMLParserError AMLParserProcessBuffer(AMLParserState* state, const uint8_t* buffer , size_t bufSize);





#endif /* AMLParser_h */
