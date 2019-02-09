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
#include "AMLHelpers.h"


int AMLDecompilerInit(AMLDecompiler* decomp)
{
    memset(decomp, 0, sizeof(AMLDecompiler));
    
    return 1;
}

static size_t _DecodeMemoryRangeDescriptor32(AMLParserState* parser ,ParserContext *ctx ,const uint8_t* bufferPos , size_t bufferSize)
{
    //assert(bufferSize >= sizeof(MemoryRangeDescriptor32));
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
    
    return bufferSize;
}

static size_t _DecodeExtendedIRQDescriptor(AMLParserState* parser ,ParserContext *ctx ,const uint8_t* bufferPos , size_t bufferSize)
{
    assert(0);
    return 0;
}

// 6.4.3.5.3 Word Address Space Descriptor
static size_t _DecodeWORDAddressSpaceDescriptor(AMLParserState* parser ,ParserContext *ctx ,const uint8_t* bufferPos , size_t bufferSize)
{
    
 
    AMLDecompiler* decomp = (AMLDecompiler*) parser->userData;
    assert(decomp);
    
    WordAddressSpaceDescriptor desc;
    
    desc.ressourceType     = bufferPos[3];
    desc.generalFlags      = bufferPos[4];
    desc.typeSpecificFlags = bufferPos[5];
    
    
    union WordConv
    {
        uint8_t b[2];
        uint16_t w;
    } conv;
    
    conv.b[0] = bufferPos[6];
    conv.b[1] = bufferPos[7];
    
    desc.addrSpaceGranularity = conv.w;
    
    conv.b[0] = bufferPos[8];
    conv.b[1] = bufferPos[9];
    
    desc.addrRangeMin = conv.w;
    
    conv.b[0] = bufferPos[10];
    conv.b[1] = bufferPos[11];
    
    desc.addrRangeMax = conv.w;
    
    conv.b[0] = bufferPos[12];
    conv.b[1] = bufferPos[13];
    
    desc.addrTranslationOffset = conv.w;
    
    conv.b[0] = bufferPos[14];
    conv.b[1] = bufferPos[15];
    
    desc.addrTranslationLength = conv.w;
    
    
    desc.ressourceSourceIndex = bufferPos[16];
    
    
    if(decomp->callbacks.onLargeItem)
    {
        decomp->callbacks.onLargeItem(decomp , ctx ,
                                      LargeResourceItemsType_WORDAddressSpaceDescriptor,
                                      (const uint8_t*) &desc , sizeof(WordAddressSpaceDescriptor)
                                      );
    }
    
    
    return bufferSize;
}

static size_t _DecodeQWORDAddressSpaceDescriptor(AMLParserState* parser ,ParserContext *ctx ,const uint8_t* bufferPos , size_t bufferSize)
{
    
    AMLDecompiler* decomp = (AMLDecompiler*) parser->userData;
    assert(decomp);
    //assert(bufferSize >= sizeof(AddressSpaceDescriptor));
    
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
    return bufferSize;
}

static size_t _DecodeIRQDescriptor(AMLParserState* parser ,ParserContext *ctx ,const uint8_t* bufferPos , size_t bufferSize)
{
    return bufferSize;
}

static size_t _DecodeIOPortDescriptor(AMLParserState* parser ,ParserContext *ctx ,const uint8_t* bufferPos , size_t bufferSize)
{
    AMLDecompiler* decomp = (AMLDecompiler*) parser->userData;
    assert(decomp);
    
    // This is a small item
    
    IOPortDescriptor desc = {0};
    desc.information = bufferPos[0];
    
    union
    {
        uint8_t b[8];
        uint16_t v;
    } c;
    
    c.b[0] = bufferPos[1];
    c.b[1] = bufferPos[2];
    
    desc.rangeMinBaseAddr = c.v;
    
    c.b[0] = bufferPos[3];
    c.b[1] = bufferPos[4];
    
    desc.rangeMaxBaseAddr = c.v;
    
    desc.baseAlign = bufferPos[5];
    desc.rangeLen = bufferPos[6];
    
    
    if(decomp->callbacks.onSmallItem)
    {
        decomp->callbacks.onSmallItem(decomp , ctx ,
                                      SmallResourceItemsType_IOPortDescriptor,
                                      (const uint8_t*) &desc , sizeof(IOPortDescriptor)
                                      );
    }
    
    return bufferSize;
}

