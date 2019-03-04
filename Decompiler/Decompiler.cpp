//
//  Decompiler.cpp
//  Decompiler
//
//  Created by Manuel Deneu on 09/02/2019.
//  Copyright © 2019 Manuel Deneu. All rights reserved.
//

#include <string>
#include <iomanip>
#include <vector>
#include <math.h>       /* floor */
//#include <iostream>
#include <sstream>
#include "Decompiler.hpp"
#include "AMLDecompilerInterface.hpp"
#include "EISAID.h"

static bool isString(const uint8_t* buffer,size_t bufferSize )
{
    if (!buffer)
        return false;
    assert(bufferSize);
    
    if ( buffer[bufferSize-1] != 0)
        return false;
    
    for(size_t i = 0;i< bufferSize-1;i++)
    {
        if (isprint(  buffer[i])  == 0)
        {
            return false;
        }
    }
    
    return true;
}


class DecompilerImpl : public AMLDecompilerInterface
{
public:
    DecompilerImpl( AMLDecompiler &decomp):
    AMLDecompilerInterface(decomp)
    {
        
    }
    template <typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    void writeHexArg(T v)
    {
        
        content << "0x"<< std::uppercase<< std::hex<< std::setw(sizeof(T)*2) << std::setfill('0') << (long long) v;
    }
    /*
    void writeHexArg(uint16_t v)
    {
        content << "0x"<< std::uppercase<< std::hex<< std::setw(4) << std::setfill('0') << v;
    }
    
    void writeHexArg(uint32_t v)
    {
        content << "0x"<< std::uppercase << std::hex<< std::setw(8) << std::setfill('0') << v;
    }
    
    
    void writeHexArg(uint64_t v)
    {
        content << "0x"<< std::uppercase<< std::hex<< std::setw(16) << std::setfill('0') << v;
    }
    */
    template <typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    void writeNumValue( T v)
    {
        if( v == 0)
        {
            content << "Zero";
        }
        else if (v==1)
        {
            content << "One";
        }
        else if (v==0xFFFFFFFFFFFFFFFF)
        {
            content << "Ones";
        }
        else
        {
            writeHexArg(v);
        }
    }
    /*
    void writeNumValue( uint32_t v)
    {
        if( v == 0)
        {
            content << "Zero";
        }
        else if (v==1)
        {
            content << "One";
        }
        else
        {
            writeHexArg(v);
        }
    }
    void writeNumValue( uint16_t v)
    {
        if( v == 0)
        {
            content << "Zero";
        }
        else if (v==1)
        {
            content << "One";
        }
        else
        {
            writeHexArg(v);
        }
    }
    void writeNumValue( uint8_t v)
    {
        if( v == 0)
        {
            content << "Zero";
        }
        else if (v==1)
        {
            content << "One";
        }
        else
        {
            writeHexArg(v);
        }
    }
     */
    /*
    void addheaderText()
    {
        content << "// generated with AML::Decompiler + libACPI" << std::endl;
    }
    */
    int onACPIDefinitionBlock( const ParserContext* context, const ACPIDefinitionBlock* desc) override
    {
        content << "DefinitionBlock ("
                << "\"\"" << ","
                << "\"" << desc->tableSignature << "\"" << ","
                << std::to_string( desc->complianceRevision)  << ","
                << "\"" << desc->OEMId << "\"" << ","
                << "\"" << desc->tableId << "\"" << ","
                << desc->OEMRev
                << ")"
                << std::endl;
        
        content << "{" << std::endl;
        
        return 0;
    }
    /*
    int onLargeItem(const ParserContext* context, LargeResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize)override
    {
        return 0;
    }
    int onSmallItem(const ParserContext* context, SmallResourceItemsType itemType, const uint8_t* buffer , size_t bufferSize)override
    {
        return 0;
    }
     */
    int OnValue(const ParserContext* context, uint64_t value)override
    {
        
        if (isEisaId(value))
        {
            char eisaid[8] = {0};
            assert(getEisaidString(value, eisaid));
            content << "EisaId (\"" << eisaid << "\")";
        }
        else
        {
            writeNumValue(value);
            
        }
        content << ")" << std::endl;
        
        decScope();
        return 0;
    }
    
    
    int onField(const ParserContext* context, const ACPIField* field)override
    {
        incScope();
        
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
        
        indent();content << "Field (";
        
        
        content << field->name << ", ";
        
        content << accessTypeDesc( field->accessType) << ", ";
        content << (field->lockRule? "Lock":"NoLock") << ", ";
        content << updateRuleDesc(field->updateRule);
        content << " )" << std::endl;
        
        indent(); content << "{" << std::endl;
        incScope();
        
        
        const int realOffset = (int)( floor( field->offset / 8));
        if (realOffset)
        {
            //Offset (1),
            indent(); content << "Offset ("<< std::to_string( realOffset ) <<")," << std::endl;
        }
        
        indent(); content << field->valueName << ", "; writeNumValue(field->value); content << "," << std::endl;
        
        decScope();
        indent(); content << "}" << std::endl;
        
        decScope();
        return 0;
    }
    
    
    
  
    int OnBuffer(const ParserContext* context , size_t bufferSize , const uint8_t* buffer ) override
    {
        
        
        content << "Buffer(";
        
        if (isString(buffer, bufferSize))
        {
            content << ")";
            content << "{\"";
            content << (const char*) buffer;
            content << "\"";
        }
        else
        {
            writeHexArg((uint32_t)bufferSize);
            content << /*std::to_string(bufferSize) <<*/ ")";
            content << " ";
            content << "{";
            
            if (buffer)
            {
                bool first = true;
                for( size_t i=0; i<bufferSize;i++)
                {
                    if (!first)
                    {
                        content << ",";
                    }
                    writeHexArg(buffer[i]);
                    //content << "0x" << std::hex <<(int) buffer[i];
                    //writeHexArg(buffer[i]);
                    
                    if (first)
                        first = false;
                }
            }
        }
        content << "}";
        
        EndName();
        
        return 0;
    }
    
    
    int StartScope(const ParserContext* context, const char* location)override
    {
        const char* realLoc = location[0] == '.' ? location+1 : location;
        incScope();
        indent();
        
        content << "Scope" << "(" << realLoc << ")" << std::endl;
        
        indent();
        content << "{" << std::endl;
        return 0;
    }
    int EndScope(const ParserContext* context, const char* location)override
    {
        indent();
        content << "}" << std::endl;
        decScope();
        return 0;
    }
    
