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



#include "DeviceTreeBuilder.hpp"
#include <EISAID.h>

TreeNode*  DeviceTree::getNodeForPathAndCreateIfNeeded( const std::string &path , const std::string &relativeTo)
{
    std::istringstream fullPath(relativeTo + (path[0] == '.'?"":".") + path);
    
    
    TreeNode* ret = &root;
    
    std::string s;
    while (getline(fullPath, s, '.'))
    {
        if (!s.empty())
        {
            TreeNode*n = ret->getChildByName(s);
            
            if (n == nullptr)
            {
                ret->children.push_back( std::make_unique<TreeNode>(TreeNode(s , ret)));
            }
            
            ret = ret->getChildByName(s);
        }

    }
    
    return ret;
}

TreeNode* DeviceTree::getNodeForPath( const std::string &path , const std::string &relativeTo)
{
    std::istringstream fullPath(relativeTo + (path[0] == '.'?"":".") + path);
    std::vector<std::string> strings;
    
    TreeNode* ret = &root;
    
    std::string s;
    while (getline(fullPath, s, '.'))
    {
        if (!s.empty())
        {
            strings.push_back(s);
            
            TreeNode*n = ret->getChildByName(s);
            
            if (n == nullptr)
            {
                return nullptr;
            }
            
            ret = ret->getChildByName(s);
        }
    }
    
    return ret;
}

DeviceTreeBuilder::DeviceTreeBuilder(AMLDecompiler& decomp):
AMLDecompilerInterface(decomp),
state(BuilderState::Ready)
{
    decomp.parserPolicy.assertOnError = 1;
}

static void printDefBlock( const ACPIDefinitionBlock& defBlock)
{
    
    printf("tableSignature :'%s'\n" , defBlock.tableSignature);
    printf("OEMId :'%s'\n" , defBlock.OEMId);
    printf("tableId :'%s'\n" , defBlock.tableId);
    printf("tableLength :%d\n" , defBlock.tableLength);
    printf("complianceRevision :%hhu\n" , defBlock.complianceRevision);
    printf("tableCheckSum :%hhu\n" , defBlock.tableCheckSum);
    printf("OEMRev : %d\n" , defBlock.OEMRev);
    printf("creatorID : %d\n" , defBlock.OEMRev);
    
}

static void printDev(TreeNode* node ,int indent)
{
    for(int i=0;i<indent;i++)
        printf("\t");
    
    printf("TreeNode '%s' %zi names : \n" , node->id.c_str() , node->_names.size() );
    for(const auto &name : node->_names)
    {
        for(int i=0;i<indent+1;i++)
            printf("\t");
        
        if (name.id == "_HID")
        {
            if (isEisaId(name.value64) )
            {
                char eisaid[8] = {0};
                assert(getEisaidString(name.value64, eisaid));
                printf("%s:%s",name.id.c_str() ,eisaid);
            }
            else
            {
                printf("%s:0x%llx",name.id.c_str() ,name.value64);
            }
            
            //printf("%s:%s",name.id.c_str() ,isEisaId(name.value64)? GetEisaId(name.value64): std::to_string(name.value64).c_str() );
        }
        else
        {
            printf("%s:%llx (type %i)",name.id.c_str() ,name.value64 , name.type );
        }
        printf("\n");
    }
    
    
    for (const auto &dev : node->children)
    {
        indent++;
        printDev(dev.get(), indent);
        indent--;
        
    }
    
};

void DeviceTreeBuilder::print()
{

    printDefBlock(_deviceTree.defBlock);
    printDev(&_deviceTree.root ,0);
    
    
    /*
    for( const auto &dev : _devices)
    {
        printf("Device '%s'\n" , dev.id.c_str());
        printf("Scope '%s'\n" , dev.scope.c_str());
        for( const auto &n : dev._names )
        {
            printf("\t Name '%s' %llx\n" , n.id.c_str() , n.value);
        }
    }
     */
}


int DeviceTreeBuilder::startResourceTemplate( const ParserContext* context , size_t numItems )
{
    assert(currentName);
    currentName->type = ValueType::Type_RessourceTemplate;
    return 0;
}
int DeviceTreeBuilder::endResourceTemplate(const ParserContext* context , size_t numItemsParsed, AMLParserError err)
{
    return 0;
}
/*
int DeviceTreeBuilder::onLargeItem(const ParserContext* context, LargeResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize)
{
    switch (itemType)
    {
        case LargeResourceItemsType_MemoryRangeDescriptor32:
            assert(bufferSize == sizeof(MemoryRangeDescriptor32));
            return onMemoryRangeDescriptor32(context, reinterpret_cast<const MemoryRangeDescriptor32&>(*buffer));
            
        case LargeResourceItemsType_QWORDAddressSpaceDescriptor:
            return onQWORDAddressSpaceDescriptor(context, reinterpret_cast<const QWordAddressSpaceDescriptor&>(*buffer));
            
        case LargeResourceItemsType_WORDAddressSpaceDescriptor:
            return onWORDAddressSpaceDescriptor(context, reinterpret_cast<const WordAddressSpaceDescriptor&>(*buffer));
            
        case LargeResourceItemsType_DWORDAddressSpaceDescriptor:
            return onDWORDAddressSpaceDescriptor(context, reinterpret_cast<const DWordAddressSpaceDescriptor&>(*buffer));
        default:
            assert(0);
            break;
    }
    return 0;
}

int DeviceTreeBuilder::onSmallItem(const ParserContext* context, SmallResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize)
{
    switch (itemType)
    {
        case SmallResourceItemsType_IOPortDescriptor:
            
            return onIOPortDescriptor(context, reinterpret_cast<const IOPortDescriptor&>(*buffer));
            
        default:
            assert(0);
            break;
    }
    return 0;
}
*/


int DeviceTreeBuilder::onOperationRegion(const ParserContext* context, const ACPIOperationRegion* reg)
{
    
    return 0;
}

int DeviceTreeBuilder::onField(const ParserContext* context, const ACPIField*field)
{

    return 0;
}



int DeviceTreeBuilder::OnBuffer(const ParserContext* context , size_t bufferSize , const uint8_t* buffer)
{

    state = Ready;
    
    currentName->setValue(buffer, bufferSize);

    
    return 0;
}

int DeviceTreeBuilder::StartDevice(const ParserContext* context, const ACPIDevice* device)
{
    currentNode = _deviceTree.getNodeForPathAndCreateIfNeeded(device->name, _scopes.empty()? "" :  _scopes.top());
    //printf("Start Device named '%s' current scope '%s' -> %s \n" , device->name , _scopes.top().c_str() , currentNode? "found":"Not found" );
    
    //assert(node);
    return 0;
}

int DeviceTreeBuilder::StartScope(const ParserContext* context, const char* location)
{
    
    currentNode = _deviceTree.getNodeForPathAndCreateIfNeeded(location, _scopes.empty()? "" :  _scopes.top());
    
    _scopes.push( (_scopes.empty()? "" :  _scopes.top() )+  location);
    assert(currentNode);

    return 0;
}

int DeviceTreeBuilder::EndScope(const ParserContext* context, const char* location)
{
    //printf("End scope '%s' '%s'\n" , location , _scopes.back().c_str());
    _scopes.pop();
    return 0;
}

int DeviceTreeBuilder::EndDevice(const ParserContext* context, const ACPIDevice* device)
{
    currentNode = nullptr;
    return 0;
}

int DeviceTreeBuilder::StartName(const ParserContext* context, const char* name)
{
    //assert( state != WaitingNameValue);

    state = WaitingNameValue;
    
    assert(currentNode);
    
    
    currentNode->_names.push_back(NameDeclaration(name) );
    currentName = &currentNode->_names.back();
    
    return 0;
}




int DeviceTreeBuilder::EndName(const ParserContext* context, const char* name)
{
    assert( state != WaitingNameValue);
    
    state = Ready;
    
    return 0;
}
int DeviceTreeBuilder::startMethod(const ParserContext* context, const char* name)
{

    return 0;
}
int DeviceTreeBuilder::endMethod(const ParserContext* context, const char* name)
{
    return 0;
}


int DeviceTreeBuilder::onACPIDefinitionBlock( const ParserContext* context, const ACPIDefinitionBlock* desc)
{
    _deviceTree.defBlock = *desc;
    return 0;
}

int DeviceTreeBuilder::OnValue(const ParserContext* context, uint64_t value)
{
    state = Ready;
    
    
    if(currentName)
    {
        currentName->setValue(value);
        
        currentName->isEisaid = isEisaId(value);
        
    }
    return 0;
}

int DeviceTreeBuilder::onQWORDAddressSpaceDescriptor( const ParserContext* context , const QWordAddressSpaceDescriptor& desc)
{
    
    state = Ready;
    assert(currentName);
    if(currentName)
    {
        RessourceItem item;
        item.setValue(desc);
        currentName->addTemplateItem(item);
    }
    return 0;
}


int DeviceTreeBuilder::onIOPortDescriptor( const ParserContext* context , const IOPortDescriptor&desc)
{
    assert(currentName);
    if(currentName)
    {
        RessourceItem item;
        item.setValue(desc);
        currentName->addTemplateItem(item);
    }
    return 0;
}

int DeviceTreeBuilder::onMemoryRangeDescriptor32( const ParserContext* context , const MemoryRangeDescriptor32& desc)
{
    state = Ready;
    assert(currentName);
    if(currentName)
    {
        RessourceItem item;
        item.setValue(desc);
        currentName->addTemplateItem(item);
    }
    return 0;
}

int DeviceTreeBuilder::onDWORDAddressSpaceDescriptor( const ParserContext* context , const DWordAddressSpaceDescriptor& desc)
{
    state = Ready;
    assert(currentName);
    if(currentName)
    {
        RessourceItem item;
        item.setValue(desc);
        currentName->addTemplateItem(item);
    }
    return 0;
}

int DeviceTreeBuilder::onWORDAddressSpaceDescriptor( const ParserContext* context , const WordAddressSpaceDescriptor& desc)
{
    
    state = Ready;
    assert(currentName);
    if(currentName)
    {
        RessourceItem item;
        item.setValue(desc);
        currentName->addTemplateItem(item);
    }
    return 0;
}

AMLParserError DeviceTreeBuilder::start(const uint8_t* buffer , size_t size)
{
    return AMLDecompilerStart(&decomp, buffer, size);
}
