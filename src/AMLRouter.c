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

#include <assert.h>
#include "AMLParser.h"
#include "AMLRouter.h"
#include "AMLByteCode.h"

// borrowed from https://github.com/tadryanom/lux/blob/master/kernel/acpi/eval.c
// acpins_parse_pkgsize(): Parses package size
// Param:    uint8_t *data - pointer to package size data
// Param:    size_t *destination - destination to store package size
// Return:    size_t - size of package size encoding

size_t acpins_parse_pkgsize(const uint8_t *data, size_t *destination )
{
    destination[0] = 0;
    
    uint8_t bytecount = (data[0] >> 6) & 3;
    if(bytecount == 0)
    {
        destination[0] = (size_t)(data[0] & 0x3F);
    }
    else if(bytecount == 1)
    {
        destination[0] = (size_t)(data[0] & 0x0F);
        destination[0] |= (size_t)(data[1] << 4);
    } else if(bytecount == 2)
    {
        destination[0] = (size_t)(data[0] & 0x0F);
        destination[0] |= (size_t)(data[1] << 4);
        destination[0] |= (size_t)(data[2] << 12);
    } else if(bytecount == 3)
    {
        destination[0]   = (size_t)(data[0] & 0x0F);
        destination[0] |= (size_t)(data[1] << 4);
        destination[0] |= (size_t)(data[2] << 12);
        destination[0] |= (size_t)(data[3] << 20);
    }
    
    if (bytecount == 0)
    {
        assert(*destination <=63);
    }
    else
    {
        // Fixme : this fails for now
        assert(*destination > 63);
    }
    // Max size is 2^28 ie 268435456
    assert(*destination < 268435456);
    
    return (size_t)(bytecount + 1);
}

uint8_t GetByteValue(const uint8_t* buffer , size_t bufSize , size_t* advance)
{
    
    assert(*buffer == AML_OP_BytePrefix);
    *advance = 2;
    return buffer[ 1];
}

uint32_t GetDWordValue(const uint8_t* buffer , size_t bufSize , size_t* advance)
{
    assert(*buffer == AML_OP_DWordPrefix);
    
    *advance = 5;
    
    union
    {
        uint8_t b[4];
        uint32_t v;
    } c;
    
    c.b[0] = buffer[1];
    c.b[1] = buffer[2];
    c.b[2] = buffer[3];
    c.b[3] = buffer[4];
    
    return c.v;
}

uint8_t ExtractInteger( const uint8_t* buffer , size_t bufSize , size_t* advance, uint64_t *toVal)
{
    if (*buffer == AML_OP_BytePrefix)
    {
        *toVal = GetByteValue(buffer, bufSize, advance);
        return 1;
    }
    else if (*buffer == AML_OP_DWordPrefix)
    {
        *toVal = GetDWordValue(buffer, bufSize, advance);
        return 4;
    }
    
    assert(0);
    return 0;
}

/*
 18.2.4 Package Length Encoding
 */
size_t _GetPackageLength(const uint8_t* buffer , size_t bufSize , size_t* advance ,  size_t offset)
{
    size_t ret = 0;
    size_t numBytesAdvanced =  acpins_parse_pkgsize(buffer+offset, &ret);
    
    *advance += numBytesAdvanced;
    
    return  ret;
    
    
    
    // The high 2 bits of the first byte reveal how many follow bytes are in the PkgLength
    
    // 00xxxxxx -> 0 (invalid?)
    // 01xxxxxx -> 1
    // 10xxxxxx -> 2
    // 11xxxxxx -> 3
    /*
    uint8_t numBytes = buffer[offset] >> 6;
    *advance+=1;
    
    *advance+=numBytes;
    
    if (numBytes == 0)
    {
        return buffer[offset];
    }
    else //if (numBytes == 1)
    {
        uint8_t ret0 = buffer[offset];
        assert( ((ret0 >> 4) & 1U) == 0);
        assert( ((ret0 >> 5) & 1U) == 0); // bit 4 and 5 of the PkgLeadByte are reserved and must be zero

        ret0 &= ~(1UL << 6);
        ret0 &= ~(1UL << 7);
        
        // at this point ret0 is guarranted to have only bits 0-3 set
        
        size_t ret = ret0;
        
        for (int i=0;i<numBytes;i++)
        {
            ret = ret >> 8;
            ret += buffer[offset +i];
        }
        return ret;
    }
    

    return numBytes;
     */
}