    int startResourceTemplate( const ParserContext* context , size_t numItems ) override
    {
        
        content << " ResourceTemplate ()"  << std::endl;
        indent();
        content << "{" << std::endl;
        incScope();
        return 0;
    }
    
    int endResourceTemplate(const ParserContext* context , size_t numItemsParsed, AMLParserError err)override
    {
        decScope();
        indent();
        content << "})" << std::endl;
        decScope();
        
        return 0;
    }
    
    
    int StartDevice(const ParserContext* context, const ACPIDevice* device)override
    {
        incScope();
        indent();
        content << "Device" << "(" << device->name << ")" << std::endl;
        
        indent();
        content << "{" << std::endl;
        return 0;
    }
    int EndDevice(const ParserContext* context, const ACPIDevice* name)override
    {
        indent();
        content << "}" << std::endl;
        
        decScope();
        return 0;
    }
    int StartName(const ParserContext* context, const char* name)override
    {
        if (strcmp(name, "_FDI") == 0)
        {
            
        }
        incScope();
        indent();
        content << "Name" << "(" << name << ",";
        return 0;
    }
    
    void EndName()
    {
        content << ")\n";
        decScope();
    }
    
    int onMethod(const ParserContext* context, const ACPIMethod* method) override
    {
        incScope();
        indent();
        content << "Method" << "(" << method->name << ",";
        content << std::to_string( method->argCount );
        content << ")" << "\n";
        
        indent(); content << "{" << std::endl;
        incScope();
        
        indent(); content << "// Some Stuff" << std::endl;
        
        decScope();
        indent(); content << "}" << std::endl;
        
        
        decScope();
        return 0;
    }
    
