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

    FILE *fileptr;
    fileptr = fopen(fromFile, "rb");  // Open the file in binary mode
    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    *bufSize = ftell(fileptr);             // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file
    
    uint8_t *buffer = (uint8_t *)malloc((*bufSize+1)*sizeof(char)); // Enough memory for file + \0
    fread(buffer, *bufSize, 1, fileptr); // Read in the entire file
    fclose(fileptr); // Close the file
    return buffer;
}




class Decompiler
{
public:
    Decompiler()
    {
        AMLDecompilerInit(&decomp);
        
        decomp.parserPolicy.assertOnError = 1;
        
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
        
        
        decomp.callbacks.OnBuffer = [](AMLDecompiler* _decomp , const ParserContext* ctx , size_t bufferSize , const uint8_t* buffer) -> int
        {
            Decompiler* self = reinterpret_cast<Decompiler*>(_decomp->userData);
            
            
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
            Decompiler* self = reinterpret_cast<Decompiler*>(_decomp->userData);
            
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
            Decompiler* self = reinterpret_cast<Decompiler*>(_decomp->userData);
            
            self->decScope();
            self->str << "\n";
            self->indent();
            self->str << "}\n";
            
            self->decScope();
            
            return 0;
        };
        
        decomp.callbacks.StartName = []( AMLDecompiler* _decomp ,const ParserContext* context, const char* name) -> int
        {
            Decompiler* self = reinterpret_cast<Decompiler*>(_decomp->userData);
            
            
            self->indent();
            self->str << std::string("Name( ") + std::string(name,4)  << "\n";


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
    
    AMLParserError start(const uint8_t* buffer , size_t size)
    {
        return AMLDecompilerStart(&decomp, buffer, size);
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
static void writeBinFile(const uint8_t* buf, size_t bufSize)
{
    FILE *fptr;
    
    if ((fptr = fopen("out.aml","wb")) == NULL)
    {
        printf("Error! opening file");
        
        return;
    }
    
    fwrite(buf, bufSize, 1, fptr);
    
    fclose(fptr);
}
*/

int main(int argc, const char * argv[])
{
    /*
    writeBinFile(b, sizeof(b));
    return 0;
     */
    size_t bufSize = 0;
    
    //const char* file = "Field.aml";
    //const char* file = "resTemp.aml";
    //const char* file = "qemu-dsdt.aml";
    const char* file = "out.aml";
    uint8_t *dataBuffer = readAndFillBuffer(file , &bufSize);
    
    if (!dataBuffer)
    {
        printf("Error while reading file '%s'\n" , file);
        return 1;
    }
    Decompiler decomp;
    
    AMLParserError err = decomp.start( dataBuffer, bufSize);
    
    
    printf("------- Result (ret code %i) --------------------\n" , err);
    printf("%s\n" , decomp.getResult().c_str());
    
    free(dataBuffer);
    return 0;
}
