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
#include "DeviceTreeBuilder.hpp"


static bool isString(const std::vector<uint8_t> bytes)
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
    
    
    ret["isDecoder"] = desc.isDecoder;
    ret["rangeMinBaseAddr"] = desc.rangeMinBaseAddr;
    ret["rangeMaxBaseAddr"] = desc.rangeMaxBaseAddr;
    ret["baseAlign"] = desc.baseAlign;
    ret["rangeLen"] = desc.rangeLen;
    
    return ret;
}

static nlohmann::json serializeQWordAddressSpaceDescriptor(const QWordAddressSpaceDescriptor& desc)
{
    nlohmann::json ret;
    
    
    ret["ressourceType"] = desc.ressourceType;
    ret["generalFlags"] = desc.generalFlags;
    ret["typeSpecificFlags"] = desc.typeSpecificFlags;
    ret["addrSpaceGranularity"] = desc.addrSpaceGranularity;
    ret["addrRangeMin"] = desc.addrRangeMin;
    ret["addrRangeMax"] = desc.addrRangeMax;
    ret["addrTranslationOffset"] = desc.addrTranslationOffset;
    ret["addrTranslationLength"] = desc.addrTranslationLength;
    
    return ret;
}

static nlohmann::json serializeDWordAddressSpaceDescriptor(const DWordAddressSpaceDescriptor& desc)
{
    nlohmann::json ret;
    
    
    ret["ressourceType"] = desc.ressourceType;
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

static nlohmann::json serializeResourseTemplate( const ResourceTemplate&resTemplate)
{
    nlohmann::json res;
    
    
    for( const auto& item : resTemplate.items)
    {
        nlohmann::json it;
        it["type"] = item.type;
        
        switch (item.type)
        {
            case Type_WordAddressSpaceDescriptor:
                it["value"] = serializeWordAddressSpaceDescriptor(item.value.wordAddressSpaceDescriptor);
                break;
                
            case Type_IOPortDescriptor:
                it["value"] = serializeIOPortDescriptor(item.value.ioPortDescriptor);
                break;
                
            case Type_DWordAddressSpaceDescriptor:
                it["value"] = serializeDWordAddressSpaceDescriptor(item.value.dwordAddressSpaceDescriptor);
                break;
                
                
            case Type_QWordAddressSpaceDescriptor:
                it["value"] = serializeQWordAddressSpaceDescriptor(item.value.qwordAddressSpaceDescriptor);
                break;
            case Type_MemoryRangeDescriptor32:
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
static nlohmann::json serializeName( const NameDeclaration&name)
{
    nlohmann::json res;
    
    res["type"] = name.type;
    switch (name.type)
    {
        case ValueType::Type_NumericValue:
            res["value"] = name.value64;
            res["eisaid"] = name.isEisaid;
            break;
            
            /*
        case ValueType::Type_MemoryRangeDescriptor32:
            res["value"]["rangeBaseAddr"]   = name.value.memoryRangeDesc32.rangeBaseAddr;
            res["value"]["rangeLength"]     = name.value.memoryRangeDesc32.rangeLength;
            res["value"]["writeStatus"]     = name.value.memoryRangeDesc32.writeStatus;
            
            break;
            
        case ValueType::Type_AddressSpaceDescriptor:
            
            res["value"]["ressourceType"]         = name.value.addressSpaceDescriptor.ressourceType;
            res["value"]["typeSpecificFlags"]     = name.value.addressSpaceDescriptor.typeSpecificFlags;
            res["value"]["addrSpaceGranularity"]  = name.value.addressSpaceDescriptor.addrSpaceGranularity;
            res["value"]["addrRangeMin"]          = name.value.addressSpaceDescriptor.addrRangeMin;
            res["value"]["addrRangeMax"]          = name.value.addressSpaceDescriptor.addrRangeMax;
            res["value"]["addrTranslationOffset"] = name.value.addressSpaceDescriptor.addrTranslationOffset;
            res["value"]["addrTranslationLength"] = name.value.addressSpaceDescriptor.addrTranslationLength;
            res["value"]["ressourceSourceIndex"]  = name.value.addressSpaceDescriptor.ressourceSourceIndex;
            res["value"]["ressourceSource"]       = name.value.addressSpaceDescriptor.ressourceSource;
            res["value"]["maf"]                   = name.value.addressSpaceDescriptor.maf;
            res["value"]["mif"]                   = name.value.addressSpaceDescriptor.mif;
            res["value"]["decodeType"]            = name.value.addressSpaceDescriptor.decodeType;
            res["value"]["isConsumer"]            = name.value.addressSpaceDescriptor.isConsumer;
            
            
            
            break;
            
        case ValueType::Type_WordAddressSpaceDescriptor:
            
            res["value"]["ressourceType"]          = name.value.wordAddressSpaceDescriptor.ressourceType;
            res["value"]["generalFlags"]           = name.value.wordAddressSpaceDescriptor.generalFlags;
            res["value"]["typeSpecificFlags"]      = name.value.wordAddressSpaceDescriptor.typeSpecificFlags;
            res["value"]["addrSpaceGranularity"]   = name.value.wordAddressSpaceDescriptor.addrSpaceGranularity;
            res["value"]["addrRangeMin"]           = name.value.wordAddressSpaceDescriptor.addrRangeMin;
            res["value"]["addrRangeMax"]           = name.value.wordAddressSpaceDescriptor.addrRangeMax;
            res["value"]["addrTranslationOffset"]  = name.value.wordAddressSpaceDescriptor.addrTranslationOffset;
            res["value"]["addrTranslationLength"]  = name.value.wordAddressSpaceDescriptor.addrTranslationLength;
            res["value"]["ressourceSourceIndex"]   = name.value.wordAddressSpaceDescriptor.ressourceSourceIndex;
            break;
            */
        case ValueType::Type_Buffer:
            
            if (isString(name.rawBuffer))
            {
                res["value"] = std::string( reinterpret_cast<const char*>( name.rawBuffer.data() )  );
            }
            else
            {
                res["value"] = name.rawBuffer;
            }
            
            
            break;
            
        case ValueType::Type_RessourceTemplate:
        {
            res["value"] = serializeResourseTemplate( name.resTemplate );
            
        }
            break;
        default:
            
            assert(false);
            break;
    }
    
    
    return res;
}
static nlohmann::json serializeField( const ACPIField&field)
{
    nlohmann::json ret;
    
    ret["accessType"] = field.accessType;
    ret["lockRule"] = field.lockRule;
    ret["updateRule"] = field.updateRule;
    
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

static nlohmann::json serializeNode( const TreeNode&node)
{
    nlohmann::json res;
    
    for( const auto &name : node._names)
    {
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
        res["Fields"][field.name] = serializeField(field);
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
