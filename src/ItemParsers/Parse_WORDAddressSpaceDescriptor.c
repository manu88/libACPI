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
#include "../ItemParsers.h"


AMLParserError Parse_WORDAddressSpaceDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    WordAddressSpaceDescriptor desc = {0};
    
    desc.ressourceType     = buffer[0];
    desc.generalFlags      = buffer[1];
    desc.typeSpecificFlags = buffer[2];
    
    
    union WordConv
    {
        uint8_t b[2];
        uint16_t w;
    } conv;
    
    conv.b[0] = buffer[3];
    conv.b[1] = buffer[4];
    
    desc.addrSpaceGranularity = conv.w;
    
    conv.b[0] = buffer[5];
    conv.b[1] = buffer[6];
    
    desc.addrRangeMin = conv.w;
    
    conv.b[0] = buffer[7];
    conv.b[1] = buffer[8];
    
    desc.addrRangeMax = conv.w;
    
    conv.b[0] = buffer[9];
    conv.b[1] = buffer[10];
    
    desc.addrTranslationOffset = conv.w;
    
    conv.b[0] = buffer[11];
    conv.b[1] = buffer[12];
    
    desc.addrTranslationLength = conv.w;
    
    
    desc.ressourceSourceIndex = buffer[13];
    
    decomp->callbacks.onLargeItem(decomp,context, LargeResourceItemsType_WORDAddressSpaceDescriptor,(const uint8_t*) &desc , sizeof(desc));
    
    return AMLParserError_None;
}

AMLParserError Parse_DWORDAddressSpaceDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    DWordAddressSpaceDescriptor desc = {0};
    
    desc.ressourceType     = buffer[0];
    desc.generalFlags      = buffer[1];
    desc.typeSpecificFlags = buffer[2];
    
    
    union WordConv
    {
        uint8_t b[4];
        uint32_t w;
    } conv;
    
    conv.b[0] = buffer[3];
    conv.b[1] = buffer[4];
    conv.b[2] = buffer[5];
    conv.b[3] = buffer[6];
    
    desc.addrSpaceGranularity = conv.w;
    
    conv.b[0] = buffer[7];
    conv.b[1] = buffer[8];
    conv.b[2] = buffer[9];
    conv.b[3] = buffer[10];
    
    desc.addrRangeMin = conv.w;
    
    conv.b[0] = buffer[11];
    conv.b[1] = buffer[12];
    conv.b[2] = buffer[13];
    conv.b[3] = buffer[14];
    
    desc.addrRangeMax = conv.w;
    
    conv.b[0] = buffer[15];
    conv.b[1] = buffer[16];
    conv.b[2] = buffer[17];
    conv.b[3] = buffer[18];
    
    desc.addrTranslationOffset = conv.w;
    
    conv.b[0] = buffer[19];
    conv.b[1] = buffer[20];
    conv.b[2] = buffer[21];
    conv.b[3] = buffer[22];
    
    desc.addrTranslationLength = conv.w;
    
    
    
    decomp->callbacks.onLargeItem(decomp,context, LargeResourceItemsType_DWORDAddressSpaceDescriptor,(const uint8_t*) &desc , sizeof(desc));
    
    return AMLParserError_None;
    
}