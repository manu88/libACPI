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

#include <stdio.h>

#include "AMLParser_Tests.h"
#include "Decompiler_Tests.h"
#include "Helper_Tests.h"
#include "DecompilerInvalid_Tests.h"
#include "Eisaid_Tests.h"
#include "BufferTests.h"
#include "AMLName_Tests.h"
#include <stdlib.h>

/*
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
 */

int main(int argc, const char * argv[])
{
    
    Eisaid_Tests();
    AMLParserTests();
    Helper_Tests();
    DecompilerInvalid_Tests();

    Decompile_Tests();
    Buffers_Tests();
    doAMLNameTests();
    doAMLNameNormalizedTests();
    /*
    size_t size = 0;
    uint8_t* buffer = readAndFillBuffer("/Users/manueldeneu/Documents/projets/Dev/libACPI/TestSuite/51.aml", &size);
    
    if (size)
    {
        for(int i=0;i<size;i++)
        {
            if (i%8==0)
                printf("\n");
            
            printf(" 0x%x, " , buffer[i]);
        }
    }
    */
    return 0;
}
