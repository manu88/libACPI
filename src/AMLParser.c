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

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "AMLParser.h"
#include "AMLHelpers.h"
#include "AMLRouter.h"
#include "AMLByteCode.h"
#include "AMLTypes.h"

static AMLParserError _AMLParserProcessBuffer(AMLParserState* state, const uint8_t* buffer , size_t bufSize , int parseDefBlock);

int AMLParserInit(AMLParserState* state)
{
    memset(state , 0, sizeof(AMLParserState));

    
    return 1;
}


static int _EnsureValidBuffer(AMLParserState* state , const uint8_t* buffer , size_t bufSize)
{
    if (buffer < state->startBuffer)
    {
        state->errorPos = buffer;
        assert( state->parserPolicy.assertOnError == 0);
        return 0;
    }
    ptrdiff_t p1 = (ptrdiff_t) buffer+bufSize;
    ptrdiff_t p2 = (ptrdiff_t) state->startBuffer + state->totalSize;
    
    ptrdiff_t diff = p2-p1; // for debug
    (void) diff;
    
    if (p1 > p2)
    {
        state->errorPos = buffer;
        assert( state->parserPolicy.assertOnError == 0);
        return 0;
    }
    //assert( p1 <= p2);
    
    return 1;
}


static AMLParserError _ParseDefinitionBlock(AMLParserState* state, const uint8_t* buffer , size_t bufSize , size_t* advanced)
{
    assert(_EnsureValidBuffer(state, buffer, bufSize));
    
    if (bufSize <= sizeof(ACPIDefinitionBlock))
    {
        state->errorPos = buffer;
        return AMLParserError_BufferTooShort;
    }
    
    
    
    size_t advancedByte = 0;
    AMLOperation op = _GetNextOp( buffer, bufSize, &advancedByte ,  0/*offset*/ );
    
    if (op != AML_Char)
    {
        state->errorPos = buffer;
        //assert(state->parserPolicy.assertOnError == 0);
        return AMLParserError_UnexpectedToken;
    }

    *advanced += sizeof(ACPIDefinitionBlock);
    const ACPIDefinitionBlock* defBlk= (const ACPIDefinitionBlock*) buffer;

    if( defBlk->tableLength > bufSize)
    {
        return AMLParserError_BufferTooShort;
    }
    
    if(   defBlk->tableSignature[0] == 0
       || defBlk->tableSignature[1] == 0
       || defBlk->tableSignature[2] == 0
       || defBlk->tableSignature[3] == 0
       )
    {
        return AMLParserError_InvalidDefinitionBlock;
    }

    
    if(state->callbacks.OnElement(state , ACPIObject_Type_DefinitionBlock  ,(const uint8_t*) defBlk , sizeof(defBlk)))
    {
        return AMLParserError_None;
    }
    return AMLParserUserAbord;
                                            //DidReadDefBlock(state , &defBlk);
}

AMLOperation AMLParserPeekOp( const uint8_t* buffer , size_t bufSize ,size_t *advance)
{
    return _GetNextOp(buffer, bufSize, advance, 0);
}

AMLParserError AMLParserProcessBuffer(AMLParserState* state, const uint8_t* buffer , size_t bufSize)
{
    return _AMLParserProcessBuffer(state, buffer, bufSize, 1);
}

AMLParserError AMLParserProcessInternalBuffer(AMLParserState* state, const uint8_t* buffer , size_t bufSize)
{
    return _AMLParserProcessBuffer(state, buffer, bufSize, 0);
}

