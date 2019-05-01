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

#include "JSONConverter.hpp"
#include "DeviceTree.hpp"


static bool isString(const std::vector<uint8_t> &bytes)
{
    if (bytes.empty())
        return false;
    
    if (bytes.at(bytes.size()-1) != 0)
        return false;
    
    for(size_t i = 0;i< bytes.size()-1;i++)
    {
        if (isprint(  bytes[i])  == 0)
        {
            return false;
        }
    }
    
    return true;
}

JSONConverter::JSONConverter( const DeviceTree &tree ):
tree(tree)
{}

static nlohmann::json serializeIOPortDescriptor( const IOPortDescriptor &desc)
{
    nlohmann::json ret;
    
    ret["isDecoder"]        = desc.isDecoder;
    ret["rangeMinBaseAddr"] = desc.rangeMinBaseAddr;
    ret["rangeMaxBaseAddr"] = desc.rangeMaxBaseAddr;
    ret["baseAlign"]        = desc.baseAlign;
    ret["rangeLen"]         = desc.rangeLen;
    
    return ret;
}
/*
static nlohmann::json serializeResourceType0Flags(const ResourceType0Flags& desc)
{
    nlohmann::json ret;
    
    
    ret["TTP"] = desc.TTP;
    ret["MTP"] = desc.MTP;
    ret["MEM"] = desc.MEM;
    ret["RW"] = desc.RW;
    
    return ret;
}
*/

static nlohmann::json serializeQWordAddressSpaceDescriptor(const QWordAddressSpaceDescriptor& desc)
{
    nlohmann::json ret;
    
    ret["ressourceType"]         = desc.resourceType;
    ret["generalFlags"]          = desc.generalFlags;
    ret["typeSpecificFlags"]     = desc.typeSpecificFlags;
    ret["addrSpaceGranularity"]  = desc.addrSpaceGranularity;
    ret["addrRangeMin"]          = desc.addrRangeMin;
    ret["addrRangeMax"]          = desc.addrRangeMax;
    ret["addrTranslationOffset"] = desc.addrTranslationOffset;
    ret["addrTranslationLength"] = desc.addrTranslationLength;
    
    return ret;
}

static nlohmann::json serializeDWordAddressSpaceDescriptor(const DWordAddressSpaceDescriptor& desc)
{
    nlohmann::json ret;
    
    
    ret["ressourceType"] = desc.resourceType;
    ret["generalFlags"] = desc.generalFlags;
    ret["typeSpecificFlags"] = desc.typeSpecificFlags;
    ret["addrSpaceGranularity"] = desc.addrSpaceGranularity;
    ret["addrRangeMin"] = desc.addrRangeMin;
    ret["addrRangeMax"] = desc.addrRangeMax;
    ret["addrTranslationOffset"] = desc.addrTranslationOffset;
    ret["addrTranslationLength"] = desc.addrTranslationLength;
    
    return ret;
}

static nlohmann::json serializeMemoryRangeDescriptor32(const MemoryRangeDescriptor32& desc)
{
    nlohmann::json ret;
    
    ret["rangeBaseAddr"] = desc.rangeBaseAddr;
    ret["rangeLength"] = desc.rangeLength;
    ret["writeStatus"] = desc.writeStatus;
    
    return ret;
}
static nlohmann::json serializeWordAddressSpaceDescriptor(const WordAddressSpaceDescriptor& desc)
{
    nlohmann::json ret;
    
    
    ret["ressourceType"] = desc.ressourceType;
    ret["generalFlags"] = desc.generalFlags;
    ret["typeSpecificFlags"] = desc.typeSpecificFlags;
    ret["ressourceSourceIndex"] = desc.ressourceSourceIndex;
    ret["addrSpaceGranularity"] = desc.addrSpaceGranularity;
    ret["addrRangeMin"] = desc.addrRangeMin;
    ret["addrRangeMax"] = desc.addrRangeMax;
    ret["addrTranslationOffset"] = desc.addrTranslationOffset;
    ret["addrTranslationLength"] = desc.addrTranslationLength;
    
    return ret;
}

