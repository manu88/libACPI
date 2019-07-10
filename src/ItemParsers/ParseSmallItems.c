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

    
    const IOPortDescriptor *desc = (const IOPortDescriptor *) buffer;
    
    return decomp->callbacks.onSmallItem(decomp , context ,
                                  SmallResourceItemsType_IOPortDescriptor,
                                  (const uint8_t*) desc , sizeof(IOPortDescriptor)
                                  );
    
    //return AMLParserError_None;
}


AMLParserError ParseSmall_IRQFormatDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    
    IRQDescriptor desc = {0};
    
    union
    {
        uint8_t b[2];
        uint16_t v;
    } c;
    
    c.b[0] = buffer[0];
    c.b[1] = buffer[1];
    
    desc.maskBits = c.v;
    
    decomp->callbacks.onSmallItem(decomp , context ,
                                  SmallResourceItemsType_IRQFormatDescriptor,
                                  (const uint8_t*) &desc , sizeof(IRQDescriptor)
                                  );
    
    
    return AMLParserError_None;
}

AMLParserError ParseSmall_DMAFormatDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    decomp->callbacks.onSmallItem(decomp , context , SmallResourceItemsType_DMAFormatDescriptor , buffer , bufferSize);

    return AMLParserError_None;
}