static AMLParserError _AMLParserProcessPackageElement(AMLParserState* state,AMLOperation op,const ACPIPackage* fromPackage, const uint8_t* buffer , size_t bufSize , size_t* advancedBy)
{
    
    ACPIPackageElement element= {0};
    element.packageRef = fromPackage;
    
    switch (op)
    {
        case AML_PackageOp:
        {
            size_t adv = 0;
            const size_t packLen = _GetPackageLength(buffer, bufSize, &adv, 0);
            
            const uint8_t numElements = buffer[adv];
            *advancedBy +=packLen;
            
            const size_t packSize = packLen - adv - 1;
            const uint8_t* startPack = buffer + adv + 1;
            
            ACPIPackage pack = {0};
            pack.packageRef = fromPackage;
            pack.buffer = startPack;
            pack.bufSize = packSize;
            pack.numElements = numElements;
            
            AMLParserError err = state->callbacks.OnElement(state, ACPIObject_Type_Package ,(const uint8_t*) &pack , sizeof(ACPIPackage));
            if (err != AMLParserError_None)
            {
                assert(state->parserPolicy.assertOnError == 0);
                return err;
            }
        }break;
        case AML_BytePrefix:
        case AML_ZeroOp:
        case AML_OneOp:
        case AML_OnesOp:
        case AML_WordPrefix:
        case AML_DWordPrefix:
        {
            const uint8_t* valPosition = buffer -1; // we step back one byte
            
            size_t valSize = getIntegerSizeFromOpCode(op);
            
            
            
            element.type = Integer;
            assert( GetInteger(valPosition, bufSize+1, &element.value.value) == valSize+1);
            
            AMLParserError err = state->callbacks.OnElement(state,
                                                            ACPIObject_Type_PackageValue ,
                                                            (const uint8_t *)&element , sizeof(ACPIPackageElement)
                                                            );
            if (err != AMLParserError_None)
            {
                assert(state->parserPolicy.assertOnError == 0);
                return err;
            }
            
            *advancedBy += valSize;
        }
            break;
        case AML_Char:
        {
            const uint8_t* valPosition = buffer -1; // we step back one byte
            
            char name[5] = {0};
            uint8_t nameSize = 0;
            ExtractName(valPosition, 4, name, &nameSize);
            element.type = String;
            element.value.string = name;
            assert(nameSize > 0);
            
            AMLParserError err = state->callbacks.OnElement(state,
                                                            ACPIObject_Type_PackageValue ,
                                                            (const uint8_t *)&element , sizeof(ACPIPackageElement)
                                                            );
            if (err != AMLParserError_None)
            {
                assert(state->parserPolicy.assertOnError == 0);
                return err;
            }
            
            *advancedBy +=nameSize -1;
            
        }
            break;
        case AML_StringPrefix:
        {
            const char* valPosition = (const char*) buffer;
            
            const size_t valSize = strlen( (const char*)valPosition) + 1; // +1 for null byte

            *advancedBy += valSize;

        }break;
       
        /*
         • Integer Constant
         • String Constant
         • Buffer Constant
         • Package Constant
         */
        default:
            if (IsRealName(buffer[0]))
            {
                
                char name[5] = {0};
                
                size_t nameSize = bufSize;
                assert(nameSize <= 5);
                assert(nameSize > 0);
                strncpy(name,(const char*) buffer , nameSize);
                
                *advancedBy = bufSize;
                
                return AMLParserError_None;
                
            }
            else
            {
                assert(0);
            }
            
            return AMLParserError_UnexpectedOp;
    }
    return AMLParserError_None;
}

