//
//  ACPIParser.h
//  KernelTaskV1
//
//  Created by Manuel Deneu on 25/12/2018.
//  Copyright © 2018 Manuel Deneu. All rights reserved.
//

#ifndef ACPIParser_h
#define ACPIParser_h

#include <stdio.h>
#include "AMLParser.h"


#define MAX_DEVICES_PER_DOC 20 // FIXME : this is absolutly arbitrary :)
typedef struct
{
    ACPIDefinitionBlock desc;
    
    //ACPIDevice devices[MAX_DEVICES_PER_DOC]; 
    //size_t devicesCount;
    
    // some flags
    uint8_t hasDesc:1;
    
} ACPIDocument; // FIXME : better name than document ! (scope is ambigeous..)


int ACPIDocumentInit(ACPIDocument* doc);

AMLParserError ACPIParseAMLByteCode(ACPIDocument* doc,const uint8_t* buffer , size_t bufferSize);


size_t ACPIDocumentGetDevicesCount(const ACPIDocument* doc);
const TreeElement* ACPIDocumentGetNthDevice(const ACPIDocument* doc , size_t index);

int ACPIDeviceGetName( const TreeElement* element , char* outChar);

#endif /* ACPIParser_h */
