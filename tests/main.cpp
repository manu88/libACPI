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

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include "AMLDecompiler.h"

static uint8_t* readAndFillBuffer(const char* fromFile , size_t* bufSize)
{
    
    int fd = open(fromFile, O_RDONLY);
    
    uint8_t c = 0;

    uint8_t* ptr = NULL;
    
    size_t size = 0;
    
    while(  read(fd, &c, 1) >0)
    {
        uint8_t* lastP = ptr;
        ptr = reinterpret_cast<uint8_t*>( realloc(ptr, ++size) );
        
        if (!ptr) 
        {
            free(lastP);
            *bufSize = 0;
            return NULL;
        }
        ptr[size-1] = c;
        
    }
    
    *bufSize = size;

    return ptr;
}


static std::string idToString(const char* s)
{
    return /*s[3] == 0? s :*/ std::string(s);//, 4);
}

class Decompiler
{
public:
    Decompiler()
    {
        AMLDecompilerInit(&decomp);
        decomp.userData = this;
        
        decomp.callbacks.OnDefinitionBlock = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIDefinitionBlock* desc)-> int
        {
            Decompiler* self = reinterpret_cast<Decompiler*>(_decomp->userData);
            
            
            self->str << "DefinitionBlock (\"\",";
            self->str << std::string(",\"")+ desc->tableSignature + "\",";
            self->str << std::to_string(desc->complianceRevision) + ",";
            self->str << std::string("\"")+ desc->OEMId + "\",";
            self->str << std::string("\"")+ desc->tableId + "\",";
            self->str << std::to_string(desc->OEMRev) + ")";
            self->str << "\n{\n";
            

            
            return 0;
        };
        
        decomp.callbacks.StartDevice = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIDevice* device) -> int
        {
            Decompiler* self = reinterpret_cast<Decompiler*>(_decomp->userData);
            
            self->incScope();
            
            self->indent();
            
            self->str << std::string("Device(") + idToString(device->name) + ")";
            
            self->str << "\n";
            self->indent();
            self->str << "{\n";
            self->incScope();
//            printf("STR '%s'\n" , self->str.str().c_str());
            return 0;
        };
        
        decomp.callbacks.EndDevice = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIDevice* dev) -> int
        {
            Decompiler* self = reinterpret_cast<Decompiler*>(_decomp->userData);
            
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
            Decompiler* self = reinterpret_cast<Decompiler*>(_decomp->userData);
            
            
            self->indent();
            self->str << std::string("Name( ") + std::string(name,4)  << "\n";
            
            
            
            //self->decScope();

            return 0;
        };
        
        decomp.callbacks.EndName = []( AMLDecompiler* _decomp ,const ParserContext* context, const char* name) -> int
        {
            Decompiler* self = reinterpret_cast<Decompiler*>(_decomp->userData);
            
            
            self->indent();
            self->str << ")" << "\n";
            
            //self->decScope();
            
            return 0;
        };
        
        decomp.callbacks.StartScope = []( AMLDecompiler* _decomp ,const ParserContext* context, const char* location) -> int
        {
            Decompiler* self = reinterpret_cast<Decompiler*>(_decomp->userData);
            
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
            Decompiler* self = reinterpret_cast<Decompiler*>(_decomp->userData);
            
            self->indent();
            self->str << "}\n";
            
            self->decScope();
            return 0;
        };
        
        
        decomp.callbacks.OnValue = []( AMLDecompiler* _decomp ,const ParserContext* context, uint64_t val) -> int
        {
            Decompiler* self = reinterpret_cast<Decompiler*>(_decomp->userData);
            
            self->indent();
            self->str << " 0x" <<  std::hex << val << " ";
            return 0;
        };
        
       
        
        decomp.callbacks.onOperationRegion = []( AMLDecompiler* _decomp ,const ParserContext* context, const ACPIOperationRegion* reg) -> int
        {
            Decompiler* self = reinterpret_cast<Decompiler*>(_decomp->userData);
            
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
            Decompiler* self = reinterpret_cast<Decompiler*>(_decomp->userData);
            
            self->incScope();
            self->indent();
            
            self->str << std::string("Field(")
            << idToString(field->name)
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
            Decompiler* self = reinterpret_cast<Decompiler*>(_decomp->userData);
            
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
            Decompiler* self = reinterpret_cast<Decompiler*>(_decomp->userData);
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
            Decompiler* self = reinterpret_cast<Decompiler*>(_decomp->userData);
            
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
    int onQWORDAddressSpaceDescriptor( const ParserContext* context , const AddressSpaceDescriptor* desc)
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
    
    int onMemoryRangeDescriptor32( const ParserContext* context , const MemoryRangeDescriptor32* desc)
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
    
    bool start(const uint8_t* buffer , size_t size)
    {
        /*
        for(int i=0; i<size;i++)
        {
            printf(" 0x%x " , buffer[i]);
        }
         */
        return AMLDecompilerStart(&decomp, buffer, size) == AMLParserError_None;
    }
    
    std::string getResult() const
    {
        return str.str();
    }
    
    void indent()
    {
        for(int i=0;i<currentScope;i++)
        {
            str << "\t";
        }
        
    }
    void incScope()
    {
        currentScope++;
        
        
    }
    
    void decScope()
    {
        currentScope--;
        
        
    }
    
private:
    AMLDecompiler decomp;
    
    
    std::stringstream str;
    int currentScope = 0;
    
    
};

/*
0x44  0x53  0x44  0x54  0x91  0x0  0x0  0x0
0x1   0xd7  0x42  0x58  0x50  0x43  0x0  0x0
0x42  0x58  0x44  0x53  0x44  0x54  0x0 0x0
0x1   0x0   0x0  0x0  0x49  0x4e  0x54  0x4c
0x13  0x12  0x18  0x20  0x10  0x49  0x4 0x5c
 0x0  0x5b  0x80  0x44  0x42  0x47 0x5f  0x1
 0xb  0x2  0x4  0x1  0x5b  0x81  0xb  0x44
 0x42  0x47  0x5f  0x1  0x44  0x42  0x47  0x42
 0x8  0x14  0x2c  0x44  0x42  0x55  0x47  0x1
 0x98  0x68  0x60  0x96  0x60  0x60  0x74  0x87
 0x60  0x1  0x61  0x70  0x0  0x62  0xa2  0x10
 0x95  0x62  0x61  0x70  0x83  0x88  0x60  0x62
 0x0  0x44  0x42  0x47  0x42  0x75  0x62  0x70
 0xa  0xa  0x44  0x42  0x47  0x42  0x10  0x22
 0x5f  0x53  0x42  0x5f  0x5b  0x82  0x1b  0x50
 0x43  0x49  0x30  0x8  0x5f  0x48  0x49  0x44
 0xc  0x41  0xd0  0xa  0x3  0x8  0x5f  0x41
 0x44  0x52  0x0  0x8  0x5f  0x55  0x49  0x44  0x1
 */
int main(int argc, const char * argv[])
{
    size_t bufSize = 0;
    
    //const char* file = "Field.aml";
    //const char* file = "resTemp.aml";
    const char* file = "qemu-dsdt.aml";
    uint8_t *dataBuffer = readAndFillBuffer(file , &bufSize);
    
    if (!dataBuffer)
    {
        printf("Error while reading file '%s'\n" , file);
        return 1;
    }
    Decompiler decomp;
    
    if(decomp.start( dataBuffer, bufSize))
    {
        
    }
    
    printf("---------------------------\n");
    printf("%s\n" , decomp.getResult().c_str());
    
    free(dataBuffer);
    return 0;
}
