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


DeviceTreeBuilder::DeviceTreeBuilder()
{
    AMLDecompilerInit(&decomp);
    
    decomp.parserPolicy.assertOnError = 1;
    
    decomp.userData = this;
    
    decomp.callbacks.OnDefinitionBlock = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIDefinitionBlock* desc)-> int
    {
        DeviceTreeBuilder* self = reinterpret_cast<DeviceTreeBuilder*>(_decomp->userData);
        
        
        self->str << "DefinitionBlock (\"\",";
        self->str << std::string(",\"")+ desc->tableSignature + "\",";
        self->str << std::to_string(desc->complianceRevision) + ",";
        self->str << std::string("\"")+ desc->OEMId + "\",";
        self->str << std::string("\"")+ desc->tableId + "\",";
        self->str << std::to_string(desc->OEMRev) + ")";
        self->str << "\n{\n";
        
        
        
        return 0;
    };
    
    
    decomp.callbacks.OnBuffer = [](AMLDecompiler* _decomp , const ParserContext* ctx , size_t bufferSize , const uint8_t* buffer) -> int
    {
        DeviceTreeBuilder* self = reinterpret_cast<DeviceTreeBuilder*>(_decomp->userData);
        
        
        self->str << ",Buffer() {";
        
        for(int i=0;i<bufferSize;i++)
        {
            self->str << "0x"<< std::hex << ((unsigned int) buffer[i]) << ",";
            
        }
        self->str << "}";
        
        self->str << "\n";
        
        return 0;
    };
    
    decomp.callbacks.StartDevice = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIDevice* device) -> int
    {
        DeviceTreeBuilder* self = reinterpret_cast<DeviceTreeBuilder*>(_decomp->userData);
        
        self->incScope();
        
        self->indent();
        
        self->str << std::string("Device(") + std::string(device->name) + ")";
        
        self->str << "\n";
        self->indent();
        self->str << "{\n";
        self->incScope();
        //            printf("STR '%s'\n" , self->str.str().c_str());
        return 0;
    };
    
    decomp.callbacks.EndDevice = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIDevice* dev) -> int
    {
        DeviceTreeBuilder* self = reinterpret_cast<DeviceTreeBuilder*>(_decomp->userData);
        
        self->decScope();
        self->str << "\n";
        self->indent();
        self->str << "}\n";
        
        
        self->decScope();
        //            printf("STR '%s'\n" , self->str.str().c_str());
        
        return 0;
    };
    
    decomp.callbacks.StartName = []( AMLDecompiler* _decomp ,const ParserContext* context, const char* name) -> int
    {
        DeviceTreeBuilder* self = reinterpret_cast<DeviceTreeBuilder*>(_decomp->userData);
        
        
        self->indent();
        self->str << std::string("Name( ") + std::string(name,4)  << "\n";
        
        
        
        //self->decScope();
        
        return 0;
    };
    
    decomp.callbacks.EndName = []( AMLDecompiler* _decomp ,const ParserContext* context, const char* name) -> int
    {
        DeviceTreeBuilder* self = reinterpret_cast<DeviceTreeBuilder*>(_decomp->userData);
        
        
        self->indent();
        self->str << ")" << "\n";
        
        //self->decScope();
        
        return 0;
    };
    
    decomp.callbacks.StartScope = []( AMLDecompiler* _decomp ,const ParserContext* context, const char* location) -> int
    {
        DeviceTreeBuilder* self = reinterpret_cast<DeviceTreeBuilder*>(_decomp->userData);
        
        self->incScope();
        
        self->indent();
        self->str << std::string("Scope(") + /*std::string(*/location/*,4)*/ + ")" + "\n";
        
        self->indent();
        self->str << "{\n"  ;
        
        //            printf("STR '%s'\n" , self->str.str().c_str());
        
        return 0;
    };
    decomp.callbacks.EndScope = []( AMLDecompiler* _decomp ,const ParserContext* context, const char* location) -> int
    {
        DeviceTreeBuilder* self = reinterpret_cast<DeviceTreeBuilder*>(_decomp->userData);
        
        self->indent();
        self->str << "}\n";
        
        self->decScope();
        return 0;
    };
    
    
    decomp.callbacks.OnValue = []( AMLDecompiler* _decomp ,const ParserContext* context, uint64_t val) -> int
    {
        DeviceTreeBuilder* self = reinterpret_cast<DeviceTreeBuilder*>(_decomp->userData);
        
        self->indent();
        self->str << " 0x" <<  std::hex << val << " ";
        return 0;
    };
    
    
    
    decomp.callbacks.onOperationRegion = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIOperationRegion* reg) -> int
    {
        DeviceTreeBuilder* self = reinterpret_cast<DeviceTreeBuilder*>(_decomp->userData);
        
        self->incScope();
        self->indent();
        
        self->str << std::string("OperationRegion(")
        << reg->name << ","
        << "0x" << std::hex << reg->offset << ","
        << std::to_string( reg->length) << ")"
        << "\n";
        
        self->decScope();
        //            printf("STR '%s'\n" , self->str.str().c_str());
        
        return 0;
    };
    
    decomp.callbacks.onField = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIField* field) -> int
    {
        DeviceTreeBuilder* self = reinterpret_cast<DeviceTreeBuilder*>(_decomp->userData);
        
        self->incScope();
        self->indent();
        
        self->str << std::string("Field(")
        << std::string(field->name)
        /*
         << reg->name << ","
         << "0x" << std::hex << reg->offset << ","
         << std::to_string( reg->length) */<< ")"
        << "\n";
        
        self->decScope();
        //            printf("STR '%s'\n" , self->str.str().c_str());
        
        return 0;
    };
    
    decomp.callbacks.startMethod = []( AMLDecompiler* _decomp ,const ParserContext* context, const char* location) -> int
    {
        DeviceTreeBuilder* self = reinterpret_cast<DeviceTreeBuilder*>(_decomp->userData);
        
        self->incScope();
        self->indent();
        
        self->str << std::string("Method(")
        /*
         << reg->name << ","
         << "0x" << std::hex << reg->offset << ","
         << std::to_string( reg->length) */<< ")"
        << "\n";
        
        self->indent();
        self->str << "{\n";
        return 0;
    };
    
    decomp.callbacks.endMethod = []( AMLDecompiler* _decomp ,const ParserContext* context, const char* location) -> int
    {
        DeviceTreeBuilder* self = reinterpret_cast<DeviceTreeBuilder*>(_decomp->userData);
        self->indent();
        self->str << "}\n";
        self->decScope();
        return 0;
    };
    
    decomp.callbacks.onSmallItem = [] (AMLDecompiler*_decomp,const ParserContext* context,
                                       SmallResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize) -> int
    {
        return 0;
    };
    
    decomp.callbacks.onLargeItem = [](AMLDecompiler*_decomp,const ParserContext* context, LargeResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize) -> int
    {
        DeviceTreeBuilder* self = reinterpret_cast<DeviceTreeBuilder*>(_decomp->userData);
        
        switch (itemType)
        {
            case LargeResourceItemsType_MemoryRangeDescriptor32:
                assert(bufferSize == sizeof(MemoryRangeDescriptor32));
                return self->onMemoryRangeDescriptor32(context, reinterpret_cast<const MemoryRangeDescriptor32*>(buffer));
                
            case LargeResourceItemsType_QWORDAddressSpaceDescriptor:
                assert(bufferSize == sizeof(AddressSpaceDescriptor));
                return self->onQWORDAddressSpaceDescriptor(context, reinterpret_cast<const AddressSpaceDescriptor*>(buffer));
                
            default:
                break;
        }
        /*
         
         */
        return 0;
    };
    
    
}