static size_t _DecodeBufferObject(AMLParserState* parser ,ParserContext *ctx ,const uint8_t* bufferPos , size_t bufferSize)
{

    AMLDecompiler* decomp = (AMLDecompiler*) parser->userData;
    assert(decomp);
    // 6.4.3.5.1
    
    const uint8_t itemType = *bufferPos; // 0x0A : large (64) / 0x07 Large (32) Item Name
    //const uint8_t something = bufferPos[1];
    
    
    size_t bitOffset = 2;
    const uint8_t addrSpaceDescriptor = bufferPos[bitOffset+0];
    
    const uint8_t varLen1 = bufferPos[bitOffset+1];
    const uint8_t varLen2 = bufferPos[bitOffset+2];
    
    
    union WordConv
    {
        uint8_t b[2];
        uint16_t w;
    } conv;
    
    conv.b[0] = bufferPos[bitOffset+1];
    conv.b[1] = bufferPos[bitOffset+2];
    
    
    const uint16_t varLen = conv.w;
    
    
    if (itemType == 0x0A)
    {
        if(addrSpaceDescriptor == 0x8A) // spec : 6.4.3.5.1
        {
            assert(varLen1 >= 0x2B); // says the specs
            return _DecodeQWORDAddressSpaceDescriptor(parser, ctx, bufferPos+bitOffset,varLen/* bufferSize - bitOffset*/) + bitOffset;
        }
        else if (addrSpaceDescriptor == 0x86)
        {
            assert(varLen1 == 0x09);
            assert(varLen2 == 0);
            
            return _DecodeMemoryRangeDescriptor32(parser, ctx, bufferPos+bitOffset,  varLen/* bufferSize - bitOffset*/);
        }
        else if (addrSpaceDescriptor == 0x87) // 6.4.3.5.2 DWord Address Space Descriptor
        {
            assert(0);
        }
        else if (addrSpaceDescriptor == 0x88) // 6.4.3.5.3 Word Address Space Descriptor
        {
            assert(varLen1 == 0x0D);
            assert(varLen2 == 0);
            
            return _DecodeWORDAddressSpaceDescriptor(parser, ctx, bufferPos+bitOffset,  varLen/* bufferSize - bitOffset*/) + bitOffset +4;
        }
        else if (addrSpaceDescriptor == 0x89)
        {
            assert(varLen1 >= 0x06);
            assert(varLen2 >= 0);
            return _DecodeExtendedIRQDescriptor(parser, ctx, bufferPos+bitOffset,varLen/* bufferSize - bitOffset*/);
        }
        else if( addrSpaceDescriptor == 0x22) // 6.4.2.1 IRQ Descriptor
        {
            return _DecodeIRQDescriptor(parser, ctx, bufferPos+bitOffset, varLen);
        }
        else if( addrSpaceDescriptor == 0x47) // 6.4.2.5 I/O Port Descriptor
        {
            return _DecodeIOPortDescriptor(parser, ctx, bufferPos+bitOffset, varLen);
        }
        else // This is a regular buffer
        {
            const uint8_t bufSize = bufferPos[1];
            
            const uint8_t* buffer = bufferPos+2;
            
            if (decomp->callbacks.OnBuffer)
            {
                decomp->callbacks.OnBuffer(decomp, ctx , bufSize , buffer);
            }
            /*
            for(int i=0;i<bufferSize;i++)
            {
                if (i%8==0)
                    printf("\n");
                
                printf(" 0x%x " , bufferPos[i]);
            }
            printf("\n");
            
            printf("Other addr space type 0x%x\n" , addrSpaceDescriptor);
            
            assert(decomp->parserPolicy.assertOnError == 0);
            return AMLParserError_UnexpectedToken;*/
        }
    }
    else
    {
/*
         for(int i=0;i<bufferSize;i++)
         {
             if (i%8==0)
                 printf("\n");
         
             printf(" 0x%x " , bufferPos[i]);
         }

 
        printf("Other Item Type type 0x%x\n" , itemType);
        assert(0); // to implement
        return 0;//AMLParserError_UnexpectedToken;
 */
//        assert(0);
        return bufferSize;
    }
    
    
    
    return varLen;
}

static int _DecodeBufferObjects(AMLParserState* parser ,ParserContext *ctx ,const uint8_t* bufferPos , size_t bufferSize)
{

    ssize_t size = bufferSize;
    size_t parsed = 0;
    while (size> 0)
    {
        const uint8_t* newBuf = bufferPos + parsed;
        size_t objectSize = _DecodeBufferObject(parser, ctx, newBuf, size);
        
        size -= objectSize;
        parsed += objectSize;
        
        
    }
    //return _DecodeBufferObject(parser, ctx, bufferPos, bufferSize) != 0? AMLParserError_None : AMLParserError_BufferTooShort;
    return AMLParserError_None;
}


