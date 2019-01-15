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

static AMLParserError _AMLParserProcessBuffer(AMLParserState* state, const uint8_t* buffer , size_t bufSize , int parseDefBlock);

int AMLParserInit(AMLParserState* state)
{
    memset(state , 0, sizeof(AMLParserState));

    return 1;
}


static int _EnsureValidBuffer(AMLParserState* state , const uint8_t* buffer , size_t bufSize)
{
    
    assert( buffer >= state->startBuffer);
    
    ptrdiff_t p1 = buffer+bufSize;
    ptrdiff_t p2 = state->startBuffer + state->totalSize;
    
    ptrdiff_t diff = p2-p1; // for debug
    (void) diff;
    assert( p1 <= p2);
    
    return 1;
}


static AMLParserError _ParseDefinitionBlock(AMLParserState* state, const uint8_t* buffer , size_t bufSize , size_t* advanced)
{
    assert(_EnsureValidBuffer(state, buffer, bufSize));
    
    if (bufSize <= sizeof(ACPIDefinitionBlock))
    {
        return AMLParserError_BufferTooShort;
    }
    
    ACPIDefinitionBlock defBlk = {0};
    size_t advancedByte = 0;
    AMLOperation op = _GetNextOp( buffer, bufSize, &advancedByte ,  0/*offset*/ );
    
    if (op != AML_Char)
    {
        return AMLParserError_UnexpectedToken;
    }

    size_t pos = 0;
    strncpy(defBlk.tableSignature,(const char*) buffer, 4);
    pos += 4;
    
    
    GetDWord(buffer + pos, &defBlk.tableLength);
    pos += 4;

    defBlk.complianceRevision = buffer[pos];
    pos +=1;
    
    defBlk.tableCheckSum = buffer[pos];
    pos +=1;// CheckSum
    
    memcpy(defBlk.OEMId, buffer + pos, 6);
    pos +=6;

    memcpy(defBlk.tableId, buffer + pos, 8);
    pos +=8;
    
    GetDWord(buffer + pos, &defBlk.OEMRev);
    pos += 4;

    GetDWord(buffer + pos, &defBlk.creatorID);
    pos += 4;
    
    
    // CreatorRevision
    //pos+=4; FIXME : This crashes
     
    // update advanced pointer
    *advanced = pos;

    
    
    if(state->callbacks.AllocateElement(state , ACPIObject_Type_DefinitionBlock  ,(const uint8_t*) &defBlk , sizeof(defBlk)))
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

static AMLParserError _AMLParserProcessBuffer(AMLParserState* state, const uint8_t* buffer , size_t bufSize , int parseDefBlock)
{
    state->maxDepth++;
    
    // 1st step is to try to get a valid DefinitionBlock
    //ACPIDefinitionBlock defBlk = {0};
    
    size_t advancedByte = 0;
    
    if (parseDefBlock)
    {
        _ParseDefinitionBlock(state, buffer, bufSize , &advancedByte);
    }
    size_t pos = advancedByte;
    bufSize -= advancedByte;
    
    state->callbacks.AllocateElement(state, ACPIObject_Type_Root  , buffer+ pos , bufSize);
    
    while (bufSize)
    {
        
        AMLOperation op = _GetNextOp( buffer+pos, bufSize, &advancedByte ,  0/*offset*/ );

        
        
        switch (op)
        {
            case AML_DeviceOpList:
            {
                // the returned _GetPackageLength will contains the size' size, so we need to substract it from the device size,
                // and include the package size in the advancedByte.
                // This could need a refactorization
                size_t deviceSizeLen = 0;
                size_t deviceSize = _GetPackageLength(buffer + pos + advancedByte, bufSize - advancedByte, &deviceSizeLen, 0)  ;
                
                advancedByte += deviceSizeLen;
                deviceSize   -= deviceSizeLen;
                
                const uint8_t* startDevice = buffer + pos + advancedByte;
                assert(startDevice);
                //assert(*sizeVal <= bufSize);
                state->callbacks.AllocateElement(state, ACPIObject_Type_Device , startDevice , deviceSize/*-advancedByte*/);
                
                advancedByte += deviceSize;
            }
                break;
                
            case AML_ScopeOp:
            {
                size_t scopeSizeLen = 0;
                size_t scopeSize = _GetPackageLength(buffer+pos+advancedByte, bufSize-advancedByte, &scopeSizeLen, 0);
                
                advancedByte += scopeSizeLen;
                scopeSize    -= scopeSizeLen;
                
                const uint8_t* startScope = buffer + pos + advancedByte;
                state->callbacks.AllocateElement(state, ACPIObject_Type_Scope , startScope , scopeSize);
                
                advancedByte +=  scopeSize;
            }
                break;
                
            case AML_DWordPrefix:
            {
                const uint8_t* valPosition = buffer + pos + advancedByte;
                
                const size_t valSize = sizeof(ACPIDWord);
                state->callbacks.AllocateElement(state, ACPIObject_Type_DWord , valPosition , valSize);
                
                advancedByte += valSize;
            }
                break;
            case AML_BytePrefix:
            {
                const uint8_t* valPosition = buffer + pos + advancedByte;
                
                advancedByte+= 1;
                
                //printf("Got a byte %x\n" , *valPosition);
            }
                break;
            case AML_WordPrefix:
            {
                advancedByte +=1;
            }
                break;
            case AML_OP_BufferOp:
            {
                const uint8_t* bufferPos = buffer + pos + advancedByte;
                //printf("Got a buffer op\n");
            }
                break;
                
            case AML_NameOp:
            {
                const uint8_t* namePos = buffer + pos + advancedByte;
                //assert(IsName(*namePos)); FIXME : this fails for now 
                state->callbacks.AllocateElement(state, ACPIObject_Type_Name ,namePos , 4 );
                
                advancedByte +=4;
            }
                break;
                
            case AML_FieldOp:
            {
                size_t fieldSizeLen = 0;
                size_t fieldSize = _GetPackageLength(buffer+pos+advancedByte, bufSize-advancedByte, &fieldSizeLen, 0);
                
                advancedByte += fieldSizeLen;
                fieldSize    -= fieldSizeLen;
                
                uint8_t* fieldPos = buffer + pos + advancedByte;
                /*
                 // DualNamePrefix '.'
                 // ParentPrefixChar '^'
                 if (*namePos ==  AML_OP_ParentPrefixChar)
                 {
                 namePos++;
                 fieldSize -= 1;
                 advancedByte += 1;
                 }
                 
                 if (*namePos == AML_OP_DualNamePrefix)
                 {
                 namePos++;
                 fieldSize -= 1;
                 advancedByte += 1;
                 }
                 
                 
                 
                 assert(IsName(*namePos));
                 
                 char name[4] = {0};
                 ExtractName(namePos, 4, name);
                 printf("Field name '%s' Package size %zi\n" , name , fieldSize);
                 */
                state->callbacks.AllocateElement(state, ACPIObject_Type_Field ,fieldPos , fieldSize );
                
                
                advancedByte += fieldSize;
            }
                break;
                
            case AML_MethodOp:
            {
                size_t methodSizeLen = 0;
                size_t methodSize = _GetPackageLength(buffer+pos+advancedByte, bufSize-advancedByte, &methodSizeLen, 0);
                
                advancedByte += methodSizeLen;
                methodSize   -= methodSizeLen;
                
                
                const uint8_t* startPos = buffer + pos + advancedByte;
                /*
                assert(IsName(*startPos));
                char name[4] = {0};
                ExtractName(startPos, 4, name);
                printf("Method name '%.4s' size %zi\n" , name , methodSize);
                */
                state->callbacks.AllocateElement(state , ACPIObject_Type_Method , startPos , methodSize);
                
                advancedByte += methodSize;
            }
                break;
                
            case AML_OpRegionOp:
            {
                const uint8_t* namePos = buffer + pos + advancedByte;
                
                ACPIOperationRegion reg;
                assert(IsName(*namePos));
                
                ExtractName(namePos, 4, reg.name);
                //printf("OpRegion name '%.4s'\n" , reg.name);
                advancedByte +=4;
                
                /*
                 uint64_t regSpaceVal = 0;
                 uint64_t regOffsetVal = 0;
                 uint64_t regLenVal = 0;
                 */
                const uint8_t* regSpace = buffer + pos + advancedByte;
                advancedByte += GetInteger(regSpace, &reg.space);
                
                const uint8_t* regOffset = buffer + pos + advancedByte;
                advancedByte += GetInteger(regOffset, &reg.offset);
                
                const uint8_t* regLen = buffer + pos + advancedByte;
                advancedByte += GetInteger(regLen, &reg.length);
                
                
                
                state->callbacks.AllocateElement(state, ACPIObject_Type_OperationRegion , (const uint8_t*)&reg , sizeof(ACPIOperationRegion) );
            }
                break;
                
            case AML_VarPackageOp:
            {
                size_t varSizeLen = 0;
                size_t varSize = _GetPackageLength(buffer+pos+advancedByte, bufSize-advancedByte, &varSizeLen, 0);
                
                advancedByte += varSizeLen;
                assert(varSizeLen == 1);// For now this only works for 1 byte varPackageOp
                varSize   -= varSizeLen;
                
                
                const uint8_t* startPos = buffer + pos + advancedByte;
                const uint8_t VarNumElements = *startPos;
                
                /*
                printf("NumElements %i pack len %zi\n" , *startPos,varSize);
                
                for(int i=0;i<varSize;i++)
                {
                    printf("0x%x\n" , startPos[1+i]);
                }
                */
                state->callbacks.AllocateElement(state, ACPIObject_Type_VarPackage , startPos , varSize );
                
                advancedByte += 2;//varSize;
            }
                break;
            
            // cases we don't care for now
            case AML_OneOp:
            case AML_NotOp:
            case AML_NameChar:
            case AML_Char:
            case AML_Int:
            case AML_ZeroOp:
            case AML_Unknown:
                break;
                
            default:
            {
                /*
                const uint8_t* currentPos = buffer + pos + advancedByte;
                const uint8_t* next = currentPos+1;
                const uint8_t* next2= currentPos+2;
                */
            }
                break;
        }

        
        

        // Check if wrapped
        if( (ssize_t)bufSize - (ssize_t)advancedByte <= 0)
        {
            break;
        }
        
        bufSize -= advancedByte;
        pos     += advancedByte;
        
        assert(_EnsureValidBuffer(state, buffer+pos, bufSize));
        
    } // end while
    
    
    
    return 0;
}

/*
static AMLParserError _ParseDeviceList(AMLParserState* state , const uint8_t* buffer , size_t bufSize)
{
    assert(0);
    assert(_EnsureValidBuffer(state, buffer, bufSize));
    
    printf("---- ParseDeviceList Size %zi\n" , bufSize);
    ACPIDevice dev = {0};
    dev.id[4] = 0;
    int idIndex = 0;
    int nameindex = 0;
    ACPIName* currentName = &dev.names[0];
    
    size_t advancedByte = 0;
    size_t pos = 0;
    
    while (bufSize)
    {
        AMLOperation op = _GetNextOp( buffer+pos, bufSize, &advancedByte ,  0 );
        
        if (op == AML_Char || op == AML_Int)
        {
            if (idIndex <4)
            {
                dev.id[idIndex] = (char)buffer[pos];
                idIndex++;
            }
        }

         else if (op == AML_NameOp)
         {
         idIndex = 10; //
         }
 
        else if (op == AML_DWordPrefix)
        {
            const uint8_t* ptr = (const uint8_t*) buffer+pos+advancedByte;
            
            GetDWord(ptr, &currentName->value.word);
            
            printf("Got DWordPrefix %x\n" , currentName->value.word);
            
            
            advancedByte += 4;
            nameindex++;
            currentName = &dev.names[nameindex];
            
        }
        else if (op == AML_StringPrefix)
        {
            idIndex = 100;// this stops the device name parsing
        }
        else if (op == AML_ParentPrefixChar)
        {
            printf("AML_ParentPrefixChar\n");
        }
        else if (op == AML_NameOp)
        {
            const uint8_t* namePosition = buffer + pos + advancedByte;
            
            strncpy(currentName->id, (const char*)namePosition, 4);
            
            if( strncmp( (const char*)namePosition, "_HID", 4) == 0)
            {
                printf("Got a nameOP _HID at %zi\n",  pos+ advancedByte );
                advancedByte += 4;
            }
            else if( strncmp( (const char*)namePosition, "_ADR", 4) == 0)
            {
                printf("Got a nameOP _ADR at %zi\n",  pos+ advancedByte );
                advancedByte += 4;
                const uint8_t* test = buffer + pos + advancedByte;
                _EnsureValidBuffer(state, test, 1);
                //printf("At pos %x %x %x %x\n" , test[0],test[1],test[2],test[3]);
                
            }
            else if( strncmp( (const char*)namePosition, "_CRS", 4) == 0)
            {
                printf("Got a nameOP _CRS at %zi\n",  pos+ advancedByte );
                advancedByte += 4;
                const uint8_t* newPos = buffer + pos + advancedByte;
                printf("%x %x %x %x\n" , newPos[0] , newPos[1],newPos[2],newPos[3]);
            }
            else if( strncmp( (const char*)namePosition, "_PRW", 4) == 0)
            {
                printf("Got a nameOP _PRW at %zi\n",  pos+ advancedByte );
                advancedByte += 4;
            }
            else if( strncmp( (const char*)namePosition, "_BBN", 4) == 0)
            {
                printf("Got a nameOP _BBN at %zi\n",  pos+ advancedByte );
                advancedByte += 4;
            }
            else if( strncmp( (const char*)namePosition, "_UID", 4) == 0)
            {
                printf("Got a nameOP _UID at %zi\n",  pos+ advancedByte );
                advancedByte += 4;
            }
            else if( strncmp( (const char*)namePosition, "_PCL", 4) == 0)
            {
                printf("Got a nameOP _PCL at %zi\n",  pos+ advancedByte );
                advancedByte += 4;
            }
            
            else
            {
                printf("Got a nameOP unknown ('%s') at %zi\n",namePosition,  pos+ advancedByte );
                //assert(0);
            }
            
        }
        else if (op == AML_BufferOp)
        {
            advancedByte +=1;
            uint8_t bufferSize = buffer[pos+advancedByte];
            
            printf("BufferOP %i\n" , bufferSize);
            
            currentName->value.word = 0;// val.v;
            nameindex++;
            currentName = &dev.names[nameindex];
            
            advancedByte += bufferSize;
        }
        else
        {
            printf("Other op %i\n" , op);
        }
        
        // Check if wrapped
        if( (ssize_t)bufSize - (ssize_t)advancedByte <= 0)
        {
            break;
        }
        
        bufSize -= advancedByte;
        pos     += advancedByte;
    }
    printf("----End device list\n");
    
    return state->callbacks.DidReadObject( state  ,&dev  );
}
*/


/*
 else if (op == AML_Char)
 {
 }
 else if (op == AML_Int)
 {
 }
 else //if (op == AML_Unknown)
 {
 //printf("Got Unknown 0x%02x %i\n" ,buffer[pos],buffer[pos] );
 }
 
 else if (op == AML_NameOp)
 {
 const uint8_t* namePosition = buffer + pos + advancedByte;
 
 if( strncmp( (const char*)namePosition, "_HID", 4) == 0)
 {
 printf("Got a nameOP _HID at %zi\n",  pos+ advancedByte );
 advancedByte += 4;
 }
 else if( strncmp( (const char*)namePosition, "_ADR", 4) == 0)
 {
 printf("Got a nameOP _ADR at %zi\n",  pos+ advancedByte );
 advancedByte += 4;
 }
 else if( strncmp( (const char*)namePosition, "_CRS", 4) == 0)
 {
 printf("Got a nameOP _CRS at %zi\n",  pos+ advancedByte );
 advancedByte += 4;
 }
 else if( strncmp( (const char*)namePosition, "_PRW", 4) == 0)
 {
 printf("Got a nameOP _PRW at %zi\n",  pos+ advancedByte );
 advancedByte += 4;
 }
 else
 {
 assert(0);
 }
 }
 
 else if (op == AML_NameChar)
 {
 const uint8_t* namePosition = buffer + pos + advancedByte;
 printf("Got NameChar '%s'\n" ,namePosition);
 
 }
 else if (op == AML_Unknown)
 {
 printf("Got Unknown 0x%02x %i\n" ,buffer[pos],buffer[pos] );
 }
 else if (op == AML_TypeOp)
 {
 printf("TypeOp\n");
 }
 else if (op == AML_PackageOp)
 {
 printf("PackageOp\n");
 }
 else if (op == AML_VarPackageOp)
 {
 printf("VarPackageOp\n");
 }
 else if (op == AML_Char)
 {
 printf("Got Char '%c'\n" ,(char)buffer[pos] );
 }
 else if (op == AML_OneOp)
 {
 printf("1op\n");
 }
 else if (op == AML_ZeroOp)
 {
 printf("0op\n");
 }
 else if (op == AML_Int)
 {
 printf("Int '%c'\n" , (char)buffer[pos]);
 }
 else
 {
 printf("Other op %i (%x)\n",op , (char)buffer[pos]);
 }
 */





/*
 static int _StartNewObject(AMLParserState* state, ACPIObject_Type type)
 {
 int lastIndex = state->currentObjIndex;
 state->currentObjIndex++;
 state->state = AML_State_StartObject;
 
 ACPIObjectInit(&state->obj[state->currentObjIndex]);
 state->obj[state->currentObjIndex].indexScope = state->currentObjIndex;
 state->obj[state->currentObjIndex].indexParent = lastIndex;
 state->obj[state->currentObjIndex].type = type;
 return 1;
 }
 
 static int _EndScope(AMLParserState* state)
 {
 return 1;
 }
 */

//static int AMLParserProcessDWord(AMLParserState* state , uint8_t byte);
/*
 static int AMLParserProcessByte3(AMLParserState* state , uint8_t byte)
 {
 int ret = 1;
 
 switch (byte)
 {
 case AML_OPCode_LNOT_Complement_LNotEqualOp:
 printf("Not Equal]");
 break;
 
 case AML_OPCode_LNOT_Complement_LLessEqualOp:
 printf("Less Equal]");
 break;
 
 case AML_OPCode_LNOT_Complement_LGreaterEqualOp:
 printf("Greater Equal]");
 break;
 
 default:
 break;
 }
 
 state->state = AML_State_Ready;
 return ret;
 }
 */
/*
 static int AMLParserProcessByte2(AMLParserState* state , uint8_t byte)
 {
 int ret = 1;
 
 switch (byte)
 {
 case AML_OPCode_Complement_MutexOp:
 printf("MutexOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_EventOp:
 printf("EventOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_CondRefOfOp:
 printf("CondRefOfOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_CreateFieldOp:
 printf("CreateFieldOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_LoadTableOp:
 printf("LoadTableOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_LoadOp:
 printf("LoadOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_StallOp:
 printf("StallOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_SleepOp:
 printf("SleepOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_AcquireOp:
 printf("AcquireOp]");
 break;
 
 case AML_OPCode_Complement_SignalOp:
 printf("SignalOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_WaitOp:
 printf("WaitOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_ResetOp:
 printf("ResetOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_ReleaseOp:
 printf("ReleaseOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_FromBCDOp:
 printf("FromBCDOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_ToBCD:
 printf("ToBCD]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_UnloadOp:
 printf("UnloadOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_RevisionOp:
 printf("RevisionOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_DebugOp:
 printf("DebugOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_FatalOp:
 printf("FatalOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_TimerOp:
 printf("TimerOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_OpRegionOp:
 printf("OpRegionOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_FieldOp:
 printf("FieldOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_ProcessorOp:
 printf("ProcessorOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_PowerResOp:
 printf("PowerResOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_ThermalZoneOpList:
 printf("ThermalZoneOpList]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_IndexFieldOp:
 printf("IndexFieldOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_BankFieldOp:
 printf("BankFieldOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_DataRegionOp:
 printf("DataRegionOp]");
 state->state = AML_State_Ready;
 break;
 
 case AML_OPCode_Complement_DeviceOpList:
 printf("DeviceOpList]");
 _StartNewObject(state, ACPIObject_Type_Device);
 
 break;
 
 default:
 assert(0);
 ret = 0;
 break;
 }
 
 
 
 return ret;
 }
 */
/*
 static int AMLParserProcessByte1(AMLParserState* state , uint8_t byte)
 {
 int ret = 1;
 switch (byte)
 {
 case AML_OP_ZeroOp:
 break;
 
 
 case AML_OP_OneOp:
 printf("[OneOP]\n ");
 break;
 
 case AML_OP_ScopeOp:
 printf("[Scope Start]\n ");
 _StartNewObject(state, ACPIObject_Type_Scope);
 
 break;
 
 case AML_OP_StringPrefix:
 printf("[String Start]");
 break;
 
 case AML_OP_NameOp:
 printf("[NameOP]");
 _StartNewObject(state, ACPIObject_Type_Name);
 break;
 case AML_OP_RootChar:
 printf("[RootChar]");
 break;
 
 case AML_OP_NameChar:
 printf("[NameChar]");
 break;
 
 case AML_OP_WordPrefix:
 printf("[WordPrefix]");
 break;
 
 
 case AML_OP_ExtOpPrefix:
 printf("[Ext:");
 state->state = AML_State_Wait2ndOP;
 break;
 
 case AML_OP_LnotOp:
 printf("[Lnot:");
 state->state = AML_State_Wait2ndLnotOp;
 break;
 
 case AML_OP_LorOp:
 printf("[LorOp]");
 break;
 
 case AML_OP_AliasOp:
 printf("[AliasOp]");
 break;
 
 case AML_OP_BytePrefix:
 printf("[BytePrefix]");
 break;
 
 case AML_OP_DWordPrefix:
 printf("[DWordPrefix]");
 state->state = AML_State_WaitDword0;
 break;
 
 case AML_OP_QWordPrefix:
 printf("[QWordPrefix]");
 break;
 
 case AML_OP_BufferOp:
 printf("[BufferOp]");
 break;
 case AML_OP_PackageOp:
 printf("[PackageOp]");
 break;
 case AML_OP_VarPackageOp:
 printf("[VarPackageOp]");
 break;
 case AML_OP_MethodOp:
 printf("[MethodOp]");
 break;
 case AML_OP_DualNamePrefix:
 printf("[DualNamePrefix]");
 break;
 case AML_OP_MultiNamePrefix:
 printf("[MultiNamePrefix]");
 break;
 
 case AML_OP_ParentPrefixChar:
 printf("[ParentPrefixChar]");
 break;
 
 case AML_OP_Local0Op:
 printf("[Local0Op]");
 break;
 case AML_OP_Local1Op:
 printf("[Local1Op]");
 break;
 case AML_OP_Local2Op:
 printf("[Local2Op]");
 break;
 case AML_OP_Local3Op:
 printf("[Local3Op]");
 break;
 case AML_OP_Local4Op:
 printf("[Local4Op]");
 break;
 case AML_OP_Local5Op:
 printf("[Local5Op]");
 break;
 case AML_OP_Local6Op:
 printf("[Local6Op]");
 break;
 case AML_OP_Local7Op:
 printf("[Local7Op]");
 break;
 case AML_OP_Arg0Op:
 printf("[Arg0Op]");
 break;
 case AML_OP_Arg1Op:
 printf("[Arg1Op]");
 break;
 case AML_OP_Arg2Op:
 printf("[Arg2Op]");
 break;
 case AML_OP_Arg3Op:
 printf("[Arg3Op]");
 break;
 case AML_OP_Arg4Op:
 printf("[Arg4Op]");
 break;
 case AML_OP_Arg5Op:
 printf("[Arg5Op]");
 break;
 case AML_OP_Arg6Op:
 printf("[Arg6Op]");
 break;
 case AML_OP_StoreOp:
 printf("[StoreOp]");
 break;
 case AML_OP_RefOfOp:
 printf("[RefOfOp]");
 break;
 case AML_OP_AddOp:
 printf("[AddOp]");
 break;
 case AML_OP_ConcatOp:
 printf("[ConcatOp]");
 break;
 case AML_OP_SubtractOp:
 printf("[SubtractOp]");
 break;
 case AML_OP_IncrementOp:
 printf("[IncrementOp]");
 break;
 case AML_OP_DecrementOp:
 printf("[DecrementOp]");
 break;
 case AML_OP_MultiplyOp:
 printf("[MultiplyOp]");
 break;
 case AML_OP_DivideOp:
 printf("[DivideOp]");
 break;
 case AML_OP_ShiftLeftOp:
 printf("[ShiftLeftOp]");
 break;
 case AML_OP_ShiftRightOp:
 printf("[ShiftRightOp]");
 break;
 case AML_OP_AndOp:
 printf("[AndOp]");
 break;
 case AML_OP_NandOp:
 printf("[NAndOp]");
 break;
 case AML_OP_OrOp:
 printf("[Or]");
 break;
 case AML_OP_NorOp:
 printf("[Nor]");
 break;
 case AML_OP_XorOp:
 printf("[Xor]");
 break;
 case AML_OP_NotOp:
 printf("[Not]");
 break;
 case AML_OP_FindSetLeftBitOp:
 printf("[FindSetLeftBitOp]");
 break;
 case AML_OP_FindSetRightBitOp:
 printf("[FindSetRightBitOp]");
 break;
 case AML_OP_DerefOfOp:
 printf("[DerefOfOp]");
 break;
 case AML_OP_ConcatResOp:
 printf("[ConcatResOp]");
 break;
 case AML_OP_ModOp:
 printf("[ModOp]");
 break;
 case AML_OP_NotifyOp:
 printf("[NotifyOp]");
 break;
 case AML_OP_SizeOfOp:
 printf("[SizeOfOp]");
 break;
 case AML_OP_IndexOp:
 printf("[IndexOp]");
 break;
 case AML_OP_MatchOp:
 printf("[MatchOp]");
 break;
 case AML_OP_CreateDWordFieldOp:
 printf("[CreateDWordFieldOp]");
 break;
 case AML_OP_CreateWordFieldOp:
 printf("[CreateWordFieldOp]");
 break;
 case AML_OP_CreateByteFieldOp:
 printf("[CreateByteFieldOp]");
 break;
 case AML_OP_CreateBitFieldOp:
 printf("[CreateBitFieldOp]");
 break;
 case AML_OP_TypeOp:
 printf("[TypeOp]");
 break;
 case AML_OP_CreateQWordFieldOp:
 printf("[CreateQWordFieldOp]");
 break;
 
 case AML_OP_LandOp:
 printf("[LandOp]");
 break;
 
 case AML_OP_LEqualOp:
 printf("[LEqualOp]");
 break;
 case AML_OP_LGreaterOp:
 printf("[LGreaterOp]");
 break;
 case AML_OP_LLessOp:
 printf("[LLessOp]");
 break;
 case AML_OP_ToBufferOp:
 printf("[ToBufferOp]");
 break;
 case AML_OP_ToDecimalStringOp:
 printf("[ToDecimalStringOp]");
 break;
 case AML_OP_ToHexStringOp:
 printf("[ToHexStringOp]");
 break;
 case AML_OP_ToIntegerOp:
 printf("[ToIntegerOp]");
 break;
 case AML_OP_ToStringOp:
 printf("[ToStringOp]");
 break;
 case AML_OP_CopyObjectOp:
 printf("[CopyObjectOp]");
 break;
 case AML_OP_MidOp:
 printf("[MidOp]");
 break;
 case AML_OP_ContinueOp:
 printf("[ContinueOp]");
 break;
 case AML_OP_IfOp:
 printf("[If]");
 break;
 case AML_OP_ElseOp:
 printf("[Else]");
 break;
 case AML_OP_WhileOp:
 printf("[While]");
 break;
 case AML_OP_NoopOp:
 printf("[Noop]");
 break;
 case AML_OP_ReturnOp:
 printf("[Return]");
 break;
 case AML_OP_BreakOp:
 printf("[Break]");
 break;
 case AML_OP_BreakPointOp:
 printf("[BreakPoint]");
 break;
 case AML_OP_OnesOp:
 printf("[Ones]");
 break;
 
 
 
 
 default:
 
 if (byte>=0x41 && byte<=0x5A)
 {
 //printf("[C:%c]" , byte);
 state->state = AML_State_GetString;
 ret = 1;
 }
 else if (byte>=0x30 && byte<=0x39)
 {
 printf("[I:%c]" , byte);
 ret = 1;
 }
 else
 {
 ret = 0;
 }
 
 break;
 }
 return ret;
 }
 */

/*
 int AMLParserProcessByte(AMLParserState* state , const uint8_t *byte)
 {
 
 switch (state->state)
 {
 case AML_State_StartObject:
 //printf("Start object at %p\n" , byte);
 state->obj[state->currentObjIndex].start = byte;
 //state->obj[state->currentObjIndex].type = ACPIObject_Type_Device;
 state->state = AML_State_Ready;
 return 1;
 break;
 
 case AML_State_EndObject:
 _EndScope(state);
 state->state = AML_State_Ready;
 return 1;
 break;
 
 
 case AML_State_Ready:
 return AMLParserProcessByte1(state, *byte);
 
 
 case AML_State_Wait2ndOP:
 return AMLParserProcessByte2(state, *byte);
 
 case AML_State_Wait2ndLnotOp:
 return AMLParserProcessByte3(state, *byte);
 
 case AML_State_GetString:
 
 return 1;
 case AML_State_WaitDword0:
 case AML_State_WaitDword1:
 case AML_State_WaitDword2:
 case AML_State_WaitDword3:
 return AMLParserProcessDWord(state, *byte);
 default:
 break;
 }
 
 assert(0);
 return 0;
 }
 */
/*
 static int AMLParserProcessDWord(AMLParserState* state , uint8_t byte)
 {
 int ret = 1;
 
 switch (state->state)
 {
 case AML_State_WaitDword0:
 state->obj[state->currentObjIndex].basicVal.bytes[0] = byte;
 state->state++;
 break;
 
 case AML_State_WaitDword1:
 state->obj[state->currentObjIndex].basicVal.bytes[1] = byte;
 state->state++;
 break;
 
 case AML_State_WaitDword2:
 state->obj[state->currentObjIndex].basicVal.bytes[2] = byte;
 state->state++;
 break;
 
 case AML_State_WaitDword3:
 state->obj[state->currentObjIndex].basicVal.bytes[3] = byte;
 state->obj[state->currentObjIndex].basicType = BasicValType_DWord;
 state->state = AML_State_EndObject;
 
 printf("\n[End DWord for object %i value 0x%x]\n" , state->currentObjIndex , state->obj[state->currentObjIndex].basicVal.DWord);
 break;
 
 default:
 assert(0);
 break;
 }
 
 return ret;
 }
 */
