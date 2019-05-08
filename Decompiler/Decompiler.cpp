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

#include <string>
#include <iomanip>
#include <iostream>
#include <vector>
#include <math.h>       /* floor */
//#include <iostream>
#include <sstream>
#include "Decompiler.hpp"
#include "AMLDecompilerInterface.hpp"
#include "EISAID.h"


struct ContentString
{
    template< typename T>
    ContentString& operator<<(const T &t)
    {
        _content << t;
        
        if (printOnWrite)
        {
            std::cout << "------- START --------" << std::endl;
            std::cout << _content.str() << std::endl;
            std::cout << "------- END   --------" << std::endl;
        }
        return *this;
    }
    
    std::string str() const
    {
        return _content.str();
    }
    
    std::stringstream _content;
    
    bool printOnWrite;
};






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
        content << "// generated with AML::Decompiler + libACPI" << "\n";
    }
    */
    int onACPIDefinitionBlock( const ParserContext* context, const ACPIDefinitionBlock& desc) override
    {
        
        content << "DefinitionBlock ("
                << "\"\"" << ","
                << "\"" << desc.tableSignature << "\"" << ","
                << std::to_string( desc.complianceRevision)  << ","
                << "\"" << desc.OEMId << "\"" << ","
                << "\"" << desc.tableId << "\"" << ","
                << desc.OEMRev
                << ")"
                << "\n";
        
        content << "{" << "\n";
        
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
    int onValue(const ParserContext* context, uint64_t value)override
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
        content << ")" << "\n";
        
        decScope();
        return 0;
    }
    
    int onString(const ParserContext* context, const char* str) override
    {
        content << "\"" << str << "\"" ;
        content << ")" << "\n";
        return 0;
    }
    
    int endField(const ParserContext* context, const ACPIField& )override
    {
        decScope();
        indent(); content << "}" << "\n";
        
        decScope();
        return 0;
    }
    int onFieldElement(const ParserContext* context, const ACPIFieldElement& fieldElement) override
    {
        if ( /*ACPIFieldElementIsOffset(&fieldElement))// */fieldElement.name[0] == 0)
        {
            if (fieldElement.offsetFromStart %8 == 0)
            {
                indent(); content << "Offset(" ; writeHexArg((uint8_t)(fieldElement.offsetFromStart / 8)); content << ")," << "\n";
            }
            else
            {
                indent(); content << "," ; writeHexArg(fieldElement.value); content << "," << "\n";
            }
        }
        else
        {
            indent(); content << fieldElement.name << ", "; writeHexArg(fieldElement.value); content << "," << "\n";
            
        }
        return 0;
    }
    int startField(const ParserContext* context, const ACPIField& field)override
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
        
        char* fieldName = AMLNameConstructNormalized(&field.name);
        assert(fieldName);
        content << fieldName << ", ";
        free(fieldName);
        
        content << accessTypeDesc( field.flags.accessType) << ", ";
        content << (field.flags.lockRule? "Lock":"NoLock") << ", ";
        content << updateRuleDesc(field.flags.updateRule);
        content << " )" << "\n";
        
        indent(); content << "{" << "\n";
        incScope();
        
        /*
        const int realOffset = (int)( floor( field->offset / 8));
        if (realOffset)
        {
            //Offset (1),
            indent(); content << "Offset ("<< std::to_string( realOffset ) <<")," << "\n";
        }
        
        indent(); content << field->valueName << ", "; writeHexArg(field->value); content << "," << "\n";
        */
        
        return 0;
    }
    
    
    int startIndexField(const ParserContext* context, const ACPIIndexField& indexField) override
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
        
        indent();content << "IndexField (";
        
        char* fieldName = AMLNameConstructNormalized(&indexField.name);
        assert(fieldName);
        char* fieldDataName = AMLNameConstructNormalized(&indexField.dataName);
        assert(fieldDataName);
        content << fieldName << ", " << fieldDataName << ", ";
        free(fieldName);
        free(fieldDataName);
        
        content << accessTypeDesc( indexField.flags.accessType) << ", ";
        content << (indexField.flags.lockRule? "Lock":"NoLock") << ", ";
        content << updateRuleDesc(indexField.flags.updateRule);
        content << " )" << "\n";
        
        indent(); content << "{" << "\n";
        incScope();
        
        return 0;
    }
    int onIndexFieldElement(const ParserContext* context, const ACPIIndexFieldElement& fieldElement)  override
    {
        if ( /*ACPIFieldElementIsOffset(&fieldElement))// */fieldElement.name[0] == 0)
        {
            if (fieldElement.offsetFromStart %8 == 0)
            {
                indent(); content << "Offset(" ; writeHexArg((uint16_t)(fieldElement.offsetFromStart / 8)); content << ")," << "\n";
            }
            else
            {
                indent(); content << "," ; writeHexArg(fieldElement.value); content << "," << "\n";
            }
        }
        else
        {
            indent(); content << fieldElement.name << ", "; writeHexArg(fieldElement.value); content << "," << "\n";
            
        }
        
        return 0;
    }
    
    int endIndexField(const ParserContext* context, const ACPIIndexField&) override
    {
        decScope();
        indent(); content << "}" << "\n";
        
        decScope();
        
        return 0;
    }
    
    int onBuffer(const ParserContext* context , size_t bufferSize , const uint8_t* buffer ) override
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
    
    
    int startScope(const ParserContext* context, const ACPIScope& scope)override
    {
        char* realLoc = AMLNameConstructNormalized(&scope._name);// scope.name[0] == '.' ? scope.name+1 : scope.name;
        incScope();
        indent();
        
        content << "Scope" << "(" << realLoc << ")" << "\n";
        
        indent();
        content << "{" << "\n";
        
        free(realLoc);
        return 0;
    }
    int endScope(const ParserContext* context, const ACPIScope& scope)override
    {
        indent();
        content << "}" << "\n";
        decScope();
        return 0;
    }
    
    int startResourceTemplate( const ParserContext* context , size_t numItems ) override
    {
        
        content << " ResourceTemplate ()"  << "\n";
        indent();
        content << "{" << "\n";
        incScope();
        return 0;
    }
    
    int endResourceTemplate(const ParserContext* context , size_t numItemsParsed, AMLParserError err)override
    {
        decScope();
        indent();
        content << "})" << "\n";
        decScope();
        
        return 0;
    }
    
    
    int startDevice(const ParserContext* context, const ACPIDevice& device)override
    {
        char* realName  = AMLNameConstructNormalized(&device._name);
        incScope();
        indent();
        content << "Device" << "(" << realName << ")" << "\n";
        
        indent();
        content << "{" << "\n";
        
        free(realName);
        return 0;
    }
    int endDevice(const ParserContext* context, const ACPIDevice&)override
    {
        indent();
        content << "}" << "\n";
        
        decScope();
        return 0;
    }
    int startName(const ParserContext* context, const char* name)override
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
    
    
    int onCreateField( const ParserContext* context , const ACPICreateFieldBase *_field) override
    {
        incScope();
        indent();
        decScope();
        switch (_field->type)
        {
            case ACPICreateFieldBase::CreateByteField:
            {
                const ACPICreateByteField* field = reinterpret_cast<const ACPICreateByteField*>(_field);
                assert(field);
                
                content << "CreateByteField" << "(" << field->base.nameSource << ",";
                writeHexArg( (uint8_t)field->byteIndex);
                content << "," << field->base.nameString << ")" << "\n";
                
            }
                break;
            case ACPICreateFieldBase::CreateWordField:
            {
                const ACPICreateWordField* field = reinterpret_cast<const ACPICreateWordField*>(_field);
                assert(field);
                
                content << "CreateWordField" << "(" << field->base.nameSource << ",";
                writeHexArg((uint16_t)field->byteIndex);
                content << "," << field->base.nameString << ")" << "\n";
            }
                break;
            case ACPICreateFieldBase::CreateField:
            {
                const ACPICreateField* field = reinterpret_cast<const ACPICreateField*>(_field);
                assert(field);
                
                content << "CreateField" << "(" << field->base.nameSource << ",";
                writeHexArg(field->byteIndex);
                content << ",";
                writeHexArg(field->numBytes);
                content << "," << field->base.nameString << ")" << "\n";
                
                
            }
                break;
                
            default:
                assert(false);
                break;
        }
        
        return 0;
    }
    
    int onMethod(const ParserContext* context, const ACPIMethod& method) override
    {
        incScope();
        indent();
        content << "Method" << "(" << method.name << ",";
        content << std::to_string( method.argCount );
        content << ")" << "\n";
        
        indent(); content << "{" << "\n";
        incScope();
        
        indent(); content << "// Some Stuff" << "\n";
        
        decScope();
        indent(); content << "}" << "\n";
        
        
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
        content << "\n";
        
        indent();writeHexArg(desc.addrSpaceGranularity);  content << "," << "\n";
        indent();writeHexArg(desc.addrRangeMin);  content << "," << "\n";
        indent();writeHexArg(desc.addrRangeMax);  content << "," << "\n";
        indent();writeHexArg(desc.addrTranslationOffset);  content << "," << "\n";
        indent();writeHexArg(desc.addrTranslationLength);  content << "," << "\n";

        indent();
        content << ",,, "; // empty args
        content << (desc.specificFlags.TTP== 0? " AddressRangeMemory,":"");
        content << (desc.specificFlags.MTP== 0? " TypeStatic":"");
        
        content <<  ")" << "\n";
        
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
        content << "\n";
        
        indent(); writeHexArg(desc.addrSpaceGranularity); content << "," << "\n";
        indent(); writeHexArg(desc.addrRangeMin); content << "," << "\n";
        indent(); writeHexArg(desc.addrRangeMax); content << "," << "\n";
        indent(); writeHexArg(desc.addrTranslationOffset); content << "," << "\n";
        indent(); writeHexArg(desc.addrTranslationLength); content << "," << "\n";
        indent();
        content << ",,, "; // empty args
        content << (desc.specificFlags.TTP== 0? " AddressRangeMemory,":"");
        content << (desc.specificFlags.MTP== 0? " TypeStatic":"");
        
        content <<  ")" << "\n";
        
        
        content << "\n";
        
        
        decScope();
        
        
        return 0;
    }
    
    int onMemoryRangeDescriptor32( const ParserContext* context , const MemoryRangeDescriptor32& desc) override
    {
        indent();content << "Memory32Fixed(";
        
        content << (desc.writeStatus == 0? " ReadOnly,":"");
        content << "\n";
        indent();writeHexArg(desc.rangeBaseAddr);  content << "," << "\n";
        indent();writeHexArg(desc.rangeLength);  content << "," << "\n";
        indent();content << ")" << "\n";
        return 0;
    }
    
    int onOperationRegion(const ParserContext* context, const ACPIOperationRegion& region)override
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
                    
                case IPMI:
                    return "IPMI";
                case GeneralPurposeIO:
                    return "GeneralPurposeIO";
                case GenericSerialBus:
                    return "GenericSerialBus";
                    
                default:
                    break;
            }
        
            return std::to_string(space);
        };
        
        incScope();
        indent();content << "OperationRegion(";
        content << region.name << " ,";
        
        
        content << regionSpaceGetStr( region.space);
        
        
        content << ",";
        
        writeHexArg( (uint16_t) region.offset);
        content << ",";
        
        writeNumValue( (uint16_t) region.length);
        
        content << ")" << "\n";
        
        
        decScope();
        return 0;
    }
    
    int startPackage( const ParserContext*context , const ACPIPackage& package) override
    {
        
        
        indent(); content << "Package(";
        writeHexArg(package.numElements);
        content << ")" << "\n";
        
        indent();content << "{"<< "\n";
        
        incScope();
        
        return 0;
    }
    
    int onPackageElement( const ParserContext* context , const ACPIPackageElement& element) override
    {
        incScope();
        
        indent();
        switch (element.type)
        {
            case ACPIPackageElement::Integer:
                writeNumValue(element.value.value);
                content << "," << "\n";
                break;
                
            case ACPIPackageElement::String:
                content << element.value.string << "," << "\n";
                break;
                
            default:
                
                break;
        }
        
        decScope();
        return 0;
    }
    
    int endPackage( const ParserContext*context , const ACPIPackage& package) override
    {
        decScope();
        indent();
        content << "}";
        
        if (package.packageRef != NULL)
        {
            content << ",";
        }
        else
        {
            content << ")";
        }
            
        
        content << "\n";
        
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
        content << "\n";
        
        indent(); writeHexArg(desc.addrSpaceGranularity); content << "," << "\n";
        indent(); writeHexArg(desc.addrRangeMin); content << "," << "\n";
        indent(); writeHexArg(desc.addrRangeMax); content << "," << "\n";
        indent(); writeHexArg(desc.addrTranslationOffset); content << "," << "\n";
        indent(); writeHexArg(desc.addrTranslationLength); content << "," << "\n";
        indent(); content << ",, )" << "\n";
        
        content << "\n";
        
        
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
        indent(); content << "IO (Decode16," << "\n";
        
        indent(); writeHexArg(desc.rangeMinBaseAddr); content << "," << "\n";
        indent(); writeHexArg(desc.rangeMaxBaseAddr); content << "," << "\n";
        indent(); writeHexArg(desc.baseAlign); content << " ," << "\n";
        indent(); writeHexArg(desc.rangeLen); content << " ," << "\n";
        
        indent(); content << ")" << "\n";
        
        content << "\n";
        
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
    
    
    ContentString content;
    
private:
    int currentScope = 0;
};


int AML::Decompiler::process( const uint8_t* buffer , std::size_t bufferSize)
{
    AMLDecompiler decomp;
    
    
    if(AMLDecompilerInit(&decomp) == 0)
    {
        return 0;
    }
    
    //decomp.parserPolicy.assertOnError = 1;
    
    //decomp.parserPolicy.assertOnInvalidState = 1;
    decomp.parserPolicy.assertOnError = 1;
    
    DecompilerImpl impl(decomp);
    
    //impl.content.printOnWrite = true;
    
    AMLParserError err = AMLDecompilerStart(&decomp, buffer, bufferSize);
    
    if (err == AMLParserError_None)
    {
        impl.content << "}" << "\n";
        
        
        result = impl.content.str();
        
    }
    else
    {
        printf("AML::Decompiler  Error : current ASL is \n %s \n" ,impl.content.str().c_str() );
    }
    return err;// == AMLParserError_None;
}




static std::string generateHeaderComment()
{
    return "// File Generated with AML::Decompiler from libACPI\n";
}


std::string AML::Decompiler::getStringResult() const
{
    return generateHeaderComment() + result;
}