static nlohmann::json serializePackageItem( const ACPI::Package & packageItem)
{
    nlohmann::json res;
    res["type"] = packageItem.type;
    
    switch (packageItem.type)
    {
        case ACPI::Package::IntegerType:
            res["value"] = packageItem.v;
            break;
        case ACPI::Package::StringType:
            res["value"] = packageItem.strV;
            break;
            
        default:
            assert(false);
            break;
    }
    
    
    return res;
}

static nlohmann::json serializePackage( const ACPI::Package & package)
{
    nlohmann::json res;
    
    for( const auto &item : package._items)
    {
        res["value"].push_back( serializePackageItem(item));
    }
    
    return res;
}

static nlohmann::json serializeResourseTemplate( const ACPI::ResourceTemplate&resTemplate)
{
    nlohmann::json res;
    
    
    for( const auto& item : resTemplate.items)
    {
        nlohmann::json it;
        it["type"] = item.type;
        
        switch (item.type)
        {
            case ACPI::Type_WordAddressSpaceDescriptor:
                it["value"] = serializeWordAddressSpaceDescriptor(item.value.wordAddressSpaceDescriptor);
                break;
                
            case ACPI::Type_IOPortDescriptor:
                it["value"] = serializeIOPortDescriptor(item.value.ioPortDescriptor);
                break;
                
            case ACPI::Type_DWordAddressSpaceDescriptor:
                it["value"] = serializeDWordAddressSpaceDescriptor(item.value.dwordAddressSpaceDescriptor);
                break;
                
                
            case ACPI::Type_QWordAddressSpaceDescriptor:
                it["value"] = serializeQWordAddressSpaceDescriptor(item.value.qwordAddressSpaceDescriptor);
                break;
            
            case ACPI::Type_MemoryRangeDescriptor32:
                it["value"] = serializeMemoryRangeDescriptor32(item.value.memoryRangeDesc32);
                break;
            default:
                assert(0);
                break;
        }
        
        res.push_back(it);
    }
    return res;
}
static nlohmann::json serializeBuffer( const ACPI::NameDeclaration&name)
{
    nlohmann::json ret;
    
    if (isString(name.rawBuffer))
    {
        ret = std::string( reinterpret_cast<const char*>( name.rawBuffer.data() )  );
    }
    else
    {
        ret["buf"]  = name.rawBuffer;
        ret["size"] = name.rawBufferSize;
    }
    
    return ret;
}
static nlohmann::json serializeName( const ACPI::NameDeclaration&name)
{
    nlohmann::json res;
    
    res["type"] = name.type;
    switch (name.type)
    {
        case ACPI::ValueType::Type_NumericValue:
            assert(name.valueStr.empty());
            res["value"] = name.value64;
            res["eisaid"] = name.isEisaid;
            break;
            
        case ACPI::ValueType::Type_Buffer:
            
            res["value"] = serializeBuffer(name);
            break;
            
        case ACPI::ValueType::Type_RessourceTemplate:
        {
            res["value"] = serializeResourseTemplate( name.resTemplate );
            
        }
            break;
        
        case ACPI::ValueType::Type_Package:
        {
            res["value"] = serializePackage( name._packageV);
        }
            break;
        case ACPI::ValueType::Type_StringValue:
            res["value"] = name.valueStr;
            break;
        default:
            
            assert(false);
            break;
    }
    
    
    return res;
}

static void serializeFielFlags( const ACPIFieldFlags& flags ,nlohmann::json &toObject )
{
    auto updateRuleDesc = [](uint8_t updateRule) -> std::string
    {
        switch (updateRule)
        {
            case 0:
                return "Preserve";
            case 1:
                return "WriteAsOnes";
            case 2:
                return "WriteAsZeros";
                
            default:
                break;
        }
        assert(false);
        return "";
    };
    
    auto accessTypeDesc = [](uint8_t accessType) -> std::string
    {
        switch(accessType)
        {
            case 0:
                return "AnyAcc";
            case 1:
                return "ByteAcc";
            case 2:
                return "WordAcc";
            case 3:
                return "DWordAcc";
            case 4:
                return "QWordAcc";
            case 5:
                return "BufferAcc";
                /*
                 case 6:
                 return "Reserved";
                 */
        }
        
        //assert(false);
        return "Reserved";
    };
    
    toObject["accessType"] = accessTypeDesc(flags.accessType);
    toObject["lockRule"] =   flags.lockRule? "Lock":"NoLock";
    toObject["updateRule"] = updateRuleDesc( flags.updateRule);
}
static nlohmann::json serializeIndexFields( const ACPI::IndexFieldDeclaration &field)
{
    nlohmann::json ret;
    ret["name"] = field.name;
    ret["dataName"] = field.dataName;
    serializeFielFlags(field.flags, ret);
    
    for( const auto &el : field.elements)
    {
        
        nlohmann::json ell;
        ell["value"] = el.value;
        ell["name"]  = el.name;
        
        
        ret["content"].push_back(ell);
    }
    
    return ret;
}
static nlohmann::json serializeFields( const ACPI::FieldDeclaration &field)
{
    nlohmann::json ret;
    ret["name"] = field.name;
    serializeFielFlags(field.flags, ret);
    for( const auto &el : field.elements)
    {
        
        nlohmann::json ell;
        ell["value"] = el.value;
        ell["name"]  = el.name;
        
        
        ret["content"].push_back(ell);
        /*
        ret[field.valueName]["accessType"] = el.accessType;
        ret[field.valueName]["lockRule"] = el.lockRule;
        ret[field.valueName]["updateRule"] = el.updateRule;
        
        if (field.offset)
        {
            ret[field.valueName]["offset"] = (int) field.offset;
        }
         */
    }
    
    return ret;
}
static nlohmann::json serializeOperationRegion( const ACPIOperationRegion&reg)
{
    nlohmann::json ret;

    ret["space"] = reg.space;
    ret["offset"] = reg.offset;
    ret["length"] = reg.length;
    return ret;
}

static nlohmann::json serializeCreateField( const ACPI::CreateField &createField)
{
    nlohmann::json ret;
    
    ret["nameSource"] = createField.nameSource;
    ret["nameString"] = createField.nameString;
    ret["type"]       = createField.type;
    ret["v1"]         = createField.v1;
    ret["v2"]         = createField.v2;
    
    return ret;
}
static nlohmann::json serializeMethod( const ACPIMethod& method)
{
    nlohmann::json ret;
    
    ret["argCount"] = method.argCount;
    ret["syncLevel"] = method.syncLevel;
    ret["serializeFlag"] = method.serializeFlag;
    
    if (method.bodyDef != nullptr )
    {
        ret["body"] ="some";
    }
    else
    {
        ret["body"] = nullptr;
    }
    
    return ret;
}

static nlohmann::json serializeNode( const TreeNode& node)
{
    nlohmann::json res;
    
    for( const auto &name : node._names)
    {
        assert(!name.id.empty());
        res[name.id] = serializeName(name);
    }
    
    for( const auto &child : node.children)
    {
        res["Children"][child.get()->id] = serializeNode(*child);
    }
    
    for(const auto &opReg : node._opRegions)
    {
        res["OperationRegions"][opReg.name] = serializeOperationRegion(opReg);
    }
    
    for( const auto &field : node._fields)
    {
        res["Fields"].push_back( serializeFields(field) );
    }
    
    for( const auto &field : node._indexFields)
    {
        res["IndexFields"].push_back( serializeIndexFields(field) );
    }
    
    for( const auto &method : node._methods)
    {
        res["Methods"][method.name] = serializeMethod(method);
    }
    
    for( const auto &createField : node._createField)
    {
        res["CreateFields"].push_back( serializeCreateField(createField));
    }
    
    return res;
}


static nlohmann::json serializeDefBlock( const ACPIDefinitionBlock&block)
{
    nlohmann::json res;
    
    if (block.OEMId[0] && block.tableId[0] && block.tableSignature[0])
    {        
        res["tableSignature"] = block.tableSignature;
//        res["tableLength"] = block.tableLength;
        res["complianceRevision"] = block.complianceRevision;
//        res["tableCheckSum"] = block.tableCheckSum;
        res["OEMId"] = block.OEMId;
        res["tableId"] = block.tableId;
        res["OEMRev"] = block.OEMRev;
        res["creatorID"] = block.creatorID;
        
    }
    return res;
}

nlohmann::json JSONConverter::getJSON() const
{
    nlohmann::json jsonResult;
    
    
    jsonResult["tree"] = serializeNode(tree.root);
    jsonResult["def"] = serializeDefBlock(tree.defBlock);
    
    return jsonResult;
    //return serializeNode(jsonResult,node);
}
