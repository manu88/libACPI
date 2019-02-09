//
//  main.cpp
//  Decompiler
//
//  Created by Manuel Deneu on 09/02/2019.
//  Copyright © 2019 Manuel Deneu. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include "Decompiler.hpp"

static uint8_t* readAndFillBuffer(const char* fromFile , size_t* bufSize)
{
    
    FILE *fileptr;
    fileptr = fopen(fromFile, "rb");  // Open the file in binary mode
    
    if (fileptr == NULL)
        return NULL;
    
    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    *bufSize = ftell(fileptr);             // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file
    
    uint8_t *buffer = (uint8_t *)malloc((*bufSize+1)*sizeof(char)); // Enough memory for file + \0
    fread(buffer, *bufSize, 1, fileptr); // Read in the entire file
    fclose(fileptr); // Close the file
    
    
    return buffer;
}

int main(int argc, const char * argv[])
{
    const char* filePath = argv[1];
    
    size_t bufSize = 0;
    uint8_t* buffer = readAndFillBuffer(filePath, &bufSize);
    
    if (buffer )
    {
        AML::Decompiler decomp;
        
        auto ret = decomp.process(buffer, bufSize);
        
        if (ret )
        {
            const std::string result = decomp.getStringResult();
            
            std::cout << "Result : " << std::endl;
            std::cout << result << std::endl;;
        }
        
        free(buffer);
        
        return ret ? 0 : 10;
    }
    return 1;
}