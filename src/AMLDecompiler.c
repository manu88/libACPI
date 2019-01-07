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


#include "EISAID.h"
#include "AMLDecompiler.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "ACPIParser.h"
#include "AMLHelpers.h"


int AMLDecompilerInit(AMLDecompiler* decomp)
{
    memset(decomp, 0, sizeof(AMLDecompiler));
    
    return 1;
}

static AMLParserError _DidReadDefBlock(AMLParserState* parser,const ACPIDefinitionBlock* desc)
{
    assert(parser);
    assert(desc);
    
    
    AMLDecompiler* decomp = (AMLDecompiler*) parser->userData;
    assert(decomp);
    //doc->desc = *desc;
    //doc->hasDesc = 1;
    
    // DefinitionBlock ("", "DSDT", 10, "IDOEM", "SOMEAID", 0x00001234)
    printf("DefinitionBlock (\"\", \"%s\", %u ,\"%s\",\"%s\", 0x%x)\n" ,
           desc->tableSignature ,
           desc->complianceRevision ,
           desc->OEMId,
           desc->tableId,
           desc->OEMRev
           );
    
    return AMLParserError_None;
}


static TreeElement* _AllocateElement(AMLParserState* parser , ACPIObject_Type forObjectType , TreeElement*parent ,const uint8_t* bufferPos , size_t bufferSize)
{
    
    
    assert(parent || (parent == NULL &&  forObjectType == ACPIObject_Type_Root));
    assert(parser);
    //printf("Allocate request for type %i (count %i)\n" , forObjectType , NextElementIndex);
    
    AMLDecompiler* decomp = (AMLDecompiler*) parser->userData;
    assert(decomp);

    
    switch (forObjectType)
    {
        
        case ACPIObject_Type_Scope:
        {
            char name[5] = {0};
            
            ACPIScopeGetLocation(bufferPos, bufferSize , name);
            printf("Got a scope at location '%s'\n" , name);
        }
            break;
            
        case ACPIObject_Type_Device:
        {
            
            char name[5] = {0};
            ExtractName(bufferPos, bufferSize, name);
            printf("--Start Device '%s' \n" , name);
            
            AMLParserProcessBuffer(parser, bufferPos, bufferSize);
            printf("--End Device '%s' \n" , name);
            //AMLDecompilerStart(decomp, bufferPos, bufferSize);
        }
            break;
        case ACPIObject_Type_Name:
        {
            printf("Name('%s' ," , (const char*)bufferPos);
        }
            break;
            
        case ACPIObject_Type_DWord:
        {
            ACPIDWord w;
            GetDWord(bufferPos, &w);
            
            if (isEisaId(w))
            {
                printf("EisaId(%x) )\n" ,w);
            }
            else
            {
                printf("%x)\n" ,w);
            }
        }
            break;
            
        case ACPIObject_Type_Root:
            //printf("Start root \n");
            printf("{\n");
            break;
        default:
            printf("Got an element type %i\n" , forObjectType);
            break;
    }
    
    
    static TreeElement throwableElement = {0};
    
    return &throwableElement;
}

static AMLParserError _DidReadDevice(AMLParserState* parser  ,const ACPIDevice*device)
{
    assert(parser);
    assert(device);
    
    
    AMLDecompiler* decomp = (AMLDecompiler*) parser->userData;
    assert(decomp);
    
    //memcpy(&doc->devices[ ACPIDocumentGetDevicesCount(doc)/* doc->devicesCount*/], device, sizeof(ACPIDevice));
    //doc->devices[doc->devicesCount] = *device;
    //doc->devicesCount++;
    
    return AMLParserError_None;
}


AMLParserError AMLDecompilerStart(AMLDecompiler* decomp,const uint8_t* buffer , size_t bufferSize)
{
    
    AMLParserState parser;
    
    AMLParserInit(&parser);
    parser.startBuffer = buffer;
    parser.totalSize = bufferSize;
    
    parser.userData = decomp;
    
    parser.callbacks.DidReadObject = _DidReadDevice;
    parser.callbacks.DidReadDefBlock = _DidReadDefBlock;
    parser.callbacks.AllocateElement = _AllocateElement;
    
    return AMLParserProcessBuffer(&parser , buffer , bufferSize);
    
}
