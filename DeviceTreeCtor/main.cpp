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
#include <dirent.h>
#include <fstream>
#include <stdlib.h>
#include "JSONConverter.hpp"

#include "DeviceTreeBuilder.hpp"


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

struct DeviceTreeTester
{
  
    bool operator()(const std::string &amlFilePath , const std::string &jsonToMatch)
    {
        printf("Test AML '%s' JSON '%s'\n" , amlFilePath.c_str() , jsonToMatch.c_str() );
        
        size_t bufSize = 0;
        buffer = readAndFillBuffer(amlFilePath.c_str(), &bufSize);
        
        if (buffer == nullptr || bufSize == 0)
            return false;
        
        
        
        AMLDecompiler decomp;
        AMLDecompilerInit(&decomp);
        
        DeviceTreeBuilder builder(decomp);
        
        AMLParserError err = builder.start( buffer, bufSize);
        
        if (err != AMLParserError_None)
            return false;
        
        
        
        JSONConverter conv(builder.getDeviceTree() );
        
        const auto jsonResult = conv.getJSON();
        
        std::ifstream ifs(jsonToMatch);
        const auto jsonReference = nlohmann::json::parse(ifs);
        
        const auto diff = nlohmann::json::diff(jsonResult, jsonReference);
        
        if (!diff.empty())
        {
            builder.print();
            printf("Got : '%s' \n" , jsonResult.dump(2).c_str());
            printf("Got a diff : '%s'/n" , diff.dump(2).c_str());
        }
        return diff.empty();
        
    }
    
    
    ~DeviceTreeTester()
    {
        if (buffer)
            free(buffer);
    }
    
    uint8_t* buffer;
};




static int ext_match(const char *name, const char *ext)
{
    size_t nl = strlen(name), el = strlen(ext);
    return nl >= el && !strcmp(name + nl - el, ext);
}

static std::vector<std::string> constructFileList(const std::string& fromPath , const std::string& extension)
{
    std::vector<std::string> ret;
    
    
    DIR* testDir = opendir(fromPath.c_str());
    
    if (!testDir)
    {
        return ret;
    }
    
    struct dirent * dp = NULL;
    
    while ((dp = readdir(testDir)) != NULL)
    {
        if (strcmp(dp->d_name, "..") == 0 ||  strcmp(dp->d_name, ".") == 0)
            continue;
        
        
        if (ext_match(dp->d_name, extension.c_str()))
        {
            ret.push_back(dp->d_name);
        }
        
    }
    
    closedir(testDir);
    
    return ret;
}

int main(int argc, const char * argv[])
{

    const char* testDirPath = argv[1];
    
    if (!testDirPath)
    {
        return 1;
    }
    
    std::vector<std::string > amlFiles = constructFileList(testDirPath, "aml");
    

    for( const auto &file : amlFiles)
    {
        const auto fullPath = testDirPath + file;
        
        assert(DeviceTreeTester()( fullPath , fullPath + ".json"));
    }
    
    return 0;
    /*
    size_t bufSize = 0;
    
    const char* file = argv[1];

    uint8_t *dataBuffer = readAndFillBuffer(file , &bufSize);
    
    if (!dataBuffer)
    {
        printf("Error while reading file '%s'\n" , file);
        return 1;
    }
    
    AMLDecompiler decomp;
    AMLDecompilerInit(&decomp);
    
    DeviceTreeBuilder builder(decomp);
    
    AMLParserError err = builder.start( dataBuffer, bufSize);
    
    
    printf("------- Result for file %s (ret code %i) --------------------\n" , file, err);
    
    builder.print();
    
    JSONConverter conv(builder.getDeviceTree() );
    
    printf("JSON : %s\n" , conv.getJSON().c_str() );
    //printf("%s\n" , builder.getResult().c_str());
    
    free(dataBuffer);
    return 0;
     */
}
