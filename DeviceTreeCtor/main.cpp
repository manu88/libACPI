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

#include <stdlib.h>


#include "DeviceTreeBuilder.hpp"

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
    DeviceTreeBuilder decomp;
    
    AMLParserError err = decomp.start( dataBuffer, bufSize);
    
    
    printf("------- Result (ret code %i) --------------------\n" , err);
    printf("%s\n" , decomp.getResult().c_str());
    
    free(dataBuffer);
    return 0;
}
