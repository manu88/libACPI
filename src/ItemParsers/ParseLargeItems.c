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


AMLParserError Parse_ExtendedIRQDescriptor(AMLDecompiler*decomp,const ParserContext* context,  const uint8_t* buffer , size_t bufferSize)
{
    const ExtendedInterruptDescriptor* desc = (const ExtendedInterruptDescriptor*) buffer;

    return decomp->callbacks.onLargeItem(decomp , context ,
                                  LargeResourceItemsType_ExtendedIRQDescriptor,
                                  (const uint8_t*) desc , sizeof(ExtendedInterruptDescriptor)
                                  );
}