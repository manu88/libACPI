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

#include <stddef.h>
#include <stdint.h>
typedef enum
{
    AML_Unknown           , // 0
    AML_ZeroOp            ,
    AML_OneOp             ,
    AML_AliasOp           ,
    AML_NameOp            ,
    AML_BytePrefix        ,
    AML_WordPrefix        ,
    AML_DWordPrefix       , // 7
    AML_StringPrefix      ,
    AML_QWordPrefix       ,
    AML_ScopeOp           , // 10
    AML_BufferOp          ,
    AML_PackageOp         ,
    AML_VarPackageOp      ,
    AML_MethodOp          ,
    AML_DualNamePrefix    ,
    AML_MultiNamePrefix   ,//16
    AML_RootChar          ,
    AML_ParentPrefixChar  ,
    AML_NameChar          ,
    AML_Local0Op          , // 20
    AML_Local1Op          ,
    AML_Local2Op          ,
    AML_Local3Op          ,
    AML_Local4Op          ,
    AML_Local5Op          ,
    AML_Local6Op          ,
    AML_Local7Op          ,
    AML_Arg0Op            ,
    AML_Arg1Op            ,
    AML_Arg2Op            ,//30
    AML_Arg3Op            ,
    AML_Arg4Op            ,
    AML_Arg5Op            ,
    AML_Arg6Op            ,
    AML_StoreOp           ,
    AML_RefOfOp           ,
    AML_AddOp             ,
    AML_ConcatOp          ,
    AML_SubtractOp        ,
    AML_IncrementOp       ,//40
    AML_DecrementOp       ,
    AML_MultiplyOp        ,
    AML_DivideOp          ,
    AML_ShiftLeftOp       ,
    AML_ShiftRightOp      ,
    AML_AndOp             ,
    AML_NandOp            ,
    AML_OrOp              ,
    AML_NorOp             ,
    AML_XorOp             ,//50
    AML_NotOp             ,
    AML_FindSetLeftBitOp  ,
    AML_FindSetRightBitOp ,
    AML_DerefOfOp         ,
    AML_ConcatResOp       ,
    AML_ModOp             ,
    AML_NotifyOp          ,
    AML_SizeOfOp          ,
    AML_IndexOp           ,
    AML_MatchOp           ,//60
    AML_CreateDWordFieldOp,
    AML_CreateWordFieldOp ,
    AML_CreateByteFieldOp ,
    AML_CreateBitFieldOp  ,
    AML_TypeOp            ,// 65
    AML_CreateQWordFieldOp,
    AML_LandOp            ,
    AML_LorOp             ,
    AML_LEqualOp          ,
    AML_LGreaterOp        ,//70
    AML_LLessOp           ,
    AML_ToBufferOp        ,
    AML_ToDecimalStringOp ,
    AML_ToHexStringOp     ,
    AML_ToIntegerOp       ,
    AML_ToStringOp        ,
    AML_CopyObjectOp      ,
    AML_MidOp             ,
    AML_ContinueOp        ,
    AML_IfOp              ,//80
    AML_ElseOp            ,
    AML_WhileOp           ,
    AML_NoopOp            ,
    AML_ReturnOp          ,
    AML_BreakOp           ,
    AML_BreakPointOp      ,
    AML_OnesOp            ,
    AML_MutexOp           ,
    AML_EventOp           ,
    AML_CondRefOfOp       ,//90
    AML_CreateFieldOp     ,
    AML_LoadTableOp       ,
    AML_LoadOp            ,
    AML_StallOp           ,
    AML_SleepOp           ,
    AML_AcquireOp         ,
    AML_SignalOp          ,
    AML_WaitOp            ,
    AML_ResetOp           ,
    AML_ReleaseOp         ,//100
    AML_FromBCDOp         ,
    AML_ToBCD             ,
    AML_UnloadOp          ,
    AML_RevisionOp        ,
    AML_DebugOp           ,
    AML_FatalOp           ,
    AML_TimerOp           ,
    AML_OpRegionOp        ,
    AML_FieldOp           ,
    AML_DeviceOpList      ,//110
    AML_ProcessorOp       ,
    AML_PowerResOp        ,
    AML_ThermalZoneOpList ,
    AML_IndexFieldOp      ,
    AML_BankFieldOp       ,
    AML_DataRegionOp      ,
    AML_LNotEqualOp       ,
    AML_LLessEqualOp      ,
    AML_LGreaterEqualOp   ,
    AML_Char              ,//120
    AML_Int               ,
    
} AMLOperation;


AMLOperation _GetNextOp(/*AMLParserState* state ,*/ const uint8_t* buffer , size_t bufSize , size_t* advance ,  size_t offset);

AMLOperation _GetNextEXTOp(/*AMLParserState* state , */const uint8_t* buffer , size_t bufSize , size_t* advance ,  size_t offset);
AMLOperation _GetNextLOPOp(/*AMLParserState* state , */const uint8_t* buffer , size_t bufSize , size_t* advance ,  size_t offset);

size_t _GetPackageLength(const uint8_t* buffer , size_t bufSize , size_t* advance ,  size_t offset);
