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
    TreeElement*   (*AllocateElement)(AMLParserState* parser , ACPIObject_Type forObjectType , TreeElement*parent , const uint8_t* bufferPos , size_t bufferSize);
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


