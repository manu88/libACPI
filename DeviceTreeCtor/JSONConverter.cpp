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


JSONConverter::JSONConverter( const DeviceTree &tree ):
tree(tree)
{}

static nlohmann::json serializeName( const NameDeclaration&name)
{
    nlohmann::json res;
    
    res["type"] = name.type;
    switch (name.type)
    {
        case NameDeclaration::Type_NumericValue:
            res["value"] = name.value.value64;
            res["eisaid"] = name.isEisaid;
            break;
            
        case NameDeclaration::Type_MemoryRangeDescriptor32:
            res["value"]["rangeBaseAddr"]   = name.value.memoryRangeDesc32.rangeBaseAddr;
            res["value"]["rangeLength"]     = name.value.memoryRangeDesc32.rangeLength;
            res["value"]["writeStatus"]     = name.value.memoryRangeDesc32.writeStatus;
            
            break;
            
        case NameDeclaration::Type_AddressSpaceDescriptor:
            
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
            
        case NameDeclaration::Type_WordAddressSpaceDescriptor:
            
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
            
        default:
            
            assert(false);
            break;
    }
    
    
    return res;
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
