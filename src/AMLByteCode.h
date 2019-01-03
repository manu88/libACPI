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

typedef enum
{
    AML_OP_ZeroOp                 = 0x0,
    AML_OP_OneOp                  = 0x01,
    AML_OP_AliasOp                = 0x06,
    AML_OP_NameOp                 = 0x08,
    
    AML_OP_BytePrefix             = 0x0A,
    AML_OP_WordPrefix             = 0x0B,
    AML_OP_DWordPrefix            = 0x0C, // start of value Integer => 0x00000000-0xFFFFFFFF
    AML_OP_StringPrefix           = 0x0D,
    AML_OP_QWordPrefix            = 0x0E,
    AML_OP_ScopeOp                = 0x10,
    AML_OP_BufferOp               = 0x11,
    AML_OP_PackageOp              = 0x12,
    AML_OP_VarPackageOp           = 0x13,
    AML_OP_MethodOp               = 0x14,
    AML_OP_DualNamePrefix         = 0x2E,
    AML_OP_MultiNamePrefix        = 0x2F,
    
    AML_OP_ExtOpPrefix            = 0x5b,
    AML_OP_RootChar               = 0x5c,
    AML_OP_ParentPrefixChar       = 0x5E,
    AML_OP_NameChar               = 0x5f,
    
    AML_OP_Local0Op               = 0x60,
    AML_OP_Local1Op               = 0x61,
    AML_OP_Local2Op               = 0x62,
    AML_OP_Local3Op               = 0x63,
    AML_OP_Local4Op               = 0x64,
    AML_OP_Local5Op               = 0x65,
    AML_OP_Local6Op               = 0x66,
    AML_OP_Local7Op               = 0x67,
    AML_OP_Arg0Op                 = 0x68,
    AML_OP_Arg1Op                 = 0x69,
    AML_OP_Arg2Op                 = 0x6A,
    AML_OP_Arg3Op                 = 0x6B,
    AML_OP_Arg4Op                 = 0x6C,
    AML_OP_Arg5Op                 = 0x6D,
    AML_OP_Arg6Op                 = 0x6E,
    AML_OP_StoreOp                = 0x70,
    AML_OP_RefOfOp                = 0x71,
    AML_OP_AddOp                  = 0x72,
    AML_OP_ConcatOp               = 0x73,
    AML_OP_SubtractOp             = 0x74,
    AML_OP_IncrementOp            = 0x75,
    AML_OP_DecrementOp            = 0x76,
    AML_OP_MultiplyOp             = 0x77,
    AML_OP_DivideOp               = 0x78,
    AML_OP_ShiftLeftOp            = 0x79,
    AML_OP_ShiftRightOp           = 0x7A,
    AML_OP_AndOp                  = 0x7B,
    AML_OP_NandOp                 = 0x7C,
    AML_OP_OrOp                   = 0x7D,
    AML_OP_NorOp                  = 0x7E,
    AML_OP_XorOp                  = 0x7F,
    AML_OP_NotOp                  = 0x80,
    AML_OP_FindSetLeftBitOp       = 0x81,
    AML_OP_FindSetRightBitOp      = 0x82,
    AML_OP_DerefOfOp              = 0x83,
    AML_OP_ConcatResOp            = 0x84,
    AML_OP_ModOp                  = 0x85,
    AML_OP_NotifyOp               = 0x86,
    AML_OP_SizeOfOp               = 0x87,
    AML_OP_IndexOp                = 0x88,
    AML_OP_MatchOp                = 0x89,
    AML_OP_CreateDWordFieldOp     = 0x8A,
    AML_OP_CreateWordFieldOp      = 0x8B,
    AML_OP_CreateByteFieldOp      = 0x8C,
    AML_OP_CreateBitFieldOp       = 0x8D,
    AML_OP_TypeOp                 = 0x8E,
    AML_OP_CreateQWordFieldOp     = 0x8F,
    AML_OP_LandOp                 = 0x90,
    AML_OP_LorOp                  = 0x91,
    AML_OP_LnotOp                 = 0x92,
    
    AML_OP_LEqualOp               = 0x93,
    AML_OP_LGreaterOp             = 0x94,
    AML_OP_LLessOp                = 0x95,
    AML_OP_ToBufferOp             = 0x96,
    AML_OP_ToDecimalStringOp      = 0x97,
    AML_OP_ToHexStringOp          = 0x98,
    AML_OP_ToIntegerOp            = 0x99,
    AML_OP_ToStringOp             = 0x9C,
    AML_OP_CopyObjectOp           = 0x9D,
    AML_OP_MidOp                  = 0x9E,
    AML_OP_ContinueOp             = 0x9F,
    AML_OP_IfOp                   = 0xA0,
    AML_OP_ElseOp                 = 0xA1,
    AML_OP_WhileOp                = 0xA2,
    AML_OP_NoopOp                 = 0xA3,
    AML_OP_ReturnOp               = 0xA4,
    AML_OP_BreakOp                = 0xA5,
    AML_OP_BreakPointOp           = 0xCC,
    AML_OP_OnesOp                 = 0xFF,
    
    
    
    
} AML_OPCode;

typedef enum
{
    AML_OPCode_Complement_MutexOp              =  0x01,
    AML_OPCode_Complement_EventOp              =  0x02,
    AML_OPCode_Complement_CondRefOfOp          =  0x12,
    AML_OPCode_Complement_CreateFieldOp        =  0x13,
    AML_OPCode_Complement_LoadTableOp          =  0x1F,
    AML_OPCode_Complement_LoadOp               =  0x20,
    AML_OPCode_Complement_StallOp              =  0x21,
    AML_OPCode_Complement_SleepOp              =  0x22,
    AML_OPCode_Complement_AcquireOp            =  0x23,
    AML_OPCode_Complement_SignalOp             =  0x24,
    AML_OPCode_Complement_WaitOp               =  0x25,
    AML_OPCode_Complement_ResetOp              =  0x26,
    AML_OPCode_Complement_ReleaseOp            =  0x27,
    AML_OPCode_Complement_FromBCDOp            =  0x28,
    AML_OPCode_Complement_ToBCD                =  0x29,
    AML_OPCode_Complement_UnloadOp             =  0x2A,
    AML_OPCode_Complement_RevisionOp           =  0x30,
    AML_OPCode_Complement_DebugOp              =  0x31,
    AML_OPCode_Complement_FatalOp              =  0x32,
    AML_OPCode_Complement_TimerOp              =  0x33,
    AML_OPCode_Complement_OpRegionOp           =  0x80,
    AML_OPCode_Complement_FieldOp              =  0x81,
    AML_OPCode_Complement_DeviceOpList         =  0x82,
    AML_OPCode_Complement_ProcessorOp          =  0x83,
    AML_OPCode_Complement_PowerResOp           =  0x84,
    AML_OPCode_Complement_ThermalZoneOpList    =  0x85,
    AML_OPCode_Complement_IndexFieldOp         =  0x86,
    AML_OPCode_Complement_BankFieldOp          =  0x87,
    AML_OPCode_Complement_DataRegionOp         =  0x88,
    
} AML_OPCode_Complement;



typedef enum
{
    AML_OPCode_LNOT_Complement_LNotEqualOp            = 0x93,// Needs 2nd byte 0x93
    AML_OPCode_LNOT_Complement_LLessEqualOp           = 0x94,// Needs 2nd byte 0x94
    AML_OPCode_LNOT_Complement_LGreaterEqualOp        = 0x95,// Needs 2nd byte 0x95
    
} AML_OPCode_LNOT_Complement;