static AMLParserError _AMLParserProcessOperation(AMLParserState* state,AMLOperation op, const uint8_t* buffer , size_t bufSize , size_t* advancedBy)
{
    switch (op)
    {
        case AML_VarPackageOp:
        {
            size_t varSizeLen = 0;
            size_t varSize = GetPackageLength(buffer, bufSize, &varSizeLen);
            
            *advancedBy += varSizeLen;
            assert(varSizeLen == 1);// For now this only works for 1 byte varPackageOp
            varSize   -= varSizeLen;
            
            const uint8_t* startPos = buffer  + *advancedBy;
            //const uint8_t VarNumElements = *startPos;
            
            AMLParserError err = state->callbacks.OnElement(state, ACPIObject_Type_VarPackage , startPos , varSize );
            if (err != AMLParserError_None)
            {
                assert(state->parserPolicy.assertOnError == 0);
                return err;
            }
            *advancedBy += 2;//varSize;
        }
            break;
            
        case AML_NameOp:
        {
            const uint8_t* namePos = buffer;
            
            if (!IsName(*namePos))
            {
                return AMLParserError_UnexpectedToken;
            }
            
            AMLParserError err = state->callbacks.OnElement(state, ACPIObject_Type_Name ,namePos , 4 );
            if (err != AMLParserError_None)
            {
                assert(state->parserPolicy.assertOnError == 0);
                return err;
            }
            *advancedBy +=4;
            
        }
            break;
        case AML_NameChar:
            
            break;
        case AML_DeviceOpList:
        {
            size_t deviceSizeLen = 0;
            size_t deviceSize = GetPackageLength(buffer,bufSize, &deviceSizeLen)  ;
            //size_t deviceSizeLen2 = 0;
            //size_t deviceSize2 = _GetPackageLength(buffer,bufSize, &deviceSizeLen2 , 0)  ;
            
            //assert(deviceSize == deviceSize2);
            //assert(deviceSizeLen == deviceSizeLen2);
            
            *advancedBy += deviceSizeLen;
            deviceSize   -= deviceSizeLen;
            
            const uint8_t* startDevice = buffer + *advancedBy;
            assert(startDevice);
            //assert(*sizeVal <= bufSize);
            
            AMLParserError err = state->callbacks.OnElement(state, ACPIObject_Type_Device , startDevice , deviceSize/*-advancedByte*/);
            if (err != AMLParserError_None)
            {
                assert(state->parserPolicy.assertOnError == 0);
                return err;
            }
            *advancedBy += deviceSize;
        }
            break;
            
        case AML_ScopeOp:
        {
            size_t scopeSizeLen = 0;
            size_t scopeSize = _GetPackageLength(buffer/*+pos+advancedByte*/, bufSize, &scopeSizeLen, 0);
            
            
            *advancedBy += scopeSizeLen;
            scopeSize    -= scopeSizeLen;
            
            const uint8_t* startScope = buffer +  *advancedBy;
            
            AMLParserError err = state->callbacks.OnElement(state, ACPIObject_Type_Scope , startScope , scopeSize);
            if (err != AMLParserError_None)
            {
                assert(state->parserPolicy.assertOnError == 0);
                return err;
            }
            *advancedBy +=  scopeSize;
        }
            break;
            
        case AML_OpRegionOp:
        {
            const uint8_t* namePos = buffer;

            ACPIOperationRegion reg;
            assert(IsName(*namePos));
            const uint8_t nameSize = ExtractNameString(namePos, 5, reg.name);// GetNameSize(namePos , bufSize);
            assert(nameSize <=5); // ACPIOperationRegion.name is size 6 so we need to check this one
            reg.name[nameSize] = 0;
            reg.name[5] = 0;
            
            *advancedBy +=nameSize;
            
            reg.space = buffer[ *advancedBy ];
            *advancedBy +=1;

            const uint8_t* regOffset = buffer +  *advancedBy;
            *advancedBy += GetInteger(regOffset,bufSize-*advancedBy, &reg.offset);
            
            const uint8_t* regLen = buffer + *advancedBy;
            *advancedBy += GetInteger(regLen,bufSize-*advancedBy, &reg.length);
            
            
            AMLParserError err = state->callbacks.OnElement(state, ACPIObject_Type_OperationRegion , (const uint8_t*)&reg , sizeof(ACPIOperationRegion) );
            if (err != AMLParserError_None)
            {
                assert(state->parserPolicy.assertOnError == 0);
                return err;
            }
            
        }
            break;
            
        case AML_FieldOp:
        {
            size_t fieldSizeLen = 0;
            size_t fieldSize = _GetPackageLength(buffer, bufSize, &fieldSizeLen, 0);
            
            *advancedBy += fieldSizeLen;
            fieldSize    -= fieldSizeLen;
            
            uint8_t* fieldPos = (uint8_t* ) buffer + *advancedBy;
            
            AMLParserError err = state->callbacks.OnElement(state, ACPIObject_Type_Field ,fieldPos , fieldSize );
            if (err != AMLParserError_None)
            {
                assert(state->parserPolicy.assertOnError == 0);
                return err;
            }
            *advancedBy += fieldSize;
        }
            break;
            
            
        case AML_MethodOp:
        {
            size_t methodSizeLen = 0;
            size_t methodSize = _GetPackageLength(buffer, bufSize, &methodSizeLen, 0);
            
            *advancedBy += methodSizeLen;
            methodSize   -= methodSizeLen;
            
            
            const uint8_t* startPos = buffer + *advancedBy;
            
            AMLParserError err = state->callbacks.OnElement(state , ACPIObject_Type_Method , startPos , methodSize);
            if (err != AMLParserError_None)
            {
                assert(state->parserPolicy.assertOnError == 0);
                return err;
            }
            
            *advancedBy += methodSize;
        }
            break;
            
        case AML_BufferOp:
        {
            size_t bufSizeLen = 0;
            size_t bufferSizePackage = _GetPackageLength(buffer, bufSize, &bufSizeLen, 0);
            
            *advancedBy        += bufSizeLen;
            bufferSizePackage   -= bufSizeLen;
            
            const uint8_t* startBuffer = buffer + *advancedBy;
            
            AMLParserError err = state->callbacks.OnElement(state, ACPIObject_Type_Buffer , startBuffer , bufferSizePackage);
            if (err != AMLParserError_None)
            {
                assert(state->parserPolicy.assertOnError == 0);
                return err;
            }
            *advancedBy +=  bufferSizePackage;
        }
            break;
            
            /* Integer types */
            
        case AML_ZeroOp:
        case AML_OneOp:
        case AML_OnesOp:
        {
            const uint8_t* valPosition = buffer-1;
            const size_t valSize = 0;
            

            
            AMLParserError err = state->callbacks.OnElement(state, ACPIObject_Type_NumericValue , valPosition , valSize +1);
            if (err != AMLParserError_None)
            {
                assert(state->parserPolicy.assertOnError == 0);
                return err;
            }
            
            *advancedBy += valSize;
        }
            break;
        case AML_BytePrefix:
        {
            const uint8_t* valPosition = buffer-1;
            
            const size_t valSize = 1;// sizeof(ACPIDWord);
            
            AMLParserError err = state->callbacks.OnElement(state, ACPIObject_Type_NumericValue , valPosition , valSize+1);
            if (err != AMLParserError_None)
            {
                assert(state->parserPolicy.assertOnError == 0);
                return err;
            }
            
            *advancedBy += valSize;
        }
            break;
        case AML_WordPrefix:
        {
            const uint8_t* valPosition = buffer-1;
            
            const size_t valSize = 2;// sizeof(ACPIDWord);
            
            AMLParserError err = state->callbacks.OnElement(state, ACPIObject_Type_NumericValue , valPosition , valSize+1);
            if (err != AMLParserError_None)
            {
                assert(state->parserPolicy.assertOnError == 0);
                return err;
            }
            
            *advancedBy += valSize;
            
        }
            break;
        case AML_DWordPrefix:
        {
            const uint8_t* valPosition = buffer -1; // we step back one byte
            
            const size_t valSize = 4;
            
            AMLParserError err = state->callbacks.OnElement(state, ACPIObject_Type_NumericValue , valPosition , valSize+1);
            if (err != AMLParserError_None)
            {
                assert(state->parserPolicy.assertOnError == 0);
                return err;
            }
            
            *advancedBy += valSize;
        }
            break;
        case AML_QWordPrefix:
        {
            const uint8_t* valPosition = buffer -1; // we step back one byte
            
            const size_t valSize = 8;
            
            AMLParserError err = state->callbacks.OnElement(state, ACPIObject_Type_NumericValue , valPosition , valSize+1);
            if (err != AMLParserError_None)
            {
                assert(state->parserPolicy.assertOnError == 0);
                return err;
            }
            
            *advancedBy += valSize;
        }
            break;
            
            /* END Integer types */
        case AML_StringPrefix:
        {
            const uint8_t* valPosition = buffer;
            
            const size_t valSize = strlen( (const char*)valPosition) + 1; // +1 for null byte
            
            AMLParserError err = state->callbacks.OnElement(state, ACPIObject_Type_StringValue , valPosition , valSize);
            if (err != AMLParserError_None)
            {
                assert(state->parserPolicy.assertOnError == 0);
                return err;
            }
            
            *advancedBy += valSize;

            
        }
            break;
            
        case AML_MutexOp:
        {
            char name[5] = {0};
            
            const uint8_t nameSize =  ExtractNameString(buffer, 4, name);
            
            // bit 0-3: SyncLevel (0x00-0x0f)
            // bit 4-7: Reserved (must be 0)
            const uint8_t syncFlags = buffer[nameSize];
            assert( (syncFlags & 0b1111000) == 0);
            *advancedBy += nameSize +1;
        }
            break;
            
        case AML_Char:
            printf("Char '%c'\n" , *buffer);
            //assert(0);
        case AML_Int:
            break;
        case AML_Unknown:
            break;
            
        case AML_PackageOp:
        {
            size_t adv = 0;
            const size_t packLen = _GetPackageLength(buffer, bufSize, &adv, 0);
            
            const uint8_t numElements = buffer[adv];
            *advancedBy +=packLen;
            
            const size_t packSize = packLen - adv - 1;
            const uint8_t* startPack = buffer + adv + 1;
            
            ACPIPackage pack ={0} ;
            pack.buffer = startPack;
            pack.bufSize = packSize;
            pack.numElements = numElements;
            
            AMLParserError err = state->callbacks.OnElement(state, ACPIObject_Type_Package ,(const uint8_t*) &pack , sizeof(ACPIPackage));
            if (err != AMLParserError_None)
            {
                assert(state->parserPolicy.assertOnError == 0);
                return err;
            }
        }
            break;
            
        case AML_AliasOp:
        {
            char name1[5] = {0};
            char name2[5] = {0};
            const uint8_t name1Size = ExtractNameString(buffer, 4, name1);
            assert(name1[4] == 0);
            const uint8_t name2Size = ExtractNameString(buffer + name1Size, 4, name2);
            assert(name2[4] == 0);
            
            *advancedBy += name1Size + name2Size;
        }
            break;
        
        case AML_CreateFieldOp:
        {
            ACPICreateField fieldOp = {0};
            fieldOp.base.type = CreateField;
            size_t fieldBufSize = bufSize;
            const uint8_t nameSourceSize = ExtractNameString(buffer, 4, fieldOp.base.nameSource);
            assert(fieldOp.base.nameSource[4] == 0); // still null-terminated
            
            fieldBufSize-= nameSourceSize;
            *advancedBy += nameSourceSize;
            
            const uint8_t* readPos = buffer + nameSourceSize;
            
            const size_t byteIndexSize =  GetInteger(readPos, fieldBufSize, &fieldOp.byteIndex);
            
            fieldBufSize-= byteIndexSize;
            *advancedBy += byteIndexSize;
            
            //
            readPos += byteIndexSize;
            //const uint8_t* numBytesPos = buffer + byteIndexSize;
            
            const size_t numBytesSize =  GetInteger(readPos, fieldBufSize, &fieldOp.numBytes);
            
            //fieldBufSize-= numBytesSize;
            *advancedBy += numBytesSize;
            
            //
            readPos+=numBytesSize;
            //const uint8_t* nameStringPos = byteIndexPos +numBytesSize;
            
            const uint8_t nameStringSize = ExtractNameString(readPos, 4, fieldOp.base.nameString);
            assert(fieldOp.base.nameString[4] == 0); // still null-terminated
            
            *advancedBy += nameStringSize;
            
            
            
            AMLParserError err = state->callbacks.OnElement(state, ACPIObject_Type_CreateField , (const uint8_t*)&fieldOp , sizeof(fieldOp));
            if (err != AMLParserError_None)
            {
                assert(state->parserPolicy.assertOnError == 0);
                return err;
            }
        }
            break;
        case AML_CreateWordFieldOp:
        case AML_CreateByteFieldOp:
        {
            
            ACPICreateByteField fieldOp = {0};
            
            switch (op)
            {
                case AML_CreateWordFieldOp:
                    fieldOp.base.type = CreateWordField;
                    break;
                case AML_CreateByteFieldOp:
                    fieldOp.base.type = CreateByteField;
                default:
                    break;
            }
            
            size_t fieldBufSize = bufSize;
            const uint8_t nameSourceSize = ExtractNameString(buffer, 4, fieldOp.base.nameSource);
            assert(fieldOp.base.nameSource[4] == 0); // still null-terminated
            
            fieldBufSize-= nameSourceSize;
            *advancedBy += nameSourceSize;
            
            const uint8_t* byteIndexPos = buffer + nameSourceSize;
            
            const size_t byteIndexSize =  GetInteger(byteIndexPos, fieldBufSize, &fieldOp.byteIndex);
            
            //fieldBufSize-= byteIndexSize;
            *advancedBy += byteIndexSize;
            
            const uint8_t* nameStringPos = byteIndexPos +byteIndexSize;
            
            const uint8_t nameStringSize = ExtractNameString(nameStringPos, 4, fieldOp.base.nameString);
            assert(fieldOp.base.nameString[4] == 0); // still null-terminated
            
            *advancedBy += nameStringSize;
            
            AMLParserError err = state->callbacks.OnElement(state, ACPIObject_Type_CreateField , (const uint8_t*)&fieldOp , sizeof(fieldOp));
            if (err != AMLParserError_None)
            {
                assert(state->parserPolicy.assertOnError == 0);
                return err;
            }
            
        }
            break;
        case AML_IndexFieldOp:
        {
            size_t adv = 0;
            size_t len = _GetPackageLength(buffer, bufSize, &adv, 0);
            
            const uint8_t* startOfIndexField = buffer + adv;
            AMLParserError err = state->callbacks.OnElement(state, ACPIObject_Type_IndexField ,startOfIndexField , len-adv);
            if (err != AMLParserError_None)
            {
                assert(state->parserPolicy.assertOnError == 0);
                return err;
            }
            
            *advancedBy += len;
            
            
            break;
        }
        case AML_ProcessorOp:
        {
            size_t adv = 0;
            size_t len = _GetPackageLength(buffer, bufSize, &adv, 0);
            *advancedBy += len;
        }
            break;
            
        case AML_ElseOp:
        case AML_IfOp:
            //printf(" Got an IF OP to implement \n");
        {
            size_t adv = 0;
            size_t len =GetPackageLength(buffer, bufSize, &adv);
            
            *advancedBy+= len;
        }
            break;
            /* Unimplemented operators*/
            /*
        
            */
            /*
        case AML_DualNamePrefix:
        case AML_RootChar:
        
        case AML_PowerResOp:
            break;
             */
        default:
            printf("OP-code %i not handled: Will assert\n" , op);
            for (int i=0;i<bufSize;i++)
            {
                if (i%8==0)
                    printf("\n");
                
                printf(" 0X%X (%c) " , buffer[i] , buffer[i]);
            }
            printf("\n");
            assert(0);
            
            
    }
            
    return AMLParserError_None;
}


