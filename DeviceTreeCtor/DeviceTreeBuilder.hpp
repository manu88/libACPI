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

#include <map>
#include <vector>
#include <memory>

#include <list>
#include <deque>
#include <string>
#include <sstream>

#include <stack>

#include "AMLDecompilerInterface.hpp"
#include "DeviceTree.hpp"

class DeviceTreeBuilder : public AMLDecompilerInterface
{
public:
    DeviceTreeBuilder(AMLDecompiler &decomp);

    AMLParserError start(const uint8_t* buffer , size_t size);
    
    std::string getResult() const
    {
        return "";
    }
    

    void print();
    
    
    const DeviceTree &getDeviceTree() const
    {
        return _deviceTree;
    }
protected:
    int onACPIDefinitionBlock( const ParserContext* context, const ACPIDefinitionBlock& desc) override;
    
    
    int startResourceTemplate( const ParserContext* context , size_t numItems ) override;
    int endResourceTemplate(const ParserContext* context , size_t numItemsParsed, AMLParserError err) override;
    //int onLargeItem(const ParserContext* context, LargeResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize)override;
    //int onSmallItem(const ParserContext* context, SmallResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize)override;
    
    int onString(const ParserContext* context, const char* str) override;
    int onValue(const ParserContext* context, uint64_t value)override;
    int onOperationRegion(const ParserContext* context, const ACPIOperationRegion&)override;
    
    int startField(const ParserContext* context, const ACPIField&)override;
    int onFieldElement(const ParserContext* context, const ACPIFieldElement& fieldElement)override;
    int endField(const ParserContext* context, const ACPIField&)override;
    
    int startIndexField(const ParserContext* context, const ACPIIndexField&) override;
    int onIndexFieldElement(const ParserContext* context, const ACPIIndexFieldElement& fieldElement)  override;
    int endIndexField(const ParserContext* context, const ACPIIndexField&) override;
    
    
    
    int onBuffer(const ParserContext* context , size_t bufferSize , const uint8_t* buffer)override;
    
    
    int startScope(const ParserContext* context, const ACPIScope& scope)override;
    int endScope(const ParserContext* context, const ACPIScope& scope)override;
    int startDevice(const ParserContext* context, const ACPIDevice& name)override;
    int endDevice(const ParserContext* context, const ACPIDevice& name)override;
    int startName(const ParserContext* context, const char* name)override;
    //int EndName(const ParserContext* context, const char* name)override;
    int onMethod(const ParserContext* context, const ACPIMethod& method)override;
    
    
    int onQWORDAddressSpaceDescriptor( const ParserContext* context , const QWordAddressSpaceDescriptor& desc) override;
    int onMemoryRangeDescriptor32( const ParserContext* context , const MemoryRangeDescriptor32& desc) override;
    int onWORDAddressSpaceDescriptor( const ParserContext* context , const WordAddressSpaceDescriptor& desc) override;
    int onDWORDAddressSpaceDescriptor( const ParserContext* context , const DWordAddressSpaceDescriptor& desc)override;
    
    int onIOPortDescriptor( const ParserContext* context , const IOPortDescriptor&desc)override;
    
    int onPackage( const ParserContext*context , const ACPIPackage& package) override;
private:
    
    //ScopeResolver _scopeResolver;
    
    DeviceTree _deviceTree;
    
    std::stack<TreeNode*> currentNode;// = nullptr;
    
    
    ACPI::NameDeclaration* getCurrentName();
    //ACPI::NameDeclaration* currentName = nullptr;
    //std::string currentName;
    
    ACPI::FieldDeclaration *_currentFieldDecl = nullptr;
    ACPI::IndexFieldDeclaration *_currentIndexFieldDecl = nullptr;

};


