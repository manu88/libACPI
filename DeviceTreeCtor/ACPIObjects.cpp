//
//  ACPIObjects.cpp
//  DeviceTreeCtor
//
//  Created by Manuel Deneu on 24/03/2019.
//  Copyright © 2019 Manuel Deneu. All rights reserved.
//
#include <stdio.h>
#include "ACPIObjects.hpp"

/*
ACPI::NameDeclaration::NameDeclaration( const NameDeclaration& other):
id(other.id),
type(other.type),
value64(other.value64),
valueStr(other.valueStr),
rawBuffer(other.rawBuffer),
rawBufferSize(other.rawBufferSize)
{
    
    if (!valueStr.empty())
    {
        
    }
}

ACPI::NameDeclaration& ACPI::NameDeclaration::operator=(const NameDeclaration &other)
{
    
    id = other.id;
    type = other.type;
    value64 = other.value64;
    valueStr = other.valueStr;
    
    rawBufferSize = other.rawBufferSize;
    rawBuffer = other.rawBuffer;
    return *this;
}
*/

ACPI::NameDeclaration::NameDeclaration(const std::string& n):
id(n)
{
    
}


void ACPI::NameDeclaration::setValue( const uint8_t* buffer , size_t bufSize)
{
    type = ACPI::Type_Buffer;
    
    rawBufferSize = bufSize;
    rawBuffer.clear();
    
    if (buffer) // a buffer can be empty
    {
        for( size_t i=0;i<bufSize;i++)
        {
            rawBuffer.push_back(buffer[i]);
        }
    }
}

void ACPI::NameDeclaration::setValue( const std::string &str)
{
    type= ACPI::Type_StringValue;
    valueStr = str;
}

void ACPI::NameDeclaration::setValue( uint64_t val)
{
    type = ACPI::Type_NumericValue;
    value64 = val;
}
void ACPI::NameDeclaration::addPackageItem( const Package& item)
{
    assert(type == ACPI::Type_Package);
    
    _packageV._items.push_back(item);
}

void ACPI::NameDeclaration::addTemplateItem( const RessourceItem& item)
{
    assert(type == ACPI::Type_RessourceTemplate);
    
    resTemplate.items.push_back(item);
}
