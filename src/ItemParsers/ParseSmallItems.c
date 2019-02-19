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


AMLParserError ParseSmall_IOPortDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    assert(bufferSize == 7);
    IOPortDescriptor desc;
    
    desc.isDecoder = buffer[0] == 1? 1:0;
    
    union
    {
        uint8_t b[8];
        uint16_t v;
    } c;
    
    c.b[0] = buffer[1];
    c.b[1] = buffer[2];
    
    desc.rangeMinBaseAddr = c.v;
    
    c.b[0] = buffer[3];
    c.b[1] = buffer[4];
    
    desc.rangeMaxBaseAddr = c.v;
    
    desc.baseAlign = buffer[5];
    desc.rangeLen = buffer[6];
    
    
    if(decomp->callbacks.onSmallItem)
    {
        decomp->callbacks.onSmallItem(decomp , context ,
                                      SmallResourceItemsType_IOPortDescriptor,
                                      (const uint8_t*) &desc , sizeof(IOPortDescriptor)
                                      );
    }
    return AMLParserError_None;
}
