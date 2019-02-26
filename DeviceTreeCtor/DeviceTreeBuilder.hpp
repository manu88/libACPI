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

#include <vector>
#include <memory>
#include <stack>
#include <list>
#include <deque>
#include <string>
#include <sstream>

#include "AMLDecompilerInterface.hpp"


enum ValueType
{
    Type_NumericValue               = 0,
    Type_Buffer                     = 1,
    Type_RessourceTemplate          = 10,
    //Type_AddressSpaceDescriptor     = 11,
    Type_MemoryRangeDescriptor32    = 12,
    Type_WordAddressSpaceDescriptor = 13,
    Type_IOPortDescriptor,
    Type_DWordAddressSpaceDescriptor,
    Type_QWordAddressSpaceDescriptor,
    //Type_OperationRegion
    
};

struct RessourceItem
{
    
    void setValue(const QWordAddressSpaceDescriptor& desc)
    {
        type = Type_QWordAddressSpaceDescriptor;
        value.qwordAddressSpaceDescriptor = desc;
    }
    
    void setValue(const MemoryRangeDescriptor32 &desc)
    {
        type = Type_MemoryRangeDescriptor32;
        value.memoryRangeDesc32 = desc;
    }
    
    void setValue(const WordAddressSpaceDescriptor &desc)
    {
        type = Type_WordAddressSpaceDescriptor;
        value.wordAddressSpaceDescriptor = desc;
    }
    
    void setValue(const DWordAddressSpaceDescriptor &desc)
    {
        type = Type_DWordAddressSpaceDescriptor;
        value.dwordAddressSpaceDescriptor = desc;
    }
    
    void setValue(const IOPortDescriptor& desc)
    {
        type = Type_IOPortDescriptor;
        value.ioPortDescriptor = desc;
    }
    
    union
    {
        MemoryRangeDescriptor32    memoryRangeDesc32;
        AddressSpaceDescriptor     addressSpaceDescriptor;
        WordAddressSpaceDescriptor wordAddressSpaceDescriptor;
        IOPortDescriptor           ioPortDescriptor;
        DWordAddressSpaceDescriptor dwordAddressSpaceDescriptor;
        QWordAddressSpaceDescriptor qwordAddressSpaceDescriptor;
        
    } value;
    
    ValueType type;
};

struct ResourceTemplate
{
    std::vector<RessourceItem> items;
};


struct NameDeclaration
{
    NameDeclaration(const std::string& n):
    id(n)
    {}
    
  
    std::string id;
    bool isEisaid = false;
    
    
    void setValue( const uint8_t* buffer , size_t bufSize)
    {
        type = Type_Buffer;
        
        rawBuffer.clear();
        for( size_t i=0;i<bufSize;i++)
        {
            rawBuffer.push_back(buffer[i]);
        }
    }
    
    void setValue( uint64_t val)
    {
        type = Type_NumericValue;
        value64 = val;
    }
    
    
    
    void addTemplateItem( const RessourceItem& item)
    {
        assert(type == Type_RessourceTemplate);
    
        resTemplate.items.push_back(item);
    }
    
    ValueType type;
/*
    enum ValueType
    {
        Type_NumericValue = 0,
        Type_Buffer = 1,
        Type_RessourceTemplate = 10,
        Type_AddressSpaceDescriptor,
        Type_MemoryRangeDescriptor32,
        Type_WordAddressSpaceDescriptor,
        
    } type;
  */
    /*
    union
    {
        
        MemoryRangeDescriptor32    memoryRangeDesc32;
        AddressSpaceDescriptor     addressSpaceDescriptor;
        WordAddressSpaceDescriptor wordAddressSpaceDescriptor;
    }value;
    */
    
    uint64_t value64;
    ResourceTemplate resTemplate;   // type = Type_RessourceTemplate
    std::vector<uint8_t> rawBuffer; // type = Type_Buffer
};

