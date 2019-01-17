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

#pragma once

#include "ACPIDesc.h"
#include "AMLRouter.h"

typedef enum
{
    AMLParserError_None             = 0,
    AMLParserError_BufferTooShort,
    AMLParserError_UnexpectedToken,
    AMLParserError_ElementAllocError,
    AMLParserUserAbord, // might be returned from Callbacks
    
} AMLParserError;

typedef struct _AMLParserState AMLParserState;

/*
 This is the low level AML Bytecode parser.
 */
typedef struct
{
    int   (*AllocateElement)(AMLParserState* parser , ACPIObject_Type forObjectType  , const uint8_t* bufferPos , size_t bufferSize);
} AMLParserCallbacks;



typedef struct
{
    uint8_t assertOnError; // default to NO
} AMLParserPolicy;
struct _AMLParserState
{
    AMLParserPolicy parserPolicy;
    AMLParserCallbacks callbacks;
    void *userData;
    
    const uint8_t* startBuffer;
    size_t   totalSize;
    
    const uint8_t* errorPos;
} ;

typedef struct _AMLParserState AMLParserState;

int AMLParserInit(AMLParserState* state);


 
AMLParserError AMLParserProcessBuffer(AMLParserState* state, const uint8_t* buffer , size_t bufSize);
AMLParserError AMLParserProcessInternalBuffer(AMLParserState* state, const uint8_t* buffer , size_t bufSize);


AMLOperation AMLParserPeekOp( const uint8_t* buffer , size_t bufSize , size_t *advance);
