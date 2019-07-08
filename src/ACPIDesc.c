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
    void* datas;
    uint8_t ready;
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

void *ACPIScopeGetName( const ACPIScope* scope , const char*name)
{
    AMLDecompiler decomp;
    AMLDecompilerInit(&decomp);
    AMLDecompilerUseDefaultCallbacks(&decomp);
    
    decomp.callbacks.startName = _startName;
    
    struct _DecompCtx ctx = {0};
    ctx.nameToFind = name;
    decomp.userData = &ctx;
    AMLDecompilerStart(&decomp, scope->obj.pos, 0);
    return NULL;
}
