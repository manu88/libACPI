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
#include "AMLBufferInterpreter.h"
#include "ItemParsers.h"

int AMLDecompilerInit(AMLDecompiler* decomp)
{
    memset(decomp, 0, sizeof(AMLDecompiler));
    
    return 1;
}
/*
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
 */

/*
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
 */





AMLParserError Parse_Reserved0(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    assert(0);
    return AMLParserError_None;
}

AMLParserError Parse_BitMemoryRangeDescriptor24(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    assert(0);
    return AMLParserError_None;
}
AMLParserError Parse_GenericRegisterDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    assert(0);
    return AMLParserError_None;
}
AMLParserError Parse_Reserved1(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    assert(0);
    return AMLParserError_None;
}
AMLParserError Parse_VendorDefinedDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    assert(0);
    return AMLParserError_None;
}

AMLParserError Parse_MemoryRangeDescriptor32(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    assert(0);
    return AMLParserError_None;
}




AMLParserError Parse_ExtendedIRQDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    assert(0);
    return AMLParserError_None;
}


AMLParserError Parse_ExtendedAddressSpaceDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    assert(0);
    return AMLParserError_None;
}


/* SMALL items */

AMLParserError ParseSmall_Reserved(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    assert(0);
    return AMLParserError_None;
}

AMLParserError ParseSmall_IRQFormatDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    assert(0);
    return AMLParserError_None;
}

AMLParserError ParseSmall_DMAFormatDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    assert(0);
    return AMLParserError_None;
}

AMLParserError ParseSmall_StartDependentFunctionsDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    assert(0);
    return AMLParserError_None;
}

AMLParserError ParseSmall_EndDependentFunctionsDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    assert(0);
    return AMLParserError_None;
}

AMLParserError ParseSmall_FixedLocationIOPortDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    assert(0);
    return AMLParserError_None;
}


AMLParserError ParseSmall_VendorDefinedDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    assert(0);
    return AMLParserError_None;
}

AMLParserError ParseSmall_EndTagDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    return AMLParserError_None;
}


/* **** **** **** **** **** **** **** */

AMLParserError Parse_ReservedStartCToF(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    assert(0);
    return AMLParserError_None;
}

// MUST MATCH SmallResourceItemsType order !
static const ParserItemMethod smallItemMethods[] =
{
    ParseSmall_Reserved,
    ParseSmall_Reserved,
    ParseSmall_Reserved,
    ParseSmall_Reserved,
    
    ParseSmall_IRQFormatDescriptor,
    ParseSmall_DMAFormatDescriptor,
    ParseSmall_StartDependentFunctionsDescriptor,
    ParseSmall_EndDependentFunctionsDescriptor,
    ParseSmall_IOPortDescriptor,
    ParseSmall_FixedLocationIOPortDescriptor,
    ParseSmall_Reserved,
    ParseSmall_Reserved,
    ParseSmall_Reserved,
    ParseSmall_Reserved,
    ParseSmall_VendorDefinedDescriptor,
    ParseSmall_EndTagDescriptor,
    
};


// MUST MATCH LargeResourceItemsType order !
static const ParserItemMethod largeItemMethods[] =
{
    Parse_Reserved0,
    Parse_BitMemoryRangeDescriptor24,
    Parse_GenericRegisterDescriptor,
    Parse_Reserved1,
    Parse_VendorDefinedDescriptor,
    Parse_MemoryRangeDescriptor32,
    Parse_FixedLocationMemoryRangeDescriptor32,
    Parse_DWORDAddressSpaceDescriptor,
    Parse_WORDAddressSpaceDescriptor,
    Parse_ExtendedIRQDescriptor,
    Parse_QWORDAddressSpaceDescriptor,
    Parse_ExtendedAddressSpaceDescriptor,
    Parse_ReservedStartCToF,
    Parse_ReservedStartCToF,
    Parse_ReservedStartCToF,
    Parse_ReservedStartCToF
};

static AMLParserError _CallbackForItem(AMLDecompiler* decomp, AMLParserState* parser ,ParserContext *ctx,const ItemEntry*entry )
{
    assert(decomp);
    assert(parser);
    assert(ctx);
    assert(entry);
    assert(entry->bufferPos != NULL);
    assert(entry->isa != NULL);
    
    if (entry->isa->isLarge)
    {
        LargeResourceItemsType realItemType = entry->isa->itemName - 0x80;
        
        return largeItemMethods[realItemType](decomp , ctx, entry->bufferPos + 3 , entry->size);
//        decomp->callbacks.onLargeItem(decomp,ctx, realItemType, entry->bufferPos , entry->size);
    }
    else // small item
    {
        SmallResourceItemsType realItemType = entry->isa->itemName >> 3;
        
        return smallItemMethods[realItemType](decomp , ctx , entry->bufferPos+1, entry->size);
    }
    
    return 0;
}

static int _DecodeBufferObjects(AMLParserState* parser ,ParserContext *ctx ,const uint8_t* buffer , size_t bufferSize)
{

    assert(parser);
    AMLDecompiler* decomp = (AMLDecompiler*) parser->userData;
    assert(decomp);
    
    size_t advance = 0;
    uint8_t s =  GetByteValue(buffer, bufferSize, &advance);
    
    const uint8_t* bufferPos = buffer + advance;
    bufferSize -= advance;
    
    assert( bufferSize == s);
    AMLBufferAnalysisResults res = { 0};
    res.ctx = ctx;
    res.parser = parser;
    const AMLParserError err =  DecodeBufferObject(bufferPos, bufferSize, &res);
    
    if (err == AMLParserError_None )
    {
        if (res.numItems == 0) // This is a regular buffer, not a resource template
        {
            if (decomp->callbacks.OnBuffer)
            {
                decomp->callbacks.OnBuffer(decomp, ctx , bufferSize , bufferPos);
            }
        }
        else
        {
            assert(res.entries[res.numItems-1].type == 0x79);// END tag
            assert(res.numItems <= MaxItemsPerBuffer);
            
            decomp->callbacks.startResourceTemplate(decomp, ctx , res.numItems);
            
            for( size_t i=0;i<res.numItems;i++)
            {
                const AMLParserError errCallback = _CallbackForItem(decomp,parser, ctx, res.entries + i);
                if ( errCallback != AMLParserError_None)
                {
                    decomp->callbacks.endResourceTemplate(decomp, ctx , i,errCallback);
                    return errCallback;
                }
                
            }
            
            decomp->callbacks.endResourceTemplate(decomp, ctx , res.numItems , AMLParserError_None);
        }
    }
    
    return err;
}


static int _OnElement(AMLParserState* parser , ACPIObject_Type forObjectType  ,const uint8_t* bufferPos , size_t bufferSize)
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
            for(int i=0;i<bufferSize;i++)
            {
                if (i%8==0)
                    printf("\n");
                
                printf(" 0x%x ", bufferPos[i]);
            }
            
            printf("\n");
            
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
                
                
                
                const uint8_t bytes = bufferPos[ 4];
                assert(bytes < 128); // bit7 : Reserved (must be 0)
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
    
    parser.callbacks.OnElement = _OnElement;
    
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
