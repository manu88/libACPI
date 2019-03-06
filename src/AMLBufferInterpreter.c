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
#include "AMLBufferInterpreter.h"


static size_t ItemClassGetSize(const ItemClass* itemClass,const uint8_t*buffer)
{
    if (itemClass->isLarge)
    {
        union
        {
            uint8_t b[2];
            uint16_t c;
        } conv;
        
        conv.b[0] = buffer[1];
        conv.b[1] = buffer[2];
        
        assert( conv.c >= itemClass->minSize);
        
        return (size_t) conv.c;
        
    }
    
    return itemClass->minSize;
}

static size_t ItemClassGetBufferSize(const ItemClass* itemClass,const uint8_t*buffer)
{
    const size_t size = ItemClassGetSize(itemClass, buffer);
    if (itemClass->isLarge)
    {
        return size + 3;// 3 bytes header
    }
    
    return size + 1;
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

static const ItemClass items[] =
{
    MakeItemClass(0x47 , "IOPort",                     7,    0), // 6.4.2.5     I/O Port Descriptor
    MakeItemClass(0x79 , "EndTag",                     1,    0), // 6.4.2.8     End Tag
    MakeItemClass(0x86 , "32BitFixedMemoryRangeDesc" , 9   , 1), // 6.4.3.4     32-Bit Fixed Memory Range Descriptor
    MakeItemClass(0x87 , "DWordAddressSpace" ,         23  , 1), // 6.4.3.5.2   DWord Address Space Descriptor
    MakeItemClass(0x88 , "WordAddressSpace" ,          0x0D, 1), // 6.4.3.5.3   Word Address Space Descriptor
    MakeItemClass(0x8A , "QWordAddressSpace" ,         43  , 1), // 6.4.3.5.1   QWord Address Space Descriptor
    
};

static const ItemClass* getItemClassFromBuffer(const uint8_t*buffer , size_t bufSize)
{
    for(size_t i=0;i<(sizeof(items)/sizeof(ItemClass));i++)
    {
        const ItemClass* item = items + i;
        
        if (item->itemName == *buffer && bufSize >= item->minSize )
        {
            return item;
        }
        
    }
    return NULL;
}


AMLParserError DecodeBufferObject(const uint8_t* buffer , size_t bufferSize, AMLBufferAnalysisResults* results)
{
    assert(results);
    
    if (!buffer)
        return AMLParserError_BufferTooShort;
    
    
    // check if n-1 byte is '6.4.2.8 End Tag'
    
    
    if ( bufferSize == 0 || buffer[bufferSize-2] != 0x79)
    {
        results->numItems = 0; // this is a plain buffer
        return AMLParserError_None;
    }

    
    const ItemClass* item = getItemClassFromBuffer(buffer, bufferSize);
    
    if (item)
    {
        const size_t itemSize = ItemClassGetBufferSize(item, buffer);
        
        results->entries[results->numItems].bufferPos = buffer;
        results->entries[results->numItems].size = ItemClassGetSize(item, buffer);
        results->entries[results->numItems].type = (item->isLarge? 0xF000 : 0x0000) + item->itemName;
        results->entries[results->numItems].isa = item;
        
        results->numItems++;
        
        const size_t remains =bufferSize - itemSize;
        
        
        if (remains)
        {
            const uint8_t* newBuf = buffer + itemSize;
            const AMLParserError err =  DecodeBufferObject( newBuf, bufferSize-itemSize , results);
        
            if (err != AMLParserError_None)
            {
                return err;
            }
        }
    }
    else
    {
        //assert(0);
        results->numItems = 0; // this is a plain buffer
        //printf("Type for 0x%x Not found \n" , buffer[0] );
    }
    
    return AMLParserError_None;
}
