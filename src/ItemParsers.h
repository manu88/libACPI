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

#include <AMLParser.h>
#include <AMLDecompiler.h>

typedef AMLParserError (*ParserItemMethod)(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize);




AMLParserError Parse_Reserved0(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize);

AMLParserError Parse_BitMemoryRangeDescriptor24(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize);
AMLParserError Parse_GenericRegisterDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize);
AMLParserError Parse_Reserved1(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize);
AMLParserError Parse_VendorDefinedDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize);
AMLParserError Parse_MemoryRangeDescriptor32(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize);
AMLParserError Parse_FixedLocationMemoryRangeDescriptor32(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize);
AMLParserError Parse_DWORDAddressSpaceDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize);
AMLParserError Parse_WORDAddressSpaceDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize);
AMLParserError Parse_ExtendedIRQDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize);
AMLParserError Parse_QWORDAddressSpaceDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize);
AMLParserError Parse_ExtendedAddressSpaceDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize);
AMLParserError Parse_ReservedStartCToF(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize);



// Small items
AMLParserError ParseSmall_IOPortDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize);
AMLParserError ParseSmall_Reserved(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize);
AMLParserError ParseSmall_IRQFormatDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize);
AMLParserError ParseSmall_DMAFormatDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize);
AMLParserError ParseSmall_StartDependentFunctionsDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t);
AMLParserError ParseSmall_EndDependentFunctionsDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize);
AMLParserError ParseSmall_FixedLocationIOPortDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize);
AMLParserError ParseSmall_VendorDefinedDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize);
AMLParserError ParseSmall_EndTagDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize);
