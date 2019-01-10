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


class Decompiler
{
public:
    Decompiler()
    {
        AMLDecompilerInit(&decomp);
        decomp.userData = this;
        
        decomp.callbacks.OnDefinitionBlock = []( AMLDecompiler* _decomp , const ACPIDefinitionBlock* desc)-> int
        {
            Decompiler* self = reinterpret_cast<Decompiler*>(_decomp->userData);
            self->str += "DefinitionBlock (\"\",";
            self->str += std::string(",\"")+ desc->tableSignature + "\",";
            self->str += std::to_string(desc->complianceRevision) + ",";
            self->str += std::string("\"")+ desc->OEMId + "\",";
            self->str += std::string("\"")+ desc->tableId + "\",";
            self->str += std::to_string(desc->OEMRev) + ")";
            self->str += "\n{\n";
            
            return 0;
        };
        
        decomp.callbacks.StartDevice = []( AMLDecompiler* _decomp , const char* name) -> int
        {
            Decompiler* self = reinterpret_cast<Decompiler*>(_decomp->userData);
            
            self->str += std::string("Device( \"") + name + "\")";
            self->str += "\n{\n";
            return 0;
        };
        
        decomp.callbacks.EndDevice = []( AMLDecompiler* _decomp , const char* name) -> int
        {
            Decompiler* self = reinterpret_cast<Decompiler*>(_decomp->userData);
            
            self->str += "\n}\n";
            return 0;
        };
        
        decomp.callbacks.StartName = []( AMLDecompiler* _decomp , const char* name) -> int
        {
            Decompiler* self = reinterpret_cast<Decompiler*>(_decomp->userData);
            
            self->str += std::string("Name( \"") + name + "\")";
            
            return 0;
        };
        
    }
    
    bool start(const uint8_t* buffer , size_t size)
    {
        
        return AMLDecompilerStart(&decomp, buffer, size) == AMLParserError_None;
    }
    
    const std::string& getResult() const
    {
        return str;
    }
    
private:
    AMLDecompiler decomp;
    
    
    std::string str;
    
    
};


int main(int argc, const char * argv[])
{
    size_t bufSize = 0;
    
    uint8_t *dataBuffer = readAndFillBuffer("qemu-dsdt.aml" , &bufSize);
    
    Decompiler decomp;
    
    if(decomp.start( dataBuffer, bufSize))
    {
        printf("---------------------------\n");
        printf("%s\n" , decomp.getResult().c_str());
    }
    
    
    free(dataBuffer);
    return 0;
}
