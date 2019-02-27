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

#include "AMLDecompiler.h"

class AMLDecompilerInterface
{
public:
    virtual ~AMLDecompilerInterface();
protected:
    AMLDecompilerInterface(AMLDecompiler &decomp);
    
  
//
    virtual int onACPIDefinitionBlock( const ParserContext* context, const ACPIDefinitionBlock* desc) = 0;
    
    virtual int startResourceTemplate( const ParserContext* context , size_t numItems ) = 0;
    virtual int endResourceTemplate(const ParserContext* context , size_t numItemsParsed, AMLParserError err) = 0;
    
    virtual int onLargeItem(const ParserContext* context, LargeResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize);
    virtual int onSmallItem(const ParserContext* context, SmallResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize);
    
    
    virtual int OnValue(const ParserContext* context, uint64_t value)= 0;

    virtual int onOperationRegion(const ParserContext* context, const ACPIOperationRegion*)= 0;
    virtual int onField(const ParserContext* context, const ACPIField*)= 0;
    
    
    virtual int OnBuffer(const ParserContext* context , size_t bufferSize , const uint8_t* buffer)= 0;
    
    
    virtual int StartScope(const ParserContext* context, const char* location)= 0;
    virtual int EndScope(const ParserContext* context, const char* location)= 0;
    virtual int StartDevice(const ParserContext* context, const ACPIDevice* name)= 0;
    virtual int EndDevice(const ParserContext* context, const ACPIDevice* name)= 0;
    virtual int StartName(const ParserContext* context, const char* name)= 0;
    //virtual int EndName(const ParserContext* context, const char* name)= 0;
    
    virtual int onMethod(const ParserContext* context, const ACPIMethod* method)= 0;
    //virtual int endMethod(const ParserContext* context, const char* name)= 0;
    
    
    virtual int onQWORDAddressSpaceDescriptor( const ParserContext* context , const QWordAddressSpaceDescriptor& desc) = 0;
    virtual int onMemoryRangeDescriptor32( const ParserContext* context , const MemoryRangeDescriptor32& desc) = 0;
    virtual int onWORDAddressSpaceDescriptor( const ParserContext* context , const WordAddressSpaceDescriptor& desc) = 0;
    virtual int onDWORDAddressSpaceDescriptor( const ParserContext* context , const DWordAddressSpaceDescriptor& desc) = 0;
    virtual int onIOPortDescriptor( const ParserContext* context , const IOPortDescriptor&desc) = 0;
    
//
    AMLDecompiler &decomp;
};