/*
struct Device
{
    std::string id;
    std::string scope;
    
    std::vector<NameDeclaration> _names;
    
};
*/
struct TreeNode
{
    
    TreeNode():
    parent(nullptr)
    {}
    
    TreeNode( const std::string &id ,TreeNode*parent):
    id(id),
    parent(parent)
    {}
    
    TreeNode* getChildByName(const std::string&name)
    {
        auto c = std::count_if(children.begin(), children.end() , [name](const std::unique_ptr<TreeNode> &node)
                               {
                                   return node.get()->id == name;
                               });
        
        assert(c<=1);
        auto iter = std::find_if(children.begin(), children.end() , [name](const std::unique_ptr<TreeNode> &node)
                                 {
                                     return node.get()->id == name;
                                 });
        
        if (iter != children.end())
        {
            return iter->get();
        }
        return nullptr;
    }
    
    std::string id;
    
    TreeNode* parent;
    
    std::list< std::unique_ptr<TreeNode> > children;
    
    
    
    std::vector<NameDeclaration> _names;
    std::vector<ACPIOperationRegion> _opRegions;
    std::vector<ACPIField> _fields;
    std::vector<ACPIMethod> _methods;
};

struct DeviceTree
{
    TreeNode root;
    
    TreeNode* getNodeForPathAndCreateIfNeeded( const std::string &path , const std::string &relativeTo);
    TreeNode* getNodeForPath( const std::string &path , const std::string &relativeTo);
    
    
    ACPIDefinitionBlock defBlock = {0};
};

class DeviceTreeBuilder : public AMLDecompilerInterface
{
public:
    DeviceTreeBuilder(AMLDecompiler &decomp);

    
    enum BuilderState
    {
        Ready = 0,
        WaitingNameValue = 1,
    };
    
    BuilderState state;
    
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
    int onACPIDefinitionBlock( const ParserContext* context, const ACPIDefinitionBlock* desc) override;
    
    
    int startResourceTemplate( const ParserContext* context , size_t numItems ) override;
    int endResourceTemplate(const ParserContext* context , size_t numItemsParsed, AMLParserError err) override;
    //int onLargeItem(const ParserContext* context, LargeResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize)override;
    //int onSmallItem(const ParserContext* context, SmallResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize)override;
    
    
    int OnValue(const ParserContext* context, uint64_t value)override;
    int onOperationRegion(const ParserContext* context, const ACPIOperationRegion*)override;
    int onField(const ParserContext* context, const ACPIField*)override;
    
    
    int OnBuffer(const ParserContext* context , size_t bufferSize , const uint8_t* buffer)override;
    
    
    int StartScope(const ParserContext* context, const char* location)override;
    int EndScope(const ParserContext* context, const char* location)override;
    int StartDevice(const ParserContext* context, const ACPIDevice* name)override;
    int EndDevice(const ParserContext* context, const ACPIDevice* name)override;
    int StartName(const ParserContext* context, const char* name)override;
    int EndName(const ParserContext* context, const char* name)override;
    int onMethod(const ParserContext* context, const ACPIMethod* method)override;
    
    
    int onQWORDAddressSpaceDescriptor( const ParserContext* context , const QWordAddressSpaceDescriptor& desc) override;
    int onMemoryRangeDescriptor32( const ParserContext* context , const MemoryRangeDescriptor32& desc) override;
    int onWORDAddressSpaceDescriptor( const ParserContext* context , const WordAddressSpaceDescriptor& desc) override;
    int onDWORDAddressSpaceDescriptor( const ParserContext* context , const DWordAddressSpaceDescriptor& desc)override;
    
    int onIOPortDescriptor( const ParserContext* context , const IOPortDescriptor&desc)override;
private:
    
    std::stack<std::string> _scopes;
    DeviceTree _deviceTree;
    
    std::stack<TreeNode*> currentNode;// = nullptr;
    NameDeclaration* currentName = nullptr;
    
    

};