    int onQWORDAddressSpaceDescriptor( const ParserContext* context , const QWordAddressSpaceDescriptor& desc) override
    {
        indent();content << "QWordMemory(" ;
        
        content << (desc.isConsumer == 0? " ResourceProducer,":"");
        content << (desc.decodeType == 0? " PosDecode,":"");
        content << (desc.mif? " MinFixed,":"");
        content << (desc.maf? " MaxFixed,":"");
        content << (desc.specificFlags.MEM== 2? " Cacheable,":"");
        content << (desc.specificFlags.RW== 1? " ReadWrite,":"");
        content << std::endl;
        
        indent();writeHexArg(desc.addrSpaceGranularity);  content << "," << std::endl;
        indent();writeHexArg(desc.addrRangeMin);  content << "," << std::endl;
        indent();writeHexArg(desc.addrRangeMax);  content << "," << std::endl;
        indent();writeHexArg(desc.addrTranslationOffset);  content << "," << std::endl;
        indent();writeHexArg(desc.addrTranslationLength);  content << "," << std::endl;

        indent();
        content << ",,, "; // empty args
        content << (desc.specificFlags.TTP== 0? " AddressRangeMemory,":"");
        content << (desc.specificFlags.MTP== 0? " TypeStatic":"");
        
        content <<  ")" << std::endl;
        
        return 0;
    }
    
    int onDWORDAddressSpaceDescriptor( const ParserContext* context , const DWordAddressSpaceDescriptor& desc) override
    {
        incScope();
        
        /*
         DWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed, NonCacheable, ReadWrite,
         ,, _Y00, AddressRangeMemory, TypeStatic)
         */
        
        indent(); content << "DWordMemory (";
        
        content << (desc.isConsumer?"ResourceConsumer":"ResourceProducer") << ",";
        content << (desc.decodeType?"SubDecode":"PosDecode") << ",";
        content << (desc.mif?"MinFixed":"_MinNOTFixed_") << ",";
        content << (desc.maf?"MaxFixed":"_MaxNOTFixed_") << ",";
        content << (desc.specificFlags.MEM== 2? " Cacheable,":"NonCacheable,");
        content << (desc.specificFlags.RW== 1? " ReadWrite,":"");
        content << std::endl;
        
        indent(); writeHexArg(desc.addrSpaceGranularity); content << "," << std::endl;
        indent(); writeHexArg(desc.addrRangeMin); content << "," << std::endl;
        indent(); writeHexArg(desc.addrRangeMax); content << "," << std::endl;
        indent(); writeHexArg(desc.addrTranslationOffset); content << "," << std::endl;
        indent(); writeHexArg(desc.addrTranslationLength); content << "," << std::endl;
        indent();
        content << ",,, "; // empty args
        content << (desc.specificFlags.TTP== 0? " AddressRangeMemory,":"");
        content << (desc.specificFlags.MTP== 0? " TypeStatic":"");
        
        content <<  ")" << std::endl;
        
        
        content << std::endl;
        
        
        decScope();
        
        
        return 0;
    }
    
    int onMemoryRangeDescriptor32( const ParserContext* context , const MemoryRangeDescriptor32& desc) override
    {
        indent();content << "Memory32Fixed(";
        
        content << (desc.writeStatus == 0? " ReadOnly,":"");
        content << std::endl;
        indent();writeHexArg(desc.rangeBaseAddr);  content << "," << std::endl;
        indent();writeHexArg(desc.rangeLength);  content << "," << std::endl;
        indent();content << ")" << std::endl;
        return 0;
    }
    
