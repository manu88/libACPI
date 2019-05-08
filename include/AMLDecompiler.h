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

#include <AMLTypes.h>
#include <AMLParser.h>


/*
 This is an intermediate module that helps the AML bytecode interpretation.
 */
typedef struct _ParserContext
{
    
    ACPIObject_Type lastObjectType;
    
    AMLOperation nextOp;
    
    //const char* currentScope;
    
    
} ParserContext;
    
typedef struct _AMLDecompiler AMLDecompiler;

// required callbacks, undefined behaviour if NULL
typedef struct _AMLDecompilerCallbacks
{
    
    int (*startResourceTemplate)(AMLDecompiler* decomp, const ParserContext* context , size_t numItems );
    int (*endResourceTemplate)(AMLDecompiler* decomp, const ParserContext* context , size_t numItemsParsed, AMLParserError err);
    
    int (*onLargeItem)(AMLDecompiler* decomp,const ParserContext* context, LargeResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize);
    int (*onSmallItem)(AMLDecompiler* decomp,const ParserContext* context, SmallResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize);
    
    int (*onValue)(AMLDecompiler* decomp,const ParserContext* context, uint64_t value);
    int (*onString)(AMLDecompiler* decomp,const ParserContext* context, const char* string);
    
    int (*onDefinitionBlock)(AMLDecompiler* decomp,const ParserContext* context, const ACPIDefinitionBlock* block);
    
    int (*onOperationRegion)(AMLDecompiler* decomp,const ParserContext* context, const ACPIOperationRegion* reg);
    
    int (*startField)(AMLDecompiler* decomp,const ParserContext* context, const ACPIField* field);
    int (*onFieldElement)(AMLDecompiler* decomp,const ParserContext* context, const ACPIFieldElement* fieldElement);
    int (*endField)(AMLDecompiler* decomp,const ParserContext* context, const ACPIField* field);
    
    int (*startIndexField)(AMLDecompiler* decomp,const ParserContext* context, const ACPIIndexField* field);
    int (*onIndexFieldElement)(AMLDecompiler* decomp,const ParserContext* context, const ACPIIndexFieldElement* fieldElement);
    int (*endIndexField)(AMLDecompiler* decomp,const ParserContext* context, const ACPIIndexField* field);
    
    
    int (*onCreateField)(AMLDecompiler* decomp, const ParserContext* context , const ACPICreateFieldBase *field);
    int (*onBuffer)(AMLDecompiler* decomp, const ParserContext* context , size_t bufferSize , const uint8_t* buffer);
    
    int (*startScope)(AMLDecompiler* decomp,const ParserContext* context, const ACPIScope* scope);
    int (*endScope)(AMLDecompiler* decomp,const ParserContext* context, const ACPIScope* scope);
    
    int (*startDevice)(AMLDecompiler* decomp,const ParserContext* context, const ACPIDevice* name);
    int (*endDevice)(AMLDecompiler* decomp,const ParserContext* context, const ACPIDevice* name);
    
    int (*startName)(AMLDecompiler* decomp,const ParserContext* context, const char* name);
    //int (*EndName)(AMLDecompiler* ,const ParserContext* context, const char* name);
    
    int (*onMethod)(AMLDecompiler* decomp,const ParserContext* context, const ACPIMethod* method);
    
    
    int (*startPackage)(AMLDecompiler* decomp,const ParserContext* context, const ACPIPackage* package);
    int (*onPackageElement)(AMLDecompiler* decomp,const ParserContext* context, const ACPIPackageElement* element);
    int (*endPackage)(AMLDecompiler* decomp,const ParserContext* context, const ACPIPackage* package);
    //int (*endMethod)(AMLDecompiler* ,const ParserContext* context, const char* name);
    
} AMLDecompilerCallbacks;

    
typedef enum
{
    AMLState_Unknown,
    AMLState_StartedScope,
    AMLState_StartedDevice,
    AMLState_WaitingNameValue,
} AMLState;


struct _AMLDecompiler
{
    AMLParserPolicy parserPolicy;
    AMLDecompilerCallbacks callbacks;
    
    void* userData;
    const uint8_t* errorPos;
    
    AMLState state;
};


int AMLDecompilerInit(AMLDecompiler* decomp);

// add default (and empty) callbacks to an instance of AMLDecompiler. Individual callbacks can be set after. Will erase All previously set callbacks
int AMLDecompilerUseDefaultCallbacks(AMLDecompiler* decomp);

AMLParserError AMLDecompilerStart(AMLDecompiler* decomp,const uint8_t* buffer , size_t bufferSize);

#ifdef __cplusplus
}
#endif
