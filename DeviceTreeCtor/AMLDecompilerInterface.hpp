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
    virtual int onACPIDefinitionBlock( const ParserContext* context, const ACPIDefinitionBlock& desc) = 0;
    
    virtual int startResourceTemplate( const ParserContext* context , size_t numItems ) = 0;
    virtual int endResourceTemplate(const ParserContext* context , size_t numItemsParsed, AMLParserError err) = 0;
    
    virtual int onLargeItem(const ParserContext* context, LargeResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize);
    virtual int onSmallItem(const ParserContext* context, SmallResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize);
    
    
    virtual int onValue(const ParserContext* context, uint64_t value)= 0;
    virtual int onString(const ParserContext* context, const char* str)= 0;

    virtual int onOperationRegion(const ParserContext* context, const ACPIOperationRegion&)= 0;
    
    virtual int startField(const ParserContext* context, const ACPIField&)= 0;
    virtual int onFieldElement(const ParserContext* context, const ACPIFieldElement& fieldElement) = 0;
    virtual int endField(const ParserContext* context, const ACPIField&)= 0;
    
    virtual int startIndexField(const ParserContext* context, const ACPIIndexField&)= 0;
    virtual int onIndexFieldElement(const ParserContext* context, const ACPIIndexFieldElement& fieldElement) = 0;
    virtual int endIndexField(const ParserContext* context, const ACPIIndexField&)= 0;
    
    
    virtual int onBuffer(const ParserContext* context , size_t bufferSize , const uint8_t* buffer)= 0;
    
    
    virtual int startScope(const ParserContext* context, const ACPIScope& scope)= 0;
    virtual int endScope(const ParserContext* context, const ACPIScope& scope)= 0;
    virtual int startDevice(const ParserContext* context, const ACPIDevice& name)= 0;
    virtual int endDevice(const ParserContext* context, const ACPIDevice& name)= 0;
    virtual int startName(const ParserContext* context, const char* name)= 0;
    //virtual int EndName(const ParserContext* context, const char* name)= 0;
    
    virtual int onMethod(const ParserContext* context, const ACPIMethod& method)= 0;
    //virtual int endMethod(const ParserContext* context, const char* name)= 0;
    
    
    virtual int onQWORDAddressSpaceDescriptor( const ParserContext* context , const QWordAddressSpaceDescriptor& desc) = 0;
    virtual int onMemoryRangeDescriptor32( const ParserContext* context , const MemoryRangeDescriptor32& desc) = 0;
    virtual int onWORDAddressSpaceDescriptor( const ParserContext* context , const WordAddressSpaceDescriptor& desc) = 0;
    virtual int onDWORDAddressSpaceDescriptor( const ParserContext* context , const DWordAddressSpaceDescriptor& desc) = 0;
    virtual int onIOPortDescriptor( const ParserContext* context , const IOPortDescriptor&desc) = 0;
    
    virtual int onPackage( const ParserContext*context , const ACPIPackage& package) = 0;

    
//
    AMLDecompiler &decomp;
};