AMLOperation _GetNextLOPOp(/*AMLParserState* state , */const uint8_t* buffer , size_t bufSize , size_t* advance ,  size_t offset)
{
    assert(*advance == 1);
    
    const uint8_t currentByte = buffer[offset];
    switch (currentByte)
    {
        case AML_OPCode_LNOT_Complement_LNotEqualOp:
            *advance +=1;
            return AML_LNotEqualOp;
            
        case AML_OPCode_LNOT_Complement_LLessEqualOp:
            *advance +=1;
            return AML_LLessEqualOp;
            
        case AML_OPCode_LNOT_Complement_LGreaterEqualOp:
            *advance +=1;
            return AML_LGreaterEqualOp;
            
        default:
            //assert(0);
            
            break;
    }
    
    return AML_Unknown;
}
AMLOperation _GetNextEXTOp(/*AMLParserState* state , */const uint8_t* buffer , size_t bufSize , size_t* advance ,  size_t offset)
{
    assert(*advance == 1);
    const uint8_t currentByte = buffer[offset];
    switch (currentByte)
    {
        case AML_OPCode_Complement_MutexOp:
            *advance += 1;
            return AML_MutexOp;
            
        case AML_OPCode_Complement_EventOp:
            *advance += 1;
            return AML_EventOp;
            
        case AML_OPCode_Complement_CondRefOfOp:
            *advance += 1;
            return AML_CondRefOfOp;
            
        case AML_OPCode_Complement_CreateFieldOp:
            *advance += 1;
            return AML_CreateFieldOp;
            
        case AML_OPCode_Complement_LoadTableOp:
            *advance += 1;
            return AML_LoadTableOp;
            
        case AML_OPCode_Complement_LoadOp:
            *advance += 1;
            return AML_LoadOp;
            
        case AML_OPCode_Complement_StallOp:
            *advance += 1;
            return AML_StallOp;
            
        case AML_OPCode_Complement_SleepOp:
            *advance += 1;
            return AML_SleepOp;
            
        case AML_OPCode_Complement_AcquireOp:
            *advance += 1;
            return AML_AcquireOp;
            
        case AML_OPCode_Complement_SignalOp:
            *advance += 1;
            return AML_SignalOp;
            
        case AML_OPCode_Complement_WaitOp:
            *advance += 1;
            return AML_WaitOp;
            
        case AML_OPCode_Complement_ResetOp:
            *advance += 1;
            return AML_ResetOp;
            
        case AML_OPCode_Complement_ReleaseOp:
            *advance += 1;
            return AML_ReleaseOp;
            
        case AML_OPCode_Complement_FromBCDOp:
            *advance += 1;
            return AML_FromBCDOp;
            
        case AML_OPCode_Complement_ToBCD:
            *advance += 1;
            return AML_ToBCD;
            
        case AML_OPCode_Complement_UnloadOp:
            *advance += 1;
            return AML_UnloadOp;
            
        case AML_OPCode_Complement_RevisionOp:
            *advance += 1;
            return AML_RevisionOp;
            
        case AML_OPCode_Complement_DebugOp:
            *advance += 1;
            return AML_DebugOp;
            
        case AML_OPCode_Complement_FatalOp:
            *advance += 1;
            return AML_FatalOp;
            
        case AML_OPCode_Complement_TimerOp:
            *advance += 1;
            return AML_TimerOp;
            
        case AML_OPCode_Complement_OpRegionOp:
            *advance += 1;
            return AML_OpRegionOp;
            
        case AML_OPCode_Complement_FieldOp:
            *advance += 1;
            return AML_FieldOp;
            
        case AML_OPCode_Complement_DeviceOpList:
            *advance += 1;
            return AML_DeviceOpList;
            
        case AML_OPCode_Complement_ProcessorOp:
            *advance += 1;
            return AML_ProcessorOp;
            
        case AML_OPCode_Complement_PowerResOp:
            *advance += 1;
            return AML_PowerResOp;
            
        case AML_OPCode_Complement_ThermalZoneOpList:
            *advance += 1;
            return AML_ThermalZoneOpList;
            
        case AML_OPCode_Complement_IndexFieldOp:
            *advance += 1;
            return AML_IndexFieldOp;
            
        case AML_OPCode_Complement_BankFieldOp:
            *advance += 1;
            return AML_BankFieldOp;
            
        case AML_OPCode_Complement_DataRegionOp:
            *advance += 1;
            return AML_DataRegionOp;
            
        default:
            //assert(0);
            break;
    }
    
    return AML_Unknown;
}

