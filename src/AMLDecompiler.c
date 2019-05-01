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
#include "AMLDecompilerDefaultCallbacks.h"

int AMLDecompilerInit(AMLDecompiler* decomp)
{
    memset(decomp, 0, sizeof(AMLDecompiler));
    
    return 1;
}

int AMLDecompilerUseDefaultCallbacks(AMLDecompiler* decomp)
{
    assert(decomp);
    
    return memcpy(&decomp->callbacks, AMLDecompilerGetDefaultCallbacks(), sizeof(AMLDecompilerCallbacks)) == &decomp->callbacks;
}

static void setState( AMLDecompiler* decomp, AMLState newState)
{
    decomp->state = newState;
}

static int exceptState( AMLDecompiler* decomp, AMLState state)
{
    if (decomp->parserPolicy.assertOnInvalidState)
    {
        assert(decomp->state == state);
    }
    return decomp->state == state;
}


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
    
    //size_t a = 0;
    //AMLOperation nextOp  =  AMLParserPeekOp(buffer, bufferSize, &a);
    
    /*
    if (nextOp == AML_DWordPrefix)
    {
        // THis is a simple DWord value buffer
        assert(0);
    }
     */
    
    //size_t adv = 0;
    uint64_t numOfBufferElements = 0;
    //uint8_t valSize = ExtractInteger(buffer, bufferSize, &advance, &numOfBufferElements);
    
    //uint64_t val = 0;
    advance  +=  GetInteger(buffer, bufferSize, &numOfBufferElements);
    
    //assert(numOfBufferElements == val);
    //assert(valSize == 1 || valSize == 4 );
    
    //uint8_t __s =  GetByteValue(buffer, bufferSize, &advance);
    //assert(numOfBufferElements == __s);
    
    const uint8_t* bufferPos = buffer + advance;
    bufferSize -= advance;
    
    // buffer size can be
    //assert( bufferSize == numOfBufferElements);
    AMLBufferAnalysisResults res = { 0};
    res.ctx = ctx;
    res.parser = parser;
    const AMLParserError err =  DecodeBufferObject(bufferPos, bufferSize, &res);
    
    if (err == AMLParserError_None )
    {
        if (res.numItems == 0) // This is a regular buffer, not a resource template
        {
            // Pass null to buffer is bufferSize == 0
            decomp->callbacks.onBuffer(decomp, ctx , numOfBufferElements ,bufferSize == 0? NULL: bufferPos );
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

static ssize_t _DecodeFieldElement(AMLParserState* parser ,ParserContext *ctx ,const uint8_t* buffer , size_t bufferSize , size_t* offsetAccum,const ACPIField *fieldRef)
{
    assert(offsetAccum);
    AMLDecompiler* decomp = (AMLDecompiler*) parser->userData;
    assert(decomp);
    
    ACPIFieldElement element = {0};
    element.fieldRef = fieldRef;
    
    size_t offsetBeforeValue = 0;
    if (buffer[0] == 0) // anonymous
    {
        offsetBeforeValue = 1;
        
    }
    else if (IsName(buffer[0]))
    {
        offsetBeforeValue = ExtractNameString(buffer, 4, element.name);
    }
    
    size_t advancedOf = 0;
    element.value = GetPackageLength(buffer+offsetBeforeValue, bufferSize-offsetBeforeValue, &advancedOf);
    
    *offsetAccum += element.value;
    element.offsetFromStart = *offsetAccum;
    
    int retCallB = decomp->callbacks.onFieldElement(decomp , ctx , &element);
    
    if (retCallB <0)
        return  retCallB;
    
    return advancedOf + offsetBeforeValue;
}

static int _DecodeFieldElementList(AMLParserState* parser ,ParserContext *ctx ,const uint8_t* fieldElementsData , size_t fieldElementsDataSize,const ACPIField *fieldRef)
{
    
    size_t offsetAccum = 0;
    while ( fieldElementsDataSize >0 )
    {
        const ssize_t retElement =  _DecodeFieldElement(parser, ctx, fieldElementsData, fieldElementsDataSize, &offsetAccum,  fieldRef);
        assert(retElement >=0);
        fieldElementsData+=retElement;
        fieldElementsDataSize -= retElement;
    }
    
    assert(fieldElementsDataSize == 0); // We must have consumed all the field element data size
    
    return 0;
}

static ssize_t _DecodeIndexFieldElement(AMLDecompiler* decomp ,ParserContext *ctx ,const uint8_t* buffer , size_t bufferSize , size_t* offsetAccum,const ACPIIndexField *fieldRef)
{
    assert(offsetAccum);
    
    
    ACPIFieldElement element = {0};
    element.fieldRef = fieldRef;
    
    size_t offsetBeforeValue = 0;
    if (buffer[0] == 0) // anonymous
    {
        offsetBeforeValue = 1;
        
    }
    else if (IsName(buffer[0]))
    {
        offsetBeforeValue = ExtractNameString(buffer, 4, element.name);
    }
    
    size_t advancedOf = 0;
    element.value = GetPackageLength(buffer+offsetBeforeValue, bufferSize-offsetBeforeValue, &advancedOf);
    
    
    element.offsetFromStart = *offsetAccum;
    
    int callb = decomp->callbacks.onIndexFieldElement(decomp , ctx , &element);
    if (callb != 0)
    {
        return callb;
    }
    *offsetAccum += element.value;
    /*
    int retCallB = decomp->callbacks.onFieldElement(decomp , ctx , &element);
    if (retCallB <0)
        return  retCallB;
    */
    return advancedOf + offsetBeforeValue;
}

static AMLParserError _DecodeFieldFlags( const uint8_t byte, ACPIFieldFlags* toFlags)
{
    if ( (byte & 0b10000000) !=0)
    {
        return AMLParserError_UnexpectedValue;
    }
    assert(toFlags);
    toFlags->accessType = byte & 0b00001111;
    toFlags->lockRule   = (byte & 0b00010000) >> 4;
    toFlags->updateRule = (byte & 0b01100000) >> 5;
    return AMLParserError_None;
}

static int _DecodeIndexField(AMLParserState* parser ,ParserContext *ctx ,const uint8_t* buffer , size_t bufferSize)
{
    AMLDecompiler* decomp = (AMLDecompiler*) parser->userData;
    assert(decomp);
    
    ACPIIndexField indexField = {0};
    
    
    const ssize_t retParseIndexName = AMLNameCreateFromBuffer(&indexField.name, buffer, bufferSize);
    assert(retParseIndexName > 0);
    
    const ssize_t retParseDataName = AMLNameCreateFromBuffer(&indexField.dataName, buffer+retParseIndexName, bufferSize-retParseIndexName);
    assert(retParseDataName > 0);
    
    char*name = AMLNameConstructNormalized(&indexField.name);
    char*datname = AMLNameConstructNormalized(&indexField.dataName);
    assert(name);
    assert(datname);
    
    const uint8_t bytesConfig = buffer[  retParseIndexName + retParseDataName];
    
    const int retFlags = _DecodeFieldFlags(bytesConfig, &indexField.flags);
    
    if (retFlags != AMLParserError_None)
        return retFlags;
    
    
    
    int callb = decomp->callbacks.startIndexField(decomp , ctx , &indexField);
    if (callb != 0)
        return callb;
    
    
    const uint8_t*  indexElementsStart =  buffer + retParseIndexName + retParseDataName + 1;
    size_t   indexElementsSize  = bufferSize - retParseIndexName - retParseDataName - 1;
    
    if (!indexElementsStart || indexElementsSize == 0)
    {
        return AMLParserError_BufferTooShort;
    }
    size_t offsetAccum = 0;
    while (indexElementsSize)
    {
        const ssize_t retElement = _DecodeIndexFieldElement(decomp, ctx, indexElementsStart, indexElementsSize , &offsetAccum , &indexField);
        assert(retElement >=0);
        indexElementsSize -= (size_t)retElement;
        indexElementsStart += retElement;
        
    }
    
    callb = decomp->callbacks.endIndexField(decomp , ctx , &indexField);
    if (callb != 0)
        return callb;
    
    return 0;
}



static int _DecodeField(AMLParserState* parser ,ParserContext *ctx ,const uint8_t* buffer , size_t bufferSize)
{
    AMLDecompiler* decomp = (AMLDecompiler*) parser->userData;
    assert(decomp);
    
    
    
    //AMLName fieldName = {0};
    
    ACPIField field ={0} ;
    
    const ssize_t retParseFieldName = AMLNameCreateFromBuffer(&field.name, buffer, bufferSize);
    
    if (retParseFieldName < 0)
        return AMLParserError_BufferTooShort;
    
    uint8_t advancedFromName = (uint8_t) retParseFieldName;// ExtractNameString(buffer, 32, field._name);
    assert(advancedFromName);
    //assert(retParseFieldName == advancedFromName);
    //const uint8_t nameSize = ExtractName(buffer, 4, field.name, &advancedFromName);
    
    //assert( field.name[nameSize] == 0);
    //field.name[nameSize] = 0;
    
    const uint8_t bytesConfig = buffer[ advancedFromName ];
    
    const int retFlags = _DecodeFieldFlags(bytesConfig, &field.flags);
    if ( retFlags != AMLParserError_None)
        return retFlags;
    
    
    
    int retCallb = decomp->callbacks.startField(decomp ,ctx , &field);
    if (retCallb != AMLParserError_None)
    {
        return retCallb;
    }
    
    // This is the payload
    uint8_t* fieldElementsData = (uint8_t*) buffer + advancedFromName +1;
    ssize_t  fieldElementsDataSize = bufferSize - advancedFromName- 1;

    
    const int retDecodeList =  _DecodeFieldElementList(parser, ctx, fieldElementsData, fieldElementsDataSize, &field);
    
    if (retDecodeList != AMLParserError_None)
        return retDecodeList;
    
    retCallb = decomp->callbacks.endField(decomp ,ctx , &field);
    if (retCallb != AMLParserError_None)
    {
        return retCallb;
    }

    
    return AMLParserError_None;
}


static AMLParserError _DecodePackage(AMLParserState* parser ,ParserContext *ctx ,const ACPIPackage* package)
{
    AMLDecompiler* decomp = (AMLDecompiler*) parser->userData;
    
    AMLParserError err = decomp->callbacks.startPackage( decomp , ctx , package);
    
    if (err != AMLParserError_None)
    {
        return err;
    }
    
    // Start parsing the package content
    err = AMLParserProcessPackageContent(parser,package, package->buffer, package->bufSize);
    
    if (err != AMLParserError_None)
    {
        return err;
    }
    
    err = decomp->callbacks.endPackage( decomp , ctx , package);
    
    if (err != AMLParserError_None)
    {
        return err;
    }
    
    return AMLParserError_None;
}

static int _OnElement(AMLParserState* parser , ACPIObject_Type forObjectType  ,const uint8_t* bufferPos , size_t bufferSize)
{
    assert(parser);
    AMLDecompiler* decomp = (AMLDecompiler*) parser->userData;
    assert(decomp);
    
    ParserContext ctx;
    //ctx.currentScope = decomp->currentScope;
    ctx.nextOp = AML_Unknown;
    AMLParserError err = AMLParserError_None;
    
    switch (forObjectType)
    {
        case ACPIObject_Type_DefinitionBlock:
        {
            const ACPIDefinitionBlock* desc =(const ACPIDefinitionBlock*) bufferPos;
            
            err = decomp->callbacks.onDefinitionBlock(decomp,&ctx , desc);
            
        }
            break;
            
        case ACPIObject_Type_Scope:
        {
            //if( exceptState(decomp, AMLState_Unknown))
            {
                setState(decomp, AMLState_StartedScope);
                
                
                //char name[SCOPE_STR_SIZE];
                ACPIScope scope = {0};
                //scope.name = name;
                
                //AMLName scopeName = {0};
                const ssize_t advanced = AMLNameCreateFromBuffer(&scope._name, bufferPos, bufferSize);
                
                //size_t __advanced = ResolvePath(name,  bufferPos);
                //assert(__advanced == advanced);
                decomp->callbacks.startScope(decomp ,&ctx , &scope);
                
                err =  AMLParserProcessInternalBuffer(parser, bufferPos + advanced, bufferSize-advanced);
                if(err != AMLParserError_None)
                {
                    assert(parser->parserPolicy.assertOnError == 0);
                    setState(decomp, AMLState_Unknown);
                    break;
                }
                
                decomp->callbacks.endScope(decomp ,&ctx , &scope);

                setState(decomp, AMLState_Unknown);
            }
            /*
            else
            {
                err = AMLParserError_InvalidState;
            }
             */

        }
            break;
        
        case ACPIObject_Type_Buffer:
        {
            if (exceptState(decomp, AMLState_WaitingNameValue))
            {
                err =  _DecodeBufferObjects(parser, &ctx, bufferPos, bufferSize);
                setState(decomp, AMLState_Unknown);
            }
            else if (decomp->parserPolicy.pedantic)
            {
                err = AMLParserError_InvalidState;
            }
        }
            break;
            
        case ACPIObject_Type_Device:
        {
            setState(decomp, AMLState_StartedDevice);
            ACPIDevice dev;
            
            const uint8_t nameSize = ExtractNameString(bufferPos, bufferSize, dev.name );
            dev.name[4] = 0;
            
            
            decomp->callbacks.startDevice(decomp ,&ctx , &dev);

            // advance bufferPos to skip the name
            err =  AMLParserProcessInternalBuffer(parser, bufferPos + nameSize, bufferSize-nameSize);
            if(err == AMLParserError_None)
            {
                err = decomp->callbacks.endDevice(decomp ,&ctx , &dev);
            }
            
        }
            break;
            
        case ACPIObject_Type_Name:
        {
            
            char name[5] = {0};
            const uint8_t* namePosition = bufferPos;// + pos + advancedByte;
            const uint8_t nameSize = ExtractNameString(namePosition, 5, name);
            assert(nameSize);
            name[4] = 0;
            
            size_t adv = 0;
            ctx.nextOp =  AMLParserPeekOp(bufferPos + 4, 1, &adv);
            
            if(   ctx.nextOp !=  AML_QWordPrefix
               && ctx.nextOp !=  AML_OneOp
               && ctx.nextOp !=  AML_ZeroOp
               && ctx.nextOp !=  AML_OnesOp
               && ctx.nextOp !=  AML_DWordPrefix
               && ctx.nextOp !=  AML_WordPrefix
               && ctx.nextOp !=  AML_BufferOp
               && ctx.nextOp !=  AML_BytePrefix
               && ctx.nextOp !=  AML_PackageOp
               && ctx.nextOp !=  AML_StringPrefix // really allowed ?
               )
            {
                assert(parser->parserPolicy.assertOnError == 0);
                err = AMLParserError_UnexpectedToken;
                break;
            }
            

            decomp->callbacks.startName(decomp ,&ctx , name);
            
            setState(decomp, AMLState_WaitingNameValue);

        }
            break;
            
        case ACPIObject_Type_NumericValue:
        {
            
            uint64_t w;
            GetInteger(bufferPos,bufferSize, &w);
            
            /*
            if (exceptState(decomp, AMLState_WaitingNameValue))
            {
              */
                decomp->callbacks.onValue(decomp,&ctx , w);

                setState(decomp, AMLState_Unknown);
            /*
            }
            else if (decomp->parserPolicy.pedantic)
            {
                assert(parser->parserPolicy.assertOnError == 0);
                err =  AMLParserError_InvalidState;
            }
             */
            
        }
            break;
            
        case ACPIObject_Type_StringValue:
        {
            
            err = decomp->callbacks.onString(decomp, &ctx , (const char*) bufferPos);
        }
            break;
            
        case ACPIObject_Type_Package:
        {
            const ACPIPackage* thePackage = (const ACPIPackage*) bufferPos;
            return _DecodePackage(parser, &ctx, thePackage);
        }
            break;
        case ACPIObject_Type_PackageValue:
        {
            const ACPIPackageElement *element = (const ACPIPackageElement *) bufferPos;
            
            err = decomp->callbacks.onPackageElement(decomp,&ctx, element);
            
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
            
            const ACPIOperationRegion *reg =  (const ACPIOperationRegion*)  bufferPos;
            
            err = decomp->callbacks.onOperationRegion(decomp ,&ctx , reg);
            
            break;
        }
            break;
        case ACPIObject_Type_IndexField:
        {
            err = _DecodeIndexField(parser, &ctx, bufferPos, bufferSize);
            break;
        }
        case ACPIObject_Type_Field:
        {
            err =  _DecodeField(parser, &ctx, bufferPos, bufferSize);
            break;
        }
//            break;
        case ACPIObject_Type_Method:
        {
            ACPIMethod method = {0};
            
            uint8_t nameSize = ExtractNameString(bufferPos, 4, method.name);
            method.name[4] = 0;
            
            
            const uint8_t methodFlags = bufferPos[nameSize];
            // bit 0-2: ArgCount (0-7)
            // bit 3: SerializeFlag (0 not serialized , 1 serialized
            // bit 4-7: SyncLevel (0x00-0x0f)
            
            method.argCount      = methodFlags & 0b00000111;
            method.serializeFlag = methodFlags & 0b00001000;
            method.syncLevel     = methodFlags & 0b11110000;
            
            nameSize++; // +1 for the methodFlags
            
            assert( bufferSize - nameSize >= 0); // remains some data for the method? Can be 0?
            method.bodySize = bufferSize - nameSize;
            method.bodyDef  = method.bodySize?( bufferPos + nameSize) : NULL;
            
            

            err = decomp->callbacks.onMethod(decomp ,&ctx , &method);

            /*
            if (decomp->callbacks.endMethod)
            {
                decomp->callbacks.endMethod(decomp ,&ctx , &method);
            }
             */
        }
            break;
        case ACPIObject_Type_CreateField:
            err = decomp->callbacks.onCreateField(decomp ,&ctx ,(const ACPICreateFieldBase*) bufferPos);
            break;
        case ACPIObject_Type_Root:

            break;
        default:
            assert(0);
            //printf("Got an element type %i\n" , forObjectType);
            break;
    }
    
    
    if(err != AMLParserError_None)
    {
        assert(parser->parserPolicy.assertOnError == 0);
    }
    
    return err;
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
