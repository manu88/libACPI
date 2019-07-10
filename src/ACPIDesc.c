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

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "ACPIDesc.h"
#include "AMLParser.h"
#include <AMLDecompiler.h>


struct _DecompCtx
{
    const char*nameToFind;
    uint8_t ready;
    
    ACPINamedResource* outRes;
    
};

static int _startName(AMLDecompiler* decomp,const ParserContext* context, const char* name)
{
    struct _DecompCtx *opCtx = (struct _DecompCtx *)decomp->userData;
    assert(opCtx);

    if( strcmp(name, opCtx->nameToFind) == 0)
    {
        opCtx->ready = 1;
    }
    return 0;
}

static int _onValue(AMLDecompiler* decomp,const ParserContext* context, uint64_t value)
{
    struct _DecompCtx *opCtx = (struct _DecompCtx *)decomp->userData;
    assert(opCtx);
    
    if( opCtx->ready)
    {
        opCtx->outRes->type = ACPIObject_Type_NumericValue;
        opCtx->outRes->data = (void*) value;
        opCtx->outRes->size = sizeof(uint64_t);
        
        opCtx->ready = 0;
        return AMLParserError_ValueFound;
    }
    return 0;
}

static int _onString(AMLDecompiler* decomp,const ParserContext* context, const char* string)
{
    struct _DecompCtx *opCtx = (struct _DecompCtx *)decomp->userData;
    assert(opCtx);
    
    if( opCtx->ready)
    {
        opCtx->outRes->type = ACPIObject_Type_StringValue;
        opCtx->outRes->data = (void*) string;
        
        opCtx->ready = 0;
        return AMLParserError_ValueFound;
        
    }
    
    return 0;
}
static int _onBuffer(AMLDecompiler* decomp, const ParserContext* context , size_t bufferSize , const uint8_t* buffer)
{
    struct _DecompCtx *opCtx = (struct _DecompCtx *)decomp->userData;
    assert(opCtx);
    
    if( opCtx->ready)
    {
        opCtx->outRes->type = ACPIObject_Type_Buffer;
        opCtx->outRes->data = (void*) buffer;
        opCtx->outRes->size = bufferSize;
        
        opCtx->ready = 0;
        return AMLParserError_ValueFound;
    }
    
    return 0;
}

static int _onLargeItem(AMLDecompiler* decomp,const ParserContext* context, LargeResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize)
{
    struct _DecompCtx *opCtx = (struct _DecompCtx *)decomp->userData;
    assert(opCtx);
    
    if( opCtx->ready)
    {
        opCtx->outRes->type = ACPIObject_Type_LargeItem;
        opCtx->outRes->data = (void*) buffer;
        opCtx->outRes->size = bufferSize;
        
        opCtx->ready = 0;
        return AMLParserError_ValueFound;
    }
    
    return 0;
}
static int _onSmallItem(AMLDecompiler* decomp,const ParserContext* context, SmallResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize)
{
    struct _DecompCtx *opCtx = (struct _DecompCtx *)decomp->userData;
    assert(opCtx);
    

    if( opCtx->ready)
    {
        opCtx->outRes->type = ACPIObject_Type_SmallItem;
        opCtx->outRes->data = (void*) buffer;
        opCtx->outRes->size = bufferSize;
        
        opCtx->ready = 0;
        return AMLParserError_ValueFound;
    }
    
    return 0;
}

AMLParserError ACPIScopeGetNamedResource( const ACPIScope* scope , const char*name, ACPINamedResource* outRes)
{
    AMLDecompiler decomp;
    AMLDecompilerInit(&decomp);
    AMLDecompilerUseDefaultCallbacks(&decomp);
    
    decomp.callbacks.startName = _startName;
    decomp.callbacks.onValue = _onValue;
    decomp.callbacks.onBuffer = _onBuffer;
    decomp.callbacks.onString = _onString;
    decomp.callbacks.onSmallItem = _onSmallItem;
    decomp.callbacks.onLargeItem = _onLargeItem;
    
    struct _DecompCtx ctx = {0};
    ctx.nameToFind = name;
    ctx.outRes = outRes;
    ctx.outRes->type = ACPIObject_Type_Unknown;
    decomp.userData = &ctx;
    AMLParserError ret = AMLDecompilerStart(&decomp, scope->obj.pos, scope->obj.size);

    if( ctx.ready == 0)
    {
        return AMLParserError_ValueNotFound;
    }
    return ret;
}