AMLOperation _GetNextOp(const uint8_t* buffer , size_t bufSize , size_t* advance ,  size_t offset)
{
    
    const uint8_t currentByte = buffer[offset];
    
    *advance = 1;
    
    if (currentByte>=0x41 && currentByte<=0x5A)
    {
        return AML_Char;
    }
    else if (currentByte>=0x30 && currentByte<=0x39)
    {
        return AML_Int;
    }
    
    switch (currentByte)
    {
        case AML_OP_ZeroOp:
            return AML_ZeroOp;
            
        case AML_OP_OneOp:
            return AML_OneOp;
            
        case AML_OP_ScopeOp:
            return AML_ScopeOp;
            
        case AML_OP_StringPrefix:
            return AML_StringPrefix;
            
        case AML_OP_NameOp:
            return AML_NameOp;
            
        case AML_OP_RootChar:
            return AML_RootChar;
            
        case AML_OP_NameChar:
            return AML_NameChar;
            
        case AML_OP_WordPrefix:
            return AML_WordPrefix;
            
        case AML_OP_ExtOpPrefix: // Needs to advance to next byte
            return _GetNextEXTOp(buffer+1, bufSize-1, advance, offset);
            
        case AML_OP_LnotOp: // Needs to advance to next byte
            return _GetNextLOPOp(buffer+1, bufSize-1, advance, offset);

        case AML_OP_LorOp:
            return AML_LorOp;
            
        case AML_OP_AliasOp:
            return AML_AliasOp;
            
        case AML_OP_BytePrefix:
            return AML_BytePrefix;
            
        case AML_OP_DWordPrefix:
            return AML_DWordPrefix;
            
        case AML_OP_QWordPrefix:
            return AML_QWordPrefix;
            
        case AML_OP_BufferOp:
            return AML_BufferOp;
            
        case AML_OP_PackageOp:
            return AML_PackageOp;
            
        case AML_OP_VarPackageOp:
            return AML_VarPackageOp;
            
        case AML_OP_MethodOp:
            return AML_MethodOp;
            
        case AML_OP_DualNamePrefix:
            return AML_DualNamePrefix;
            
        case AML_OP_MultiNamePrefix:
            return AML_MultiNamePrefix;
            
        case AML_OP_ParentPrefixChar:
            return AML_ParentPrefixChar;
            
        case AML_OP_Local0Op:
            return AML_Local0Op;
            
        case AML_OP_Local1Op:
            return AML_Local1Op;
            
        case AML_OP_Local2Op:
            return AML_Local2Op;
            
        case AML_OP_Local3Op:
            return AML_Local3Op;
            
        case AML_OP_Local4Op:
            return AML_Local4Op;
            
        case AML_OP_Local5Op:
            return AML_Local5Op;
            
        case AML_OP_Local6Op:
            return AML_Local6Op;
            
        case AML_OP_Local7Op:
            return AML_Local7Op;
            
        case AML_OP_Arg0Op:
            return AML_Arg0Op;
            
        case AML_OP_Arg1Op:
            return AML_Arg1Op;
            
        case AML_OP_Arg2Op:
            return AML_Arg2Op;
            
        case AML_OP_Arg3Op:
            return AML_Local1Op;
            
        case AML_OP_Arg4Op:
            return AML_Arg4Op;
            
        case AML_OP_Arg5Op:
            return AML_Arg5Op;
            
        case AML_OP_Arg6Op:
            return AML_Arg6Op;
            
        case AML_OP_StoreOp:
            return AML_StoreOp;
            
        case AML_OP_RefOfOp:
            return AML_RefOfOp;
            
        case AML_OP_AddOp:
            return AML_AddOp;
            
        case AML_OP_ConcatOp:
            return AML_ConcatOp;
            
        case AML_OP_SubtractOp:
            return AML_SubtractOp;
            
        case AML_OP_IncrementOp:
            return AML_IncrementOp;
            
        case AML_OP_DecrementOp:
            return AML_DecrementOp;
            
        case AML_OP_MultiplyOp:
            return AML_MultiplyOp;
            
        case AML_OP_DivideOp:
            return AML_DivideOp;
            
        case AML_OP_ShiftLeftOp:
            return AML_ShiftLeftOp;
            
        case AML_OP_ShiftRightOp:
            return AML_ShiftRightOp;
            
        case AML_OP_AndOp:
            return AML_AndOp;
            
        case AML_OP_NandOp:
            return AML_NandOp;
            
        case AML_OP_OrOp:
            return AML_OrOp;
            
        case AML_OP_NorOp:
            return AML_NorOp;
            
        case AML_OP_XorOp:
            return AML_XorOp;
            
        case AML_OP_NotOp:
            return AML_NotOp;
            
        case AML_OP_FindSetLeftBitOp:
            return AML_FindSetLeftBitOp;
            
        case AML_OP_FindSetRightBitOp:
            return AML_FindSetRightBitOp;
            
        case AML_OP_DerefOfOp:
            return AML_DerefOfOp;
            
        case AML_OP_ConcatResOp:
            return AML_ConcatResOp;
            
        case AML_OP_ModOp:
            return AML_ModOp;
            
        case AML_OP_NotifyOp:
            return AML_NotifyOp;
            
        case AML_OP_SizeOfOp:
            return AML_SizeOfOp;
            
        case AML_OP_IndexOp:
            return AML_IndexOp;
            
        case AML_OP_MatchOp:
            return AML_MatchOp;
            
        case AML_OP_CreateDWordFieldOp:
            return AML_CreateDWordFieldOp;
            
        case AML_OP_CreateWordFieldOp:
            return AML_CreateWordFieldOp;
            
        case AML_OP_CreateByteFieldOp:
            return AML_CreateByteFieldOp;
            
        case AML_OP_CreateBitFieldOp:
            return AML_CreateBitFieldOp;
            
        case AML_OP_TypeOp:
            return AML_TypeOp;
            
        case AML_OP_CreateQWordFieldOp:
            return AML_CreateQWordFieldOp;
            
        case AML_OP_LandOp:
            return AML_LandOp;
            
        case AML_OP_LEqualOp:
            return AML_LEqualOp;
            
        case AML_OP_LGreaterOp:
            return AML_LGreaterOp;
            
        case AML_OP_LLessOp:
            return AML_LLessOp;
            
        case AML_OP_ToBufferOp:
            return AML_ToBufferOp;
            
        case AML_OP_ToDecimalStringOp:
            return AML_ToDecimalStringOp;
            
        case AML_OP_ToHexStringOp:
            return AML_ToHexStringOp;
            
        case AML_OP_ToIntegerOp:
            return AML_ToIntegerOp;
            
        case AML_OP_ToStringOp:
            return AML_ToStringOp;
            
        case AML_OP_CopyObjectOp:
            return AML_CopyObjectOp;
            
        case AML_OP_MidOp:
            return AML_MidOp;
            
        case AML_OP_ContinueOp:
            return AML_ContinueOp;
            
        case AML_OP_IfOp:
            return AML_IfOp;
            
        case AML_OP_ElseOp:
            return AML_ElseOp;
            
        case AML_OP_WhileOp:
            return AML_WhileOp;
            
        case AML_OP_NoopOp:
            return AML_NoopOp;
            
        case AML_OP_ReturnOp:
            return AML_ReturnOp;
            
        case AML_OP_BreakOp:
            return AML_BreakOp;
            
        case AML_OP_BreakPointOp:
            return AML_BreakPointOp;
            
        case AML_OP_OnesOp:
            return AML_OnesOp;
            
        default:
            break;
    }
    
    return AML_Unknown;
}
