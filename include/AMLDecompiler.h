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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "AMLParser.h"

typedef struct
{
    ACPIObject_Type lastObjectType;
} ParserContext;
    
typedef struct _AMLDecompiler AMLDecompiler;

typedef struct
{
    //AMLParserError (*DidReadDefBlock)(AMLParserState* parser,const ACPIDefinitionBlock* block);
    //AMLParserError (*DidReadObject)(AMLParserState* parser  ,const ACPIDevice*device  );
    //TreeElement*   (*AllocateElement)(AMLParserState* parser , ACPIObject_Type forObjectType  , const uint8_t* bufferPos , size_t bufferSize);
    
    
    
    int (*onLargeItem)(AMLDecompiler*,const ParserContext* context, LargeResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize);
    int (*onSmallItem)(AMLDecompiler*,const ParserContext* context, SmallResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize);
    
    int (*OnValue)(AMLDecompiler*,const ParserContext* context, uint64_t value);
    
    int (*OnDefinitionBlock)(AMLDecompiler*,const ParserContext* context, const ACPIDefinitionBlock* block);
    
    int (*onOperationRegion)(AMLDecompiler*,const ParserContext* context, const ACPIOperationRegion*);
    
    int (*onField)(AMLDecompiler*,const ParserContext* context, const ACPIField*);
    
    int (*StartScope)(AMLDecompiler* ,const ParserContext* context, const char* location);
    int (*EndScope)(AMLDecompiler* ,const ParserContext* context, const char* location);
    
    int (*StartDevice)(AMLDecompiler* ,const ParserContext* context, const char* name);
    int (*EndDevice)(AMLDecompiler* ,const ParserContext* context, const char* name);
    
    int (*StartName)(AMLDecompiler* ,const ParserContext* context, const char* name);
    int (*EndName)(AMLDecompiler* ,const ParserContext* context, const char* name);
    
    int (*startMethod)(AMLDecompiler* ,const ParserContext* context, const char* name);
    int (*endMethod)(AMLDecompiler* ,const ParserContext* context, const char* name);
    
} AMLDecompilerCallbacks;


struct _AMLDecompiler
{
    AMLDecompilerCallbacks callbacks;
    void* userData;
};


int AMLDecompilerInit(AMLDecompiler* decomp);



AMLParserError AMLDecompilerStart(AMLDecompiler* decomp,const uint8_t* buffer , size_t bufferSize);

#ifdef __cplusplus
}
#endif
