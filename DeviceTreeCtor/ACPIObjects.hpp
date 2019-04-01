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

#include <ACPIDesc.h>
#include <vector>
#include <string>

namespace ACPI
{
    enum ValueType
    {
        Type_NumericValue               = 0,
        Type_Buffer                     = 1,
        Type_StringValue                = 2,
        Type_RessourceTemplate          = 10,
        //Type_AddressSpaceDescriptor     = 11,
        Type_MemoryRangeDescriptor32    = 12,
        Type_WordAddressSpaceDescriptor = 13,
        Type_IOPortDescriptor,
        Type_DWordAddressSpaceDescriptor,
        Type_QWordAddressSpaceDescriptor,
        
        //Type_OperationRegion
        Type_Invalid = 300,
    };
    
    struct RessourceItem
    {
        void setValue(const QWordAddressSpaceDescriptor& desc)
        {
            type = ACPI::Type_QWordAddressSpaceDescriptor;
            value.qwordAddressSpaceDescriptor = desc;
        }
        
        void setValue(const MemoryRangeDescriptor32 &desc)
        {
            type = ACPI::Type_MemoryRangeDescriptor32;
            value.memoryRangeDesc32 = desc;
        }
        
        void setValue(const WordAddressSpaceDescriptor &desc)
        {
            type = ACPI::Type_WordAddressSpaceDescriptor;
            value.wordAddressSpaceDescriptor = desc;
        }
        
        void setValue(const DWordAddressSpaceDescriptor &desc)
        {
            type = ACPI::Type_DWordAddressSpaceDescriptor;
            value.dwordAddressSpaceDescriptor = desc;
        }
        
        void setValue(const IOPortDescriptor& desc)
        {
            type = ACPI::Type_IOPortDescriptor;
            value.ioPortDescriptor = desc;
        }
        
        union
        {
            MemoryRangeDescriptor32     memoryRangeDesc32;
            AddressSpaceDescriptor      addressSpaceDescriptor;
            WordAddressSpaceDescriptor  wordAddressSpaceDescriptor;
            IOPortDescriptor            ioPortDescriptor;
            DWordAddressSpaceDescriptor dwordAddressSpaceDescriptor;
            QWordAddressSpaceDescriptor qwordAddressSpaceDescriptor;
            
        } value;
        
        ACPI::ValueType type = ACPI::Type_Invalid;
    };
    
    struct ResourceTemplate
    {
        std::vector<RessourceItem> items;
    };
    
    
    struct NameDeclaration
    {
        
        /*
        NameDeclaration( const NameDeclaration& other);
        NameDeclaration& operator=(const NameDeclaration &other);
         */
        explicit NameDeclaration(const std::string& n);
        
        
        std::string id;
        bool isEisaid = false;
        
        void setValue( const uint8_t* buffer , size_t bufSize);
        
        void setValue( const std::string &str);
        
        void setValue( uint64_t val);
        
        void addTemplateItem( const RessourceItem& item);
        
        ACPI::ValueType type = ACPI::Type_Invalid;
        
        
        uint64_t value64 = 0;
        ResourceTemplate resTemplate;   // type = Type_RessourceTemplate
        std::vector<uint8_t> rawBuffer; // type = Type_Buffer
        size_t rawBufferSize = 0;
        std::string valueStr;
    };
    
    struct FieldElement
    {
        std::string name;
        uint8_t value;
    };
    struct FieldDeclaration
    {
        std::string name;
        uint8_t accessType:4;
        uint8_t lockRule:1; /* 0 NoLock 1 Lock */
        uint8_t updateRule:2; /*0 Preserve 1 WriteAsOnes 2 WriteAsZeros */
        std::vector<FieldElement> elements;
    };
    
    
    struct IndexFieldElement
    {
        std::string name;
        uint8_t value;
    };
    
    struct IndexFieldDeclaration
    {
        std::string name;
        std::string dataName;
        uint8_t accessType:4;
        uint8_t lockRule:1; /* 0 NoLock 1 Lock */
        uint8_t updateRule:2; /*0 Preserve 1 WriteAsOnes 2 WriteAsZeros */
        std::vector<IndexFieldElement> elements;
    };
    
}
