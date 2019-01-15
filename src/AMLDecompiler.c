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

static int _DecodeMemoryRangeDescriptor32(AMLParserState* parser ,ParserContext *ctx ,const uint8_t* bufferPos , size_t bufferSize)
{
    AMLDecompiler* decomp = (AMLDecompiler*) parser->userData;
    assert(decomp);
    
    MemoryRangeDescriptor32 desc;
    
    desc.writeStatus = (bufferPos[3 ] >> 7) & 1U;
    
    union Conv32
    {
        uint8_t b[4];
        uint32_t v;
    }c;
    
    
    c.b[0] = bufferPos[4];
    c.b[1] = bufferPos[5];
    c.b[2] = bufferPos[6];
    c.b[3] = bufferPos[7];
    
    desc.rangeBaseAddr = c.v;
    
    c.b[0] = bufferPos[8];
    c.b[1] = bufferPos[9];
    c.b[2] = bufferPos[10];
    c.b[3] = bufferPos[11];
    
    desc.rangeLength = c.v;
    
    if(decomp->callbacks.onLargeItem)
    {
        decomp->callbacks.onLargeItem(decomp , ctx ,
                                      LargeResourceItemsType_MemoryRangeDescriptor32,
                                      (const uint8_t*) &desc , sizeof(MemoryRangeDescriptor32));
    }
    
    return 0;
}

static int _DecodeExtendedIRQDescriptor(AMLParserState* parser ,ParserContext *ctx ,const uint8_t* bufferPos , size_t bufferSize)
{
    return 0;
}
static int _DecodeWORDAddressSpaceDescriptor(AMLParserState* parser ,ParserContext *ctx ,const uint8_t* bufferPos , size_t bufferSize)
{
    return 0;
}

static int _DecodeQWORDAddressSpaceDescriptor(AMLParserState* parser ,ParserContext *ctx ,const uint8_t* bufferPos , size_t bufferSize)
{
    
    AMLDecompiler* decomp = (AMLDecompiler*) parser->userData;
    assert(decomp);
    
    
    size_t bitOffset = 0;
    AddressSpaceDescriptor addSpaceDesc;
    addSpaceDesc.ressourceType = bufferPos[bitOffset+3];
    
    const uint8_t generalFlags = bufferPos[bitOffset+4];
    
    //Bits[7:4]Reserved (must be 0)
    assert(((generalFlags >> 7) & 1U) == 0);
    assert(((generalFlags >> 6) & 1U) == 0);
    assert(((generalFlags >> 5) & 1U) == 0);
    assert(((generalFlags >> 4) & 1U) == 0);
    
    addSpaceDesc.maf        = (generalFlags >> 3) & 1U;
    addSpaceDesc.mif        = (generalFlags >> 2) & 1U;
    addSpaceDesc.decodeType = (generalFlags >> 1) & 1U;
    addSpaceDesc.isConsumer = (generalFlags >> 0) & 1U;
    
    addSpaceDesc.typeSpecificFlags = bufferPos[bitOffset+5];
    
    
    union
    {
        uint8_t b[8];
        uint64_t v;
    } c;
    
    c.b[0] = bufferPos[bitOffset+6];
    c.b[1] = bufferPos[bitOffset+7];
    c.b[2] = bufferPos[bitOffset+8];
    c.b[3] = bufferPos[bitOffset+9];
    c.b[4] = bufferPos[bitOffset+10];
    c.b[5] = bufferPos[bitOffset+11];
    c.b[6] = bufferPos[bitOffset+12];
    c.b[7] = bufferPos[bitOffset+13];
    
    addSpaceDesc.addrSpaceGranularity = c.v;
    
    c.b[0] = bufferPos[bitOffset+14];
    c.b[1] = bufferPos[bitOffset+15];
    c.b[2] = bufferPos[bitOffset+16];
    c.b[3] = bufferPos[bitOffset+17];
    c.b[4] = bufferPos[bitOffset+18];
    c.b[5] = bufferPos[bitOffset+19];
    c.b[6] = bufferPos[bitOffset+20];
    c.b[7] = bufferPos[bitOffset+21];
    addSpaceDesc.addrRangeMin = c.v;
    
    c.b[0] = bufferPos[bitOffset+22];
    c.b[1] = bufferPos[bitOffset+23];
    c.b[2] = bufferPos[bitOffset+24];
    c.b[3] = bufferPos[bitOffset+25];
    c.b[4] = bufferPos[bitOffset+26];
    c.b[5] = bufferPos[bitOffset+27];
    c.b[6] = bufferPos[bitOffset+28];
    c.b[7] = bufferPos[bitOffset+29];
    addSpaceDesc.addrRangeMax = c.v;
    
    //translation
    c.b[0] = bufferPos[bitOffset+30];
    c.b[1] = bufferPos[bitOffset+31];
    c.b[2] = bufferPos[bitOffset+32];
    c.b[3] = bufferPos[bitOffset+33];
    c.b[4] = bufferPos[bitOffset+34];
    c.b[5] = bufferPos[bitOffset+35];
    c.b[6] = bufferPos[bitOffset+36];
    c.b[7] = bufferPos[bitOffset+37];
    addSpaceDesc.addrTranslationOffset = c.v;
    
    c.b[0] = bufferPos[bitOffset+38];
    c.b[1] = bufferPos[bitOffset+39];
    c.b[2] = bufferPos[bitOffset+40];
    c.b[3] = bufferPos[bitOffset+41];
    c.b[4] = bufferPos[bitOffset+42];
    c.b[5] = bufferPos[bitOffset+43];
    c.b[6] = bufferPos[bitOffset+44];
    c.b[7] = bufferPos[bitOffset+45];
    addSpaceDesc.addrTranslationLength = c.v;
    
    addSpaceDesc.ressourceSourceIndex = bufferPos[bitOffset+46];
    addSpaceDesc.ressourceSource = bufferPos[bitOffset+47];
    
    
    
    if(decomp->callbacks.onLargeItem)
    {
        decomp->callbacks.onLargeItem(decomp , ctx ,
                                      LargeResourceItemsType_QWORDAddressSpaceDescriptor,
                                      (const uint8_t*) &addSpaceDesc , sizeof(AddressSpaceDescriptor)
                                      );
    }
    return 0;
}

