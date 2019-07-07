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

#include "AMLDecompilerDefaultCallbacks.h"
#include <AMLDecompiler.h>

static int _Default_startResourceTemplate(AMLDecompiler* decomp, const ParserContext* context , size_t numItems )
{
    (void) decomp;
    (void) context;
    (void) numItems;
    return 0;
}
static int _Default_endResourceTemplate(AMLDecompiler* decomp, const ParserContext* context , size_t numItemsParsed, AMLParserError err)
{
    (void) decomp;
    (void) context;
    (void) numItemsParsed;
    (void) err;
    return 0;
}
static int _Default_onLargeItem(AMLDecompiler* decomp,const ParserContext* context, LargeResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize)
{
    (void) decomp;
    (void) context;
    (void) itemType;
    (void) buffer;
    (void) bufferSize;
    return 0;
}
static int _Default_onSmallItem(AMLDecompiler* decomp,const ParserContext* context, SmallResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize)
{
    (void) decomp;
    (void) context;
    (void) itemType;
    (void) buffer;
    (void) bufferSize;
    return 0;
}
static int _Default_onValue(AMLDecompiler* decomp,const ParserContext* context, uint64_t value)
{
    (void) decomp;
    (void) context;
    (void) value;
    return 0;
}
static int _Default_onString(AMLDecompiler* decomp,const ParserContext* context, const char* string)
{
    (void) decomp;
    (void) context;
    (void) string;
    return 0;
}
static int _Default_onDefinitionBlock(AMLDecompiler* decomp,const ParserContext* context, const ACPIDefinitionBlock* block)
{
    (void) decomp;
    (void) context;
    (void) block;
    return 0;
}
static int _Default_onOperationRegion(AMLDecompiler* decomp,const ParserContext* context, const ACPIOperationRegion* reg)
{
    (void) decomp;
    (void) context;
    (void) reg;
    return 0;
}
static int _Default_startField(AMLDecompiler* decomp,const ParserContext* context, const ACPIField* field)
{
    (void) decomp;
    (void) context;
    (void) field;
    return 0;
}
static int _Default_onFieldElement(AMLDecompiler* decomp,const ParserContext* context, const ACPIFieldElement* fieldElement)
{
    (void) decomp;
    (void) context;
    (void) fieldElement;
    return 0;
}
static int _Default_endField(AMLDecompiler* decomp,const ParserContext* context, const ACPIField* field)
{
    (void) decomp;
    (void) context;
    (void) field;
    return 0;
}

static int _Default_onCreateField(AMLDecompiler* decomp, const ParserContext* context , const ACPICreateFieldBase *field)
{
    (void) decomp;
    (void) context;
    (void) field;
    return 0;
}

static int _Default_onBuffer(AMLDecompiler* decomp, const ParserContext* context , size_t bufferSize , const uint8_t* buffer)
{
    (void) decomp;
    (void) context;
    (void) bufferSize;
    (void) buffer;
    return 0;
}
static int _Default_startScope(AMLDecompiler* decomp,const ParserContext* context, const ACPIScope*scope )
{
    (void) decomp;
    (void) context;
    (void) scope;
    return 0;
}
static int _Default_endScope(AMLDecompiler* decomp,const ParserContext* context, const ACPIScope*scpe)
{
    (void) decomp;
    (void) context;
    (void) scpe;
    return 0;
}
static int _Default_startDevice(AMLDecompiler* decomp,const ParserContext* context, const ACPIDevice* name)
{
    (void) decomp;
    (void) context;
    (void) name;
    return 0;
}
static int _Default_endDevice(AMLDecompiler* decomp,const ParserContext* context, const ACPIDevice* name)
{
    (void) decomp;
    (void) context;
    (void) name;
    return 0;
}
static int _Default_startName(AMLDecompiler* decomp,const ParserContext* context, const char* name)
{
    (void) decomp;
    (void) context;
    (void) name;
    return 0;
}
static int _Default_onMethod(AMLDecompiler* decomp,const ParserContext* context, const ACPIMethod* method)
{
    (void) decomp;
    (void) context;
    (void) method;
    return 0;
}
static int _Default_startPackage(AMLDecompiler* decomp,const ParserContext* context, const ACPIPackage* package)
{
    (void) decomp;
    (void) context;
    (void) package;
    return 0;
}

static int _Default_onPackageElement(AMLDecompiler* decomp,const ParserContext* context, const ACPIPackageElement* element)
{
    (void) decomp;
    (void) context;
    (void) element;
    return 0;
}
static int _Default_endPackage(AMLDecompiler* decomp,const ParserContext* context, const ACPIPackage* package)
{
    (void) decomp;
    (void) context;
    (void) package;
    return 0;
}
    
static int _Default_startIndexField(AMLDecompiler* decomp,const ParserContext* context, const ACPIIndexField* field)
{
    (void) decomp;
    (void) context;
    (void) field;
    return 0;
}
static int _Default_onIndexFieldElement(AMLDecompiler* decomp,const ParserContext* context, const ACPIIndexFieldElement* fieldElement)
{
    (void) decomp;
    (void) context;
    (void) fieldElement;
    return 0;
}

static int _Default_endIndexField(AMLDecompiler* decomp,const ParserContext* context, const ACPIIndexField* field)
{
    (void) decomp;
    (void) context;
    (void) field;
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
    _Default_startIndexField,
    _Default_onIndexFieldElement,
    _Default_endIndexField,
    _Default_onCreateField,
    _Default_onBuffer,
    _Default_startScope,
    _Default_endScope,
    _Default_startDevice,
    _Default_endDevice,
    _Default_startName,
    _Default_onMethod,
    _Default_startPackage,
    _Default_onPackageElement,
    _Default_endPackage
 
};

AMLDecompilerCallbacks* AMLDecompilerGetDefaultCallbacks()
{
    return &_defaultCallbacks;
}