AMLParserError AMLParserProcessPackageContent(AMLParserState* state, const ACPIPackage* fromPackage,const uint8_t* buffer , size_t bufSize)
{
    size_t advancedByte = 0;
    
    
    size_t pos = advancedByte;
    bufSize -= advancedByte;

    while (bufSize)
    {
        AMLOperation op = _GetNextOp( buffer+pos, bufSize, &advancedByte ,  0/*offset*/ );
        
        const uint8_t* startOfOP = buffer +pos+ advancedByte;
        const size_t   operationSize = bufSize - advancedByte;
        
        size_t advancedOf = 0;
        
        AMLParserError retOperation =  _AMLParserProcessPackageElement(state, op, fromPackage, startOfOP, operationSize, &advancedOf);
        
        if (retOperation != AMLParserError_None)
        {
            assert(state->parserPolicy.assertOnError == 0);
            return retOperation;
        }
        advancedByte += advancedOf;
        
        // Check if wrapped
        if( (ssize_t)bufSize - (ssize_t)advancedByte <= 0)
        {
            break;
        }
        
        bufSize -= advancedByte;
        pos     += advancedByte;
        
        if (!_EnsureValidBuffer(state, buffer+pos, bufSize))
        {
            assert( state->parserPolicy.assertOnError == 0);
            return AMLParserError_BufferTooShort;
        }
        
    } // end while
    
    
    
    return AMLParserError_None;
}