static int _DecodeBufferObject(AMLParserState* parser ,ParserContext *ctx ,const uint8_t* bufferPos , size_t bufferSize)
{
/*
    for(int i=0;i<bufferSize;i++)
    {
        if (i%8==0)
            printf("\n");
        
        printf(" 0x%x " , bufferPos[i]);
    }
 */
    AMLDecompiler* decomp = (AMLDecompiler*) parser->userData;
    assert(decomp);
    // 6.4.3.5.1
    
    const uint8_t itemType = *bufferPos; // 0x0A : large (64) / 0x07 Large (32) Item Name
    const uint8_t something = bufferPos[1];
    
    
    const size_t bitOffset = 2;
    const uint8_t addrSpaceDescriptor = bufferPos[bitOffset+0];
    const uint8_t varLen1 = bufferPos[bitOffset+1];
    const uint8_t varLen2 = bufferPos[bitOffset+2];
    const size_t varLen = (varLen1 - 0x2B ) * 255 + varLen2;
    
    if(addrSpaceDescriptor == 0x8A) // spec : 6.4.3.5.1
    {
        assert(varLen1 >= 0x2B); // says the specs
        return _DecodeQWORDAddressSpaceDescriptor(parser, ctx, bufferPos+bitOffset, bufferSize - bitOffset);
    }
    else if (addrSpaceDescriptor == 0x86)
    {
        assert(varLen1 == 0x09);
        assert(varLen2 == 0);
        
        return _DecodeMemoryRangeDescriptor32(parser, ctx, bufferPos+bitOffset, bufferSize - bitOffset);
    }
    else if (addrSpaceDescriptor == 0x88) // 6.4.3.5.3 Word Address Space Descriptor
    {
        assert(varLen1 == 0x0D);
        assert(varLen2 == 0);
        
        return _DecodeWORDAddressSpaceDescriptor(parser, ctx, bufferPos+bitOffset, bufferSize - bitOffset);
    }
    else if (addrSpaceDescriptor == 0x89)
    {
        assert(varLen1 >= 0x06);
        assert(varLen2 >= 0);
        return _DecodeExtendedIRQDescriptor(parser, ctx, bufferPos+bitOffset, bufferSize - bitOffset);
    }
    else if( addrSpaceDescriptor == 0x22) // 6.4.2.1 IRQ Descriptor
    {
        return 0;
    }
    else if( addrSpaceDescriptor == 0x47) // 6.4.2.5 I/O Port Descriptor
    {
        return 0;
    }
        
    else
    {
        printf("Other addr space type 0x%x\n" , addrSpaceDescriptor);
//        assert(0);
    }
    
    
    
    return 0;
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
        
        case ACPIObject_Type_Buffer:
        {
            
            _DecodeBufferObject(parser, &ctx, bufferPos, bufferSize);
            
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
            
            size_t adv = 0;
            const AMLOperation nextNameOp =  AMLParserPeekOp(bufferPos + 4, 1, &adv);
            //printf("\nNext op %i val 0x%x\n" , nextNameOp , bufferPos[4]);
            
            
            

        }
            break;
        case ACPIObject_NumericValue:
        //case ACPIObject_Type_DWord:
        {
            //ACPIDWord w;
            //GetDWord(bufferPos, &w);
            uint64_t w;
            GetInteger(bufferPos-1, &w);
            
            if (decomp->callbacks.OnValue)
            {
                decomp->callbacks.OnValue(decomp,&ctx , w);
            }
            /*
            if (isEisaId(w))
            {
                printf("EisaId(%llx) )\n" ,w);
            }
            else
            {
                printf("_NOT_EisaId%llx)\n" ,w);
            }
             */
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
