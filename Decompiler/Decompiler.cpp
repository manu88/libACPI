//
//  Decompiler.cpp
//  Decompiler
//
//  Created by Manuel Deneu on 09/02/2019.
//  Copyright © 2019 Manuel Deneu. All rights reserved.
//

#include <string>
#include <iomanip>
//#include <iostream>
#include <sstream>
#include "Decompiler.hpp"
#include "AMLDecompilerInterface.hpp"
#include "EISAID.h"




class DecompilerImpl : public AMLDecompilerInterface
{
public:
    DecompilerImpl( AMLDecompiler &decomp):
    AMLDecompilerInterface(decomp)
    {
        
    }
    
    void writeHexArg(uint8_t v)
    {
        content << "0x"<< std::uppercase<< std::hex<< std::setw(2) << std::setfill('0') << v;
    }
    
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
    
    void writeNumValue( uint64_t v)
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
                << desc->OEMRev  << ","
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
            content << "EisaId (\"" << GetEisaId(value) << "\")";
        }
        else
        {
            writeNumValue(value);
            
        }
        content << ")" << std::endl;
        
        decScope();
        return 0;
    }
    
    int onOperationRegion(const ParserContext* context, const ACPIOperationRegion*)override
    {
        return 0;
    }
    int onField(const ParserContext* context, const ACPIField*)override
    {
        return 0;
    }
    int OnBuffer(const ParserContext* context , size_t bufferSize , const uint8_t* buffer)override
    {
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
        content << "}) // ENDOF ressourceTemplate" << std::endl;
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
        incScope();
        indent();
        content << "Name" << "(" << name << ",";
        return 0;
    }
    int EndName(const ParserContext* context, const char* name)override
    {
        return 0;
    }
    int startMethod(const ParserContext* context, const char* name)override
    {
        return 0;
    }
    int endMethod(const ParserContext* context, const char* name)override
    {
        return 0;
    }
    int onQWORDAddressSpaceDescriptor( const ParserContext* context , const QWordAddressSpaceDescriptor& desc) override
    {
        indent();content << "QWordMemory(SOME ARGS," << std::endl;
        indent();writeHexArg(desc.addrSpaceGranularity);  content << "," << std::endl;
        indent();writeHexArg(desc.addrRangeMin);  content << "," << std::endl;
        indent();writeHexArg(desc.addrRangeMax);  content << "," << std::endl;
        indent();writeHexArg(desc.addrTranslationOffset);  content << "," << std::endl;
        indent();writeHexArg(desc.addrTranslationLength);  content << "," << std::endl;

        indent();content << "SOME OTHER ARGS" << ")" << std::endl;
        
        return 0;
    }
    
    
    
    int onMemoryRangeDescriptor32( const ParserContext* context , const MemoryRangeDescriptor32& desc) override
    {
        indent();content << "Memory32Fixed(SOME ARGS," << std::endl;
        indent();writeHexArg(desc.rangeBaseAddr);  content << "," << std::endl;
        indent();writeHexArg(desc.rangeLength);  content << "," << std::endl;
        indent();content << ")" << std::endl;
        return 0;
    }
    
    int onWORDAddressSpaceDescriptor( const ParserContext* context , const WordAddressSpaceDescriptor& desc) override
    {
        return 0;
    }
    int onDWORDAddressSpaceDescriptor( const ParserContext* context , const DWordAddressSpaceDescriptor& desc) override
    {
        return 0;
    }
    int onIOPortDescriptor( const ParserContext* context , const IOPortDescriptor&desc) override
    {
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
    
    DecompilerImpl impl(decomp);
    
    AMLParserError err = AMLDecompilerStart(&decomp, buffer, bufferSize);
    
    if (err == AMLParserError_None)
    {
        impl.content << "}" << std::endl;
        
        
        result = impl.content.str();
        
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