int _AllocateElement(AMLParserState* parser , ACPIObject_Type forObjectType  ,const uint8_t* bufferPos , size_t bufferSize)
{
    
    assert(parser);

    AMLDecompiler* decomp = (AMLDecompiler*) parser->userData;
    assert(decomp);

        
    ParserContext ctx;
    ctx.currentScope = decomp->currentScope;
    ctx.nextOp = AML_Unknown;
    

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
            char name[SCOPE_STR_SIZE];

            size_t advanced = ResolvePath(name,  bufferPos);

            if (decomp->callbacks.StartScope)
            {
                decomp->callbacks.StartScope(decomp ,&ctx , name);
            }
            
            strncpy(decomp->currentScope, name, SCOPE_STR_SIZE);
            
            AMLParserError err =  AMLParserProcessInternalBuffer(parser, bufferPos + advanced, bufferSize-advanced);
            if(err != AMLParserError_None)
                return err;
            
            if (decomp->callbacks.EndScope)
            {
                decomp->callbacks.EndScope(decomp ,&ctx , name);
            }
            
            
        }
            break;
        
        case ACPIObject_Type_Buffer:
        {
            
            AMLParserError err =  _DecodeBufferObjects(parser, &ctx, bufferPos, bufferSize);
            if(err != AMLParserError_None)
                return err;
            
            
        }
            break;
        case ACPIObject_Type_Device:
        {
            ACPIDevice dev;
            
            const uint8_t nameSize = ExtractName(bufferPos, bufferSize, dev.name);
            dev.name[4] = 0;
            //printf("Device '%.4s' - " , dev.id);
            
            
            //for(int i=0;i<indent;i++)printf("\t");
            //printf("--Start Device '%s' \n" , name);
            
            
            if (decomp->callbacks.StartDevice)
            {
                decomp->callbacks.StartDevice(decomp ,&ctx , &dev);
            }
            // advance bufferPos to skip the name
            AMLParserError err =  AMLParserProcessInternalBuffer(parser, bufferPos + nameSize, bufferSize-nameSize);
            if(err != AMLParserError_None)
                return err;
            
            
            if (decomp->callbacks.EndDevice)
            {
                decomp->callbacks.EndDevice(decomp ,&ctx , &dev);
            }
            
        }
            break;
            
        case ACPIObject_Type_Name:
        {

            char name[5] = {0};
            const uint8_t* namePosition = bufferPos;// + pos + advancedByte;
            ExtractName(namePosition, 5, name);
            name[4] = 0;
            
            size_t adv = 0;
            ctx.nextOp =  AMLParserPeekOp(bufferPos + 4, 1, &adv);
            
            if(   ctx.nextOp !=  AML_QWordPrefix
               && ctx.nextOp !=  AML_OneOp
               && ctx.nextOp !=  AML_ZeroOp
               && ctx.nextOp !=  AML_DWordPrefix
               && ctx.nextOp !=  AML_BufferOp
               && ctx.nextOp !=  AML_BytePrefix
               && ctx.nextOp !=  AML_PackageOp
               && ctx.nextOp !=  AML_StringPrefix // really allowed ?
               )
            {
                return AMLParserError_UnexpectedToken;
            }
            
            if (decomp->callbacks.StartName)
            {
                decomp->callbacks.StartName(decomp ,&ctx , name);
            }
        }
            break;
        case ACPIObject_NumericValue:
        {
            uint64_t w;
            GetInteger(bufferPos,bufferSize, &w);
            
            if (decomp->callbacks.OnValue)
            {
                decomp->callbacks.OnValue(decomp,&ctx , w);
            }

        }
            break;
        case ACPIObject_StringValue:
        {
            if (decomp->callbacks.OnString)
            {
                decomp->callbacks.OnString(decomp, &ctx , (const char*) bufferPos);
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
            ACPIField field;
            if(decomp->callbacks.onField)
            {
                ExtractName(bufferPos, 4, field.name);
                field.name[4] = 0;
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
            
            if (decomp->callbacks.endMethod)
            {
                decomp->callbacks.endMethod(decomp ,&ctx , NULL);
            }
        }
            break;
        case ACPIObject_Type_Root:

            break;
        default:
            assert(0);
            //printf("Got an element type %i\n" , forObjectType);
            break;
    }
    
    
    return AMLParserError_None;
}


AMLParserError AMLDecompilerStart(AMLDecompiler* decomp,const uint8_t* buffer , size_t bufferSize)
{
    if (buffer == NULL || bufferSize == 0)
    {
        return AMLParserError_BufferTooShort;
    }
    
    AMLParserState parser;
    
    AMLParserInit(&parser);
    
    parser.parserPolicy = decomp->parserPolicy;
    
    parser.startBuffer = buffer;
    parser.totalSize = bufferSize;
    
    parser.userData = decomp;
    
    parser.callbacks.AllocateElement = _AllocateElement;
    
    AMLParserError ret =  AMLParserProcessBuffer(&parser , buffer , bufferSize);
    decomp->errorPos = parser.errorPos;
    
    return ret;
    
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
