//
//  main.c
//  DomTest
//
//  Created by Manuel Deneu on 25/05/2019.
//  Copyright © 2019 Manuel Deneu. All rights reserved.
//
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <AMLDecompiler.h>
#include "ACPIDom.h"

static uint8_t* readAndFillBuffer(const char* fromFile , size_t* bufSize)
{
    
    FILE *fileptr;
    fileptr = fopen(fromFile, "rb");  // Open the file in binary mode
    
    if (fileptr == NULL)
        return NULL;
    
    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    *bufSize = ftell(fileptr);             // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file
    
    uint8_t *buffer = (uint8_t *)malloc((*bufSize+1)*sizeof(uint8_t)); // Enough memory for file + \0
    fread(buffer, *bufSize, 1, fileptr); // Read in the entire file
    fclose(fileptr); // Close the file
    
    
    return buffer;
}

int main(int argc, const char * argv[])
{
    size_t bufSize = 0;
    uint8_t* buffer = readAndFillBuffer(argv[1], &bufSize);
    
    ACPIDom dom = {0};
    ACPIDomParseBuffer( &dom, buffer, bufSize);
    
    assert(dom.root.obj.pos);
    assert(dom.root.obj.size);
    
    printf("Root size %zi\n" ,dom.root.obj.size);
    char *rootName = AMLNameConstructNormalized(&dom.root.name);
    assert(rootName);
    assert(strcmp(rootName, "PCI0") == 0);
    free(rootName);
    
    ACPINamedResource res = {0};
    AMLParserError err =  ACPIScopeGetNamedResource(&dom.root, "_HID" , &res);
    
    assert(res.data);
    assert(res.type != ACPIObject_Type_Unknown);
    if( res.data)
    {
        printf("Object has name _HID type = %i\n" , res.type);
        
        if( res.type == ACPIObject_Type_NumericValue)
        {
            printf("Num value %x\n" , (uint64_t) res.data );
        }
    }
    
    
    
    return 0;
}
