//
//  ACPIDom.c
//  DomTest
//
//  Created by Manuel Deneu on 25/05/2019.
//  Copyright © 2019 Manuel Deneu. All rights reserved.
//

#include "ACPIDom.h"

static int onDefinitionBlock(AMLDecompiler* decomp,const ParserContext* context, const ACPIDefinitionBlock* block)
{
    return 0;
}

static int startScope(AMLDecompiler* decomp,const ParserContext* context, const ACPIScope* scope)
{
    return 0;
}

static int endScope(AMLDecompiler* decomp,const ParserContext* context, const ACPIScope* scope)
{
    return 0;
}

static int startDevice(AMLDecompiler* decomp,const ParserContext* context, const ACPIDevice* name)
{
    return 0;
}

static int endDevice(AMLDecompiler* decomp,const ParserContext* context, const ACPIDevice* name)
{
    return 0;
}

int ACPIDomParseBuffer( const uint8_t* buffer , size_t bufferSize)
{
    AMLDecompiler decomp;
    
    AMLDecompilerInit(&decomp);
    AMLDecompilerUseDefaultCallbacks(&decomp);
    decomp.callbacks.onDefinitionBlock = onDefinitionBlock;
    decomp.callbacks.startScope = startScope;
    decomp.callbacks.endScope   = endScope;
    decomp.callbacks.startDevice = startDevice;
    decomp.callbacks.endDevice = endDevice;
    
    AMLDecompilerStart(&decomp, buffer, bufferSize);
    return 0;
}
