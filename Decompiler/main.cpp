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

#include <fstream>
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

static bool writeStringToFile(const std::string &input , const std::string &path)
{
    
    std::ofstream out(path);
    out << input;
    out.close();
    return 0;
}

int main(int argc, const char * argv[])
{
    if (argc < 2)
        return 1;
    
    const std::string inputPath = argv[1];
    
    size_t bufSize = 0;
    uint8_t* buffer = readAndFillBuffer(inputPath.c_str() , &bufSize);
    
    if (buffer )
    {
        /*
        for (int i=0;i<bufSize;i++)
        {
            if (i%8==0)
                printf("\n");
            
            //printf(" 0x%x " , buffer[i]);
            printf(" 0x%.2x (%c) " , buffer[i], (isprint( buffer[i]) ?buffer[i]: ' ' ));
        }
        
        
        printf("\n");
        */
        
        
        AML::Decompiler decomp;
        
        auto ret = decomp.process(buffer, bufSize);
        
        if (ret )
        {
            const std::string result = decomp.getStringResult();
            
            //std::cout << "Result : " << std::endl;
            //std::cout << result << std::endl;;
            
            size_t lastindex = inputPath.find_last_of(".");
            
            std::string outputPath = "out.dsl";
            if (lastindex != std::string::npos)
            {
                outputPath = inputPath.substr(0, lastindex) + ".dsl";
                
            }
            
            writeStringToFile(decomp.getStringResult(), outputPath);
        }
        
        free(buffer);
        
        return ret;// ? 0 : 10;
    }
    return 2;
}
