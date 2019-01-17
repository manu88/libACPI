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

#include <string>
#include <sstream>

#include "AMLDecompilerInterface.h"

class DeviceTreeBuilder : public AMLDecompilerInterface
{
public:
    DeviceTreeBuilder();

    int onQWORDAddressSpaceDescriptor( const ParserContext* context , const AddressSpaceDescriptor* desc);
    
    int onMemoryRangeDescriptor32( const ParserContext* context , const MemoryRangeDescriptor32* desc);
    
    AMLParserError start(const uint8_t* buffer , size_t size);
    
    std::string getResult() const
    {
        return str.str();
    }
    
    void indent();
    void incScope();

    
    void decScope();
    
private:
    AMLDecompiler decomp;
    
    
    std::stringstream str;
    int currentScope = 0;
    
    
};


