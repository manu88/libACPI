//
//  AMLDecompilerDefaultCallbacks.c
//  libACPI
//
//  Created by Manuel Deneu on 28/03/2019.
//  Copyright © 2019 Manuel Deneu. All rights reserved.
//

#include "AMLDecompilerDefaultCallbacks.h"
#include <AMLDecompiler.h>

static int _Default_startResourceTemplate(AMLDecompiler* decomp, const ParserContext* context , size_t numItems )
{
    return 0;
}
static int _Default_endResourceTemplate(AMLDecompiler* decomp, const ParserContext* context , size_t numItemsParsed, AMLParserError err)
{
    return 0;
}
static int _Default_onLargeItem(AMLDecompiler* decomp,const ParserContext* context, LargeResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize)
{
    return 0;
}
static int _Default_onSmallItem(AMLDecompiler* decomp,const ParserContext* context, SmallResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize)
{
    return 0;
}
static int _Default_onValue(AMLDecompiler* decomp,const ParserContext* context, uint64_t value)
{
    return 0;
}
static int _Default_onString(AMLDecompiler* decomp,const ParserContext* context, const char* string)
{
    return 0;
}
static int _Default_onDefinitionBlock(AMLDecompiler* decomp,const ParserContext* context, const ACPIDefinitionBlock* block)
{
    return 0;
}
static int _Default_onOperationRegion(AMLDecompiler* decomp,const ParserContext* context, const ACPIOperationRegion* reg)
{
    return 0;
}
static int _Default_startField(AMLDecompiler* decomp,const ParserContext* context, const ACPIField* field)
{
    return 0;
}
static int _Default_onFieldElement(AMLDecompiler* decomp,const ParserContext* context, const ACPIFieldElement* fieldElement)
{
    return 0;
}
static int _Default_endField(AMLDecompiler* decomp,const ParserContext* context, const ACPIField* field)
{
    return 0;
}
static int _Default_onBuffer(AMLDecompiler* decomp, const ParserContext* context , size_t bufferSize , const uint8_t* buffer)
{
    return 0;
}
static int _Default_startScope(AMLDecompiler* decomp,const ParserContext* context, const ACPIScope*scpe )
{
    return 0;
}
static int _Default_endScope(AMLDecompiler* decomp,const ParserContext* context, const ACPIScope*scpe)
{
    return 0;
}
static int _Default_startDevice(AMLDecompiler* decomp,const ParserContext* context, const ACPIDevice* name)
{
    return 0;
}
static int _Default_endDevice(AMLDecompiler* decomp,const ParserContext* context, const ACPIDevice* name)
{
    return 0;
}
static int _Default_startName(AMLDecompiler* decomp,const ParserContext* context, const char* name)
{
    return 0;
}
static int _Default_onMethod(AMLDecompiler* decomp,const ParserContext* context, const ACPIMethod* method)
{
    return 0;
}
static int _Default_onPackage(AMLDecompiler* decomp,const ParserContext* context, const ACPIPackage* package)
{
    return 0;
}
    



static AMLDecompilerCallbacks _defaultCallbacks =
{
  
    _Default_startResourceTemplate,
    _Default_endResourceTemplate,
    _Default_onLargeItem,
    _Default_onSmallItem,
    _Default_onValue,
    _Default_onString,
    _Default_onDefinitionBlock,
    _Default_onOperationRegion,
    _Default_startField,
    _Default_onFieldElement,
    _Default_endField,
    _Default_onBuffer,
    _Default_startScope,
    _Default_endScope,
    _Default_startDevice,
    _Default_endDevice,
    _Default_startName,
    _Default_onMethod,
    _Default_onPackage
 
};

AMLDecompilerCallbacks* AMLDecompilerGetDefaultCallbacks()
{
    return &_defaultCallbacks;
}
