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
    const WordAddressSpaceDescriptor *desc = (const WordAddressSpaceDescriptor *) buffer;
    return decomp->callbacks.onLargeItem(decomp,context, LargeResourceItemsType_WORDAddressSpaceDescriptor,(const uint8_t*) desc , sizeof(desc));
    
}

AMLParserError Parse_DWORDAddressSpaceDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    const DWordAddressSpaceDescriptor *desc = (const DWordAddressSpaceDescriptor *) buffer;
    return decomp->callbacks.onLargeItem(decomp,context, LargeResourceItemsType_DWORDAddressSpaceDescriptor,(const uint8_t*) desc , sizeof(desc));
}

AMLParserError Parse_QWORDAddressSpaceDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    const QWordAddressSpaceDescriptor *desc = (const QWordAddressSpaceDescriptor *) buffer;
    
    return decomp->callbacks.onLargeItem(decomp,context, LargeResourceItemsType_QWORDAddressSpaceDescriptor,(const uint8_t*) desc , sizeof(desc));
}


AMLParserError Parse_FixedLocationMemoryRangeDescriptor32(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    const MemoryRangeDescriptor32* desc = (const MemoryRangeDescriptor32*) (buffer+2);// we skip the length part
    return decomp->callbacks.onLargeItem(decomp , context ,
                                         LargeResourceItemsType_MemoryRangeDescriptor32,
                                         (const uint8_t*) desc , sizeof(MemoryRangeDescriptor32));
/*
    desc.writeStatus = (buffer[0] >> 7) & 1U;
    
    union Conv32
    {
        uint8_t b[4];
        uint32_t v;
    }c;
    
    
    c.b[0] = buffer[1];
    c.b[1] = buffer[2];
    c.b[2] = buffer[3];
    c.b[3] = buffer[4];
    
    desc.rangeBaseAddr = c.v;
    
    c.b[0] = buffer[5];
    c.b[1] = buffer[6];
    c.b[2] = buffer[7];
    c.b[3] = buffer[8];
    
    desc.rangeLength = c.v;
    

*/

}
