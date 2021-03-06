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
#include <ACPIDesc.h>


TreeNode* DeviceTree::getNodeForPathAndCreateIfNeeded( const AMLName& name,TreeNode* current)
{
    //char* resolved = AMLNameConstructNormalized(&name);
    
    TreeNode* ret = nullptr;
    
    
    if (AMLNameHasPrefixRoot(&name))
    {
        ret = &root;
    }
    else
    {
        ret = current;
    }
    
    uint8_t numParents =  AMLNameCountParents(&name);
    
    
    while (numParents--)
    {
        ret = ret->parent;
    }
    
    
    uint8_t numSegments = AMLNameCountSegments(&name);
    
    for (uint8_t i = 0;i<numSegments;i++)
    {
        char toBuffer[5] = {0};
        assert(AMLNameGetSegment(&name, i, toBuffer));
        
        TreeNode*n = ret->getChildByName(toBuffer);
        if (n == nullptr)
        {
            ret->children.push_back( std::make_unique<TreeNode>(TreeNode(toBuffer , ret)));
        }
        ret = ret->getChildByName(toBuffer);
    }
    
    
    
    return ret;
}

TreeNode*  DeviceTree::getNodeForPathAndCreateIfNeeded( const std::string &path , TreeNode* current)
{
    if (path == "\\")
        return &root;
    
    
    TreeNode*n = current->getChildByName(path);
    
    if (n == nullptr)
    {
        current->children.push_back( std::make_unique<TreeNode>(
                                                                TreeNode(path , current)
                                                                ));
        return current->getChildByName(path);
    }
    return n;
    
    /*
    std::istringstream fullPath((relativeTo == "\\"? "":relativeTo) + (path[0] == '.'?"":".") +  path);
    
    const std::string test = fullPath.str();
    
    printf("Resolve '%s' relative to '%s'\n" , path.c_str() , relativeTo.c_str());
    
    
    
    std::string s;
    while (getline(fullPath, s, '.'))
    {
        assert( s.size() < 5);
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
     */
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
AMLDecompilerInterface(decomp)
{
    decomp.parserPolicy.assertOnError = 1;
    decomp.parserPolicy.assertOnInvalidState = 1;
    //_scopeResolver.reset();
}

static void printDefBlock( const ACPIDefinitionBlock* defBlock)
{
    
    printf("tableSignature :'%s'\n" , defBlock->tableSignature);
    printf("OEMId :'%s'\n" , defBlock->OEMId);
    printf("tableId :'%s'\n" , defBlock->tableId);
    printf("tableLength :%d\n" , defBlock->tableLength);
    printf("complianceRevision :%hhu\n" , defBlock->complianceRevision);
    printf("tableCheckSum :%hhu\n" , defBlock->tableCheckSum);
    printf("OEMRev : %d\n" , defBlock->OEMRev);
    printf("creatorID : %d\n" , defBlock->OEMRev);
    
}

static void printDev(TreeNode* node ,int indent)
{
    for(int i=0;i<indent;i++)
    {
        printf("\t");
    }
    
    printf("TreeNode '%s' %zi names : \n" , node->id.c_str() , node->_names.size() );
    
    for(const auto &name : node->_names)
    {
        for(int i=0;i<indent+1;i++)
            printf("\t");
        
        if (name.id == "_HID")
        {
            if (name.type == ACPI::Type_NumericValue)
            {
                if (isEisaId(name.value64) )
                {
                    char eisaid[8] = {0};
                    assert(getEisaidString(name.value64, eisaid));
                    printf("%s:%s(EisaID)",name.id.c_str() ,eisaid);
                }
                else
                {
                    printf("%s:0x%llx",name.id.c_str() ,name.value64);
                }
            }
            else if (name.type == ACPI::Type_StringValue)
            {
                printf("%s:%s",name.id.c_str() ,name.valueStr.c_str());
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

ACPI::NameDeclaration* DeviceTreeBuilder::getCurrentName()
{
    return &(currentNode.top()->_names.back());
}


int DeviceTreeBuilder::startResourceTemplate( const ParserContext* context , size_t numItems )
{
    assert(getCurrentName());
    getCurrentName()->type = ACPI::ValueType::Type_RessourceTemplate;
    return 0;
}
int DeviceTreeBuilder::endResourceTemplate(const ParserContext* context , size_t numItemsParsed, AMLParserError err)
{
    return 0;
}

int DeviceTreeBuilder::onOperationRegion(const ParserContext* context, const ACPIOperationRegion& reg)
{
    
    assert(!currentNode.empty());
    currentNode.top()->_opRegions.push_back(reg);
    //currentNode->_opRegions.push_back(*reg);
    return 0;
}

int DeviceTreeBuilder::startField(const ParserContext* context, const ACPIField&field)
{
    char* fieldName = AMLNameConstructNormalized(&field.name);
    ACPI::FieldDeclaration decl;
    decl.name = fieldName;
    decl.flags = field.flags;
    currentNode.top()->_fields.push_back(decl);
    _currentFieldDecl = &currentNode.top()->_fields.back();
    free(fieldName);
    return 0;
}

int DeviceTreeBuilder::onFieldElement(const ParserContext* context, const ACPIFieldElement& fieldElement)
{
    assert(_currentFieldDecl);
    
    ACPI::FieldElement el;
    
    el.name = fieldElement.name;
    el.value = (uint8_t) fieldElement.value;
    _currentFieldDecl->elements.push_back(el);
    
    return 0;
}

int DeviceTreeBuilder::endField(const ParserContext* context, const ACPIField&)
{
    
    
    _currentFieldDecl  = nullptr;
    return 0;
}


int DeviceTreeBuilder::startIndexField(const ParserContext* context, const ACPIIndexField& indexField)
{
    char* fieldName = AMLNameConstructNormalized(&indexField.name);
    char* fieldDataName = AMLNameConstructNormalized(&indexField.dataName);
    ACPI::IndexFieldDeclaration decl;
    decl.name = fieldName;
    decl.dataName = fieldDataName;
    decl.flags = indexField.flags;
    currentNode.top()->_indexFields.push_back(decl);
    
    _currentIndexFieldDecl = &currentNode.top()->_indexFields.back();
    free(fieldName);
    free(fieldDataName);
    
    return 0;
}
int DeviceTreeBuilder::onIndexFieldElement(const ParserContext* context, const ACPIIndexFieldElement& fieldElement)
{
    assert(_currentIndexFieldDecl);
    
    ACPI::IndexFieldElement el;
    
    el.name = fieldElement.name;
    el.value = (uint8_t)fieldElement.value;
    _currentIndexFieldDecl->elements.push_back(el);
    
    return 0;
}

int DeviceTreeBuilder::endIndexField(const ParserContext* context, const ACPIIndexField&)
{
    _currentIndexFieldDecl = nullptr;
    return 0;
}

int DeviceTreeBuilder::onMethod(const ParserContext* context, const ACPIMethod& method)
{
    assert(!currentNode.empty());
    assert(method.name[4] == 0);
    currentNode.top()->_methods.push_back(method);
    return 0;
}



int DeviceTreeBuilder::startDevice(const ParserContext* context, const ACPIDevice& device)
{
    assert( device.obj.pos);
    currentNode.push( _deviceTree.getNodeForPathAndCreateIfNeeded(device.name , currentNode.top()) );
    return 0;
}

int DeviceTreeBuilder::endDevice(const ParserContext* context, const ACPIDevice& device)
{
    assert( device.obj.pos);
    assert(!currentNode.empty() );
    currentNode.pop();
    return 0;
}

int DeviceTreeBuilder::startScope(const ParserContext* context, const ACPIScope& scope)
{
    assert( scope.obj.pos);
    //char* scopeName = AMLNameConstructNormalized(&scope._name);
    //assert(scopeName);
    
    currentNode.push( _deviceTree.getNodeForPathAndCreateIfNeeded( scope.name , currentNode.top()));// scopeName,   _scopeResolver.top()) );
    
    
    //_scopeResolver.pushNamespace(  scopeName);
    assert(!currentNode.empty());

    //free(scopeName);
    return 0;
}

int DeviceTreeBuilder::endScope(const ParserContext* context, const ACPIScope& scope)
{
    assert( scope.obj.pos);
    //printf("End scope '%s' '%s'\n" , location , _scopes.back().c_str());
    //assert(!_scopeResolver.isEmpty());
    
    //_scopeResolver.pop();
    currentNode.pop();
    return 0;
}

int DeviceTreeBuilder::startName(const ParserContext* context, const char* name)
{
    assert(!currentNode.empty());
    
    currentNode.top()->_names.push_back(ACPI::NameDeclaration(name) );
    assert(getCurrentName());
    //currentName = &(currentNode.top()->_names.back());
    
    return 0;
}


/*
int DeviceTreeBuilder::EndName(const ParserContext* context, const char* name)
{
    return 0;
}
*/


int DeviceTreeBuilder::onACPIDefinitionBlock( const ParserContext* context, const ACPIDefinitionBlock& desc)
{
    _deviceTree.defBlock = &desc;
    return 0;
}

int DeviceTreeBuilder::startPackage( const ParserContext*context , const ACPIPackage& package)
{
    
    
    assert(getCurrentName());
    getCurrentName()->type = ACPI::ValueType::Type_Package;
    
    if (!currentPackage.empty() && currentPackage.top() == &getCurrentName()->_packageV)
    {
        ACPI::Package newPack;
        newPack.type = ACPI::Package::PackageType;
        getCurrentName()->_packageV._items.push_back(newPack );
        currentPackage.push( &getCurrentName()->_packageV._items.back());
    }
    else
    {
        currentPackage.push( &getCurrentName()->_packageV );
        
    }
    
    
    return 0;
}

int DeviceTreeBuilder::onPackageElement( const ParserContext* context , const ACPIPackageElement& element)
{
    assert(!currentPackage.empty());
    
    if(getCurrentName())
    {
        ACPI::Package item;
        
        switch (element.type)
        {
            case ACPIPackageElement::Integer:
                item.type = ACPI::Package::IntegerType;
                item.v = element.value.value;
                break;
                
            case ACPIPackageElement::String:
                item.type = ACPI::Package::StringType;
                item.strV = element.value.string;
                break;
                
            default:
                assert(false);
                break;
        }
        
        currentPackage.top()->_items.push_back(item);
        //getCurrentName()->addPackageItem(item);
    }
    
    return 0;
}

int DeviceTreeBuilder::endPackage( const ParserContext*context , const ACPIPackage& package)
{
    assert(!currentPackage.empty());
    currentPackage.pop();
    assert(getCurrentName()->type == ACPI::Type_Package);
    
    return 0;
}

int DeviceTreeBuilder::onBuffer(const ParserContext* context , size_t bufferSize , const uint8_t* buffer)
{
    getCurrentName()->setValue(buffer, bufferSize);
    return 0;
}

int DeviceTreeBuilder::onString(const ParserContext* context, const char* str)
{
    getCurrentName()->setValue(str);
    
    assert(getCurrentName()->type== ACPI::Type_StringValue);
    return 0;
}

int DeviceTreeBuilder::onValue(const ParserContext* context, uint64_t value)
{

    if(getCurrentName())
    {
        getCurrentName()->setValue(value);
        
        getCurrentName()->isEisaid = isEisaId(value);
        
    }
    return 0;
}

int DeviceTreeBuilder::onQWORDAddressSpaceDescriptor( const ParserContext* context , const QWordAddressSpaceDescriptor& desc)
{
    assert(getCurrentName());

    ACPI::RessourceItem item;
    item.setValue(desc);
    getCurrentName()->addTemplateItem(item);

    return 0;
}


int DeviceTreeBuilder::onIOPortDescriptor( const ParserContext* context , const IOPortDescriptor&desc)
{
    assert(getCurrentName());

    ACPI::RessourceItem item;
    item.setValue(desc);
    getCurrentName()->addTemplateItem(item);

    return 0;
}

int DeviceTreeBuilder::onIRQFormatDescriptor( const ParserContext* context , const IRQDescriptor&desc)
{
    assert(getCurrentName());
    
    ACPI::RessourceItem item;
    item.setValue(desc);
    getCurrentName()->addTemplateItem(item);
    
    return 0;
}


int DeviceTreeBuilder::onMemoryRangeDescriptor32( const ParserContext* context , const MemoryRangeDescriptor32& desc)
{
    assert(getCurrentName());

    ACPI::RessourceItem item;
    item.setValue(desc);
    getCurrentName()->addTemplateItem(item);

    return 0;
}

int DeviceTreeBuilder::onDWORDAddressSpaceDescriptor( const ParserContext* context , const DWordAddressSpaceDescriptor& desc)
{
    assert(getCurrentName());

    ACPI::RessourceItem item;
    item.setValue(desc);
    getCurrentName()->addTemplateItem(item);

    return 0;
}

int DeviceTreeBuilder::onCreateField(const ParserContext* contextn , const ACPICreateFieldBase *_field)
{
    ACPI::CreateField f;
    f.type = _field->type;
    
    switch (_field->type)
    {
        case ACPICreateFieldBase::CreateByteField:
        {
            const ACPICreateByteField* field = reinterpret_cast<const ACPICreateByteField*>(_field);
            assert(field);
             
            f.nameSource = field->base.nameSource;
            f.nameString = field->base.nameString;
            
            f.v1 = field->byteIndex;
        }
            break;
        case ACPICreateFieldBase::CreateWordField:
        {
            const ACPICreateWordField* field = reinterpret_cast<const ACPICreateWordField*>(_field);
            assert(field);
            
            f.nameSource = field->base.nameSource;
            f.nameString = field->base.nameString;
            
            f.v1 = field->byteIndex;
            
        }
            break;
        case ACPICreateFieldBase::CreateField:
        {
            const ACPICreateField* field = reinterpret_cast<const ACPICreateField*>(_field);
            assert(field);
            
            f.nameSource = field->base.nameSource;
            f.nameString = field->base.nameString;
            
            f.v1 = field->byteIndex;
            f.v2 = field->numBytes;
        }
            break;
            
        default:
            assert(false);
            break;
    }
    
    currentNode.top()->_createField.push_back(f);
    
    return 0;
}

int DeviceTreeBuilder::onWORDAddressSpaceDescriptor( const ParserContext* context , const WordAddressSpaceDescriptor& desc)
{
    assert(getCurrentName());

    ACPI::RessourceItem item;
    item.setValue(desc);
    getCurrentName()->addTemplateItem(item);

    return 0;
}

AMLParserError DeviceTreeBuilder::start(const uint8_t* buffer , size_t size)
{
    
    currentNode.push( &_deviceTree.root );
    
    return AMLDecompilerStart(&decomp, buffer, size);
}
