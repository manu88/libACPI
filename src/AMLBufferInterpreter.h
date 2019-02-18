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

#include <AMLParser.h>
//#include "AMLDecompiler.h"


typedef struct
{
    uint8_t itemName;
    const char* desc;
    size_t minSize; // this is the real size for small items
    uint8_t isLarge;
    
} ItemClass;

#define MaxItemsPerBuffer 32
typedef struct
{
    const uint8_t* bufferPos;
    size_t size;
    uint16_t type;
    
    const ItemClass* isa;
} ItemEntry;




typedef struct _AMLParserState AMLParserState;
typedef struct _ParserContext ParserContext;


typedef struct _AMLBufferAnalysisResults AMLBufferAnalysisResults;
typedef struct _AMLBufferAnalysisResults
{
    int (*onLargeItem)(AMLBufferAnalysisResults* results, LargeResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize);
    int (*onSmallItem)(AMLBufferAnalysisResults* results, SmallResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize);
    
    size_t numItems;
    AMLParserState* parser;
    ParserContext *ctx;
    
    ItemEntry entries[MaxItemsPerBuffer];
    
} AMLBufferAnalysisResults;

AMLParserError DecodeBufferObject(const uint8_t* bufferPos , size_t bufferSize , AMLBufferAnalysisResults* results);