static AMLParserError _AMLParserProcessBuffer(AMLParserState* state, const uint8_t* buffer , size_t bufSize , int parseDefBlock)
{
    size_t advancedByte = 0;
    
    if (parseDefBlock)
    {
        AMLParserError errDefBlock =  _ParseDefinitionBlock(state, buffer, bufSize , &advancedByte);
        
    }
    size_t pos = advancedByte;
    bufSize -= advancedByte;
    
    AMLParserError err_ = state->callbacks.OnElement(state, ACPIObject_Type_Root  , buffer+ pos , bufSize);
    if (err_ != AMLParserError_None)
    {
        assert(state->parserPolicy.assertOnError == 0);
        return err_;
    }
    
    while (bufSize)
    {
        AMLOperation op = _GetNextOp( buffer+pos, bufSize, &advancedByte ,  0/*offset*/ );

        const uint8_t* startOfOP = buffer +pos+ advancedByte;
        const size_t   operationSize = bufSize - advancedByte;
        
        size_t advancedOf = 0;

        AMLParserError retOperation =  _AMLParserProcessOperation(state, op, startOfOP, operationSize, &advancedOf);
        
        if (retOperation != AMLParserError_None)
        {
            assert(state->parserPolicy.assertOnError == 0);
            return retOperation;
        }
        advancedByte += advancedOf;
        
        // Check if wrapped
        if( (ssize_t)bufSize - (ssize_t)advancedByte <= 0)
        {
            break;
        }
        
        bufSize -= advancedByte;
        pos     += advancedByte;
        
        if (!_EnsureValidBuffer(state, buffer+pos, bufSize))
        {
            assert( state->parserPolicy.assertOnError == 0);
            return AMLParserError_BufferTooShort;
        }
    
    } // end while
    
    
    
    return AMLParserError_None;
}