    int onOperationRegion(const ParserContext* context, const ACPIOperationRegion* region)override
    {
        auto regionSpaceGetStr = [] (uint64_t space) -> std::string
        {
            switch (space)
            {
                case SystemMemory:
                    return "SystemMemory";
                case SystemIO:
                    return "SystemIO";
                case PCI_Config:
                    return "PCI_Config";
                case EmbeddedControl:
                    return "EmbeddedControl";
                case SMBus:
                    return "SMBus";
                case CMOS:
                    return "CMOS";
                case PCIBARTarget:
                    return "PCIBARTarget";
                    
                default:
                    break;
            }
        
            return std::to_string(space);
        };
        
        incScope();
        indent();content << "OperationRegion(";
        content << region->name << ",";
        
        
        content << regionSpaceGetStr( region->space);
        
        
        content << ",";
        
        writeHexArg( (uint16_t) region->offset);
        content << ",";
        
        writeNumValue( (uint16_t) region->length);
        
        content << ")" << std::endl;
        
        
        decScope();
        return 0;
    }
    
    int onPackage( const ParserContext*context , const ACPIPackage& package) override
    {
        const size_t s = sizeof(FloppyDiskInformation);
        
        const FloppyDiskInformation *disk = reinterpret_cast<const FloppyDiskInformation *>(&package);
        
        
        return 0;
    }
    
    int onWORDAddressSpaceDescriptor( const ParserContext* context , const WordAddressSpaceDescriptor& desc) override
    {
        incScope();
        
        indent(); content << "WordBusNumber (";
        
        content << (desc.isConsumer?"ResourceConsumer":"ResourceProducer") << ",";
        content << (desc.mif?"MinFixed":"_MinNOTFixed_") << ",";
        content << (desc.maf?"MaxFixed":"_MaxNOTFixed_") << ",";
        content << (desc.decodeType?"SubDecode":"PosDecode") << ",";
        content << std::endl;
        
        indent(); writeHexArg(desc.addrSpaceGranularity); content << "," << std::endl;
        indent(); writeHexArg(desc.addrRangeMin); content << "," << std::endl;
        indent(); writeHexArg(desc.addrRangeMax); content << "," << std::endl;
        indent(); writeHexArg(desc.addrTranslationOffset); content << "," << std::endl;
        indent(); writeHexArg(desc.addrTranslationLength); content << "," << std::endl;
        indent(); content << ",, )" << std::endl;
        
        content << std::endl;
        
        
        decScope();
        return 0;
    }

    int onIOPortDescriptor( const ParserContext* context , const IOPortDescriptor&desc) override
    {
        /*
         IO (Decode16,
         0x0CF8,             // Range Minimum
         0x0CF8,             // Range Maximum
         0x01,               // Alignment
         0x08,               // Length
         )
         
         */
        incScope();
        indent(); content << "IO (Decode16," << std::endl;
        
        indent(); writeHexArg(desc.rangeMinBaseAddr); content << "," << std::endl;
        indent(); writeHexArg(desc.rangeMaxBaseAddr); content << "," << std::endl;
        indent(); writeHexArg(desc.baseAlign); content << " ," << std::endl;
        indent(); writeHexArg(desc.rangeLen); content << " ," << std::endl;
        
        indent(); content << ")" << std::endl;
        
        content << std::endl;
        
        decScope();
        
        return 0;
    }
    
    void indent()
    {
        for(int i=0;i<currentScope;i++)
        {
            content << "\t";
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
    
    std::stringstream content;
    
private:
    int currentScope = 0;
};


bool AML::Decompiler::process( const uint8_t* buffer , std::size_t bufferSize)
{
    AMLDecompiler decomp;
    
    if(AMLDecompilerInit(&decomp) == 0)
    {
        return 0;
    }
    
    //decomp.parserPolicy.assertOnError = 1;
    
    DecompilerImpl impl(decomp);
    
    AMLParserError err = AMLDecompilerStart(&decomp, buffer, bufferSize);
    
    if (err == AMLParserError_None)
    {
        impl.content << "}" << std::endl;
        
        
        result = impl.content.str();
        
    }
    else
    {
        printf("AML::Decompiler  Error : current ASL is \n %s \n" ,impl.content.str().c_str() );
    }
    return err == AMLParserError_None;
}




static std::string generateHeaderComment()
{
    return "// File Generated with AML::Decompiler from libACPI\n";
}


std::string AML::Decompiler::getStringResult() const
{
    return generateHeaderComment() + result;
}