int DeviceTreeBuilder::onQWORDAddressSpaceDescriptor( const ParserContext* context , const AddressSpaceDescriptor* desc)
{
    indent();
    str << "QWordMemory( "
    << (desc->isConsumer? "ResourceConsumer":"ResourceProducer") << ","
    << (desc->decodeType? "SubDecode" : "PosDecode") << ","
    << (desc->mif? "MinFixed" : "minNOTFixed") << ","
    << (desc->maf? "MaxFixed" : "MaxNOTFixed") << ","
    << "0x" << std::hex << desc->addrSpaceGranularity << ","
    << "0x" << std::hex << desc->addrRangeMin << ","
    << "0x" << std::hex << desc->addrRangeMax << ","
    << "0x" << std::hex << desc->addrTranslationOffset << ","
    << "0x" << std::hex << desc->addrTranslationLength << ","
    << "\n";
    
    indent();
    str << ")\n";
    
    decScope();
    return 0;
}

int DeviceTreeBuilder::onMemoryRangeDescriptor32( const ParserContext* context , const MemoryRangeDescriptor32* desc)
{
    indent();
    str << "Memory32Fixed( "
    << (desc->writeStatus? "Write?":"ReadOnly") << ","
    << std::hex << desc->rangeBaseAddr << ","
    << std::hex << desc->rangeLength << ")"
    << "\n";
    
    decScope();
    return 0;
}

AMLParserError DeviceTreeBuilder::start(const uint8_t* buffer , size_t size)
{
    return AMLDecompilerStart(&decomp, buffer, size);
}


void DeviceTreeBuilder::indent()
{
    for(int i=0;i<currentScope;i++)
    {
        str << "\t";
    }
    
}
void DeviceTreeBuilder::incScope()
{
    currentScope++;
    
    
}

void DeviceTreeBuilder::decScope()
{
    currentScope--;
    }
