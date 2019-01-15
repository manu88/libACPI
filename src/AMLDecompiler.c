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


static TreeElement* _AllocateElement(AMLParserState* parser , ACPIObject_Type forObjectType  ,const uint8_t* bufferPos , size_t bufferSize)
{
    
    assert(parser);

    static int indent = 0;
    
    AMLDecompiler* decomp = (AMLDecompiler*) parser->userData;
    assert(decomp);

    
    
    ParserContext ctx;
    

    switch (forObjectType)
    {
        case ACPIObject_Type_DefinitionBlock:
        {
            const ACPIDefinitionBlock* desc =(const ACPIDefinitionBlock*) bufferPos;
            
            if (decomp->callbacks.OnDefinitionBlock)
            {
                decomp->callbacks.OnDefinitionBlock(decomp,&ctx , desc);
            }
            
        }
            break;
        case ACPIObject_Type_Scope:
        {
            char name[5] = {0};
            
            
            ACPIScopeGetLocation(bufferPos, bufferSize , name);
            
            indent++;
            
            //for(int i=0;i<indent;i++)printf("\t");
            //printf("-----Got a scope at location '%s'\n" , name);
            
            if (decomp->callbacks.StartScope) decomp->callbacks.StartScope(decomp ,&ctx , name);
            
            
            AMLParserProcessInternalBuffer(parser, bufferPos, bufferSize);
            
            if (decomp->callbacks.EndScope) decomp->callbacks.EndScope(decomp ,&ctx , name);
            
            //for(int i=0;i<indent;i++)printf("\t");
            //printf("----- END SCOPE \n");
            
            indent--;
        }
            break;
            
        case ACPIObject_Type_Device:
        {
            
            char name[5] = {0};
            ExtractName(bufferPos, bufferSize, name);
            
            indent++;
            
            //for(int i=0;i<indent;i++)printf("\t");
            //printf("--Start Device '%s' \n" , name);
            
            if (decomp->callbacks.StartDevice)
            {
                decomp->callbacks.StartDevice(decomp ,&ctx , name);
            }
            
            AMLParserProcessInternalBuffer(parser, bufferPos, bufferSize);
            
            if (decomp->callbacks.EndDevice)
            {
                decomp->callbacks.EndDevice(decomp ,&ctx , name);
            }
            
            //for(int i=0;i<indent;i++)printf("\t");
            //printf("--End Device '%s' \n" , name);
            
            indent--;
            //AMLDecompilerStart(decomp, bufferPos, bufferSize);
        }
            break;
            
        case ACPIObject_Type_Name:
        {
            char name[5] = {0};
            const uint8_t* namePosition = bufferPos;// + pos + advancedByte;
            ExtractName(namePosition, 4, name);
            
            //for(int i=0;i<indent;i++)printf("\t");
            //printf("Name('%s' ," , name);
            
            if (decomp->callbacks.StartName)
            {
                decomp->callbacks.StartName(decomp ,&ctx , name);
            }
            
            //int (*EndName)(AMLDecompiler* , const char* name);
            /*
            size_t adv = 0;
            const AMLOperation nextNameOp =  AMLParserPeekOp(bufferPos + 4, 4, &adv);
            printf("\nNext op %i val 0x%x\n" , nextNameOp , bufferPos[4]);
            
            uint64_t val;
            GetInteger(bufferPos + 4 , &val);
            printf("Value is %llx\n" , val);
             */
        }
            break;
            
        case ACPIObject_Type_DWord:
        {
            //ACPIDWord w;
            //GetDWord(bufferPos, &w);
            uint64_t w;
            GetInteger(bufferPos-1, &w);
            if (isEisaId(w))
            {
                printf("EisaId(%llx) )\n" ,w);
            }
            else
            {
                printf("_NOT_EisaId%llx)\n" ,w);
            }
        }
            break;
        case ACPIObject_Type_VarPackage:
        {
            
            //AMLParserProcessInternalBuffer(parser, bufferPos, bufferSize);
            
        }
            break;
        case ACPIObject_Type_OperationRegion:
        {
            assert( bufferSize == sizeof(ACPIOperationRegion));
            assert(bufferPos);
            const ACPIOperationRegion* reg = (const ACPIOperationRegion*)  bufferPos;
            
            if(decomp->callbacks.onOperationRegion)
            {
                decomp->callbacks.onOperationRegion(decomp ,&ctx , reg);
            }
            
            
        }
            break;
        case ACPIObject_Type_Field:
        {
            //printf("Got a Field size %zi at pos %p\n" , bufferSize , bufferPos);
            
            ACPIField field;
            if(decomp->callbacks.onField)
            {
                ExtractName(bufferPos, 4, field.name);
                decomp->callbacks.onField(decomp ,&ctx , &field);
            }
            
        }
            break;
        case ACPIObject_Type_Method:
        {
            if (decomp->callbacks.startMethod)
            {
                decomp->callbacks.startMethod(decomp ,&ctx , NULL);
            }
            //printf("Got a method size %zi at pos %p\n" , bufferSize , bufferPos);
            
            if (decomp->callbacks.endMethod)
            {
                decomp->callbacks.endMethod(decomp ,&ctx , NULL);
            }
        }
            break;
        case ACPIObject_Type_Root:
            //printf("Start root \n");
            //for(int i=0;i<indent;i++)printf("\t");
            //printf("{\n");
            break;
        default:
            //printf("Got an element type %i\n" , forObjectType);
            break;
    }
    
    
    static TreeElement throwableElement = {0};
    
    return &throwableElement;
}


AMLParserError AMLDecompilerStart(AMLDecompiler* decomp,const uint8_t* buffer , size_t bufferSize)
{
    if (buffer == NULL || bufferSize == 0)
    {
        return AMLParserError_BufferTooShort;
    }
    
    AMLParserState parser;
    
    AMLParserInit(&parser);
    parser.startBuffer = buffer;
    parser.totalSize = bufferSize;
    
    parser.userData = decomp;
    
    //parser.callbacks.DidReadObject = _DidReadDevice;
    //parser.callbacks.DidReadDefBlock = _DidReadDefBlock;
    parser.callbacks.AllocateElement = _AllocateElement;
    
    return AMLParserProcessBuffer(&parser , buffer , bufferSize);
    
}

/*
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
 */

/*
 static AMLParserError _DidReadDevice(AMLParserState* parser  ,const ACPIDevice*device)
 {
 assert(parser);
 assert(device);
 
 
 AMLDecompiler* decomp = (AMLDecompiler*) parser->userData;
 assert(decomp);
 
 //memcpy(&doc->devices[ ACPIDocumentGetDevicesCount(doc)], device, sizeof(ACPIDevice));
 //doc->devices[doc->devicesCount] = *device;
 //doc->devicesCount++;
 
 return AMLParserError_None;
 }
 */
