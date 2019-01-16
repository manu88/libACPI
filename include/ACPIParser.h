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

#pragma once

#include <stdio.h>
#include <AMLParser.h>


#define MAX_DEVICES_PER_DOC 20 // FIXME : this is absolutly arbitrary :)
typedef struct
{
    ACPIDefinitionBlock desc;
    
    //ACPIDevice devices[MAX_DEVICES_PER_DOC]; 
    //size_t devicesCount;
    
    // some flags
    uint8_t hasDesc:1;
    
    TreeElement* root;
    
} ACPIDocument; // FIXME : better name than document ! (scope is ambigeous..)


int ACPIDocumentInit(ACPIDocument* doc);

AMLParserError ACPIParseAMLByteCode(ACPIDocument* doc,const uint8_t* buffer , size_t bufferSize);

TreeElement* ACPIDocumentGetRoot(const ACPIDocument* doc);
size_t ACPIDocumentGetDevicesCount(const ACPIDocument* doc);
const TreeElement* ACPIDocumentGetNthDevice(const ACPIDocument* doc , size_t index);

//int ACPIDeviceGetName( const TreeElement* element , char* outChar);

//int ACPIScopeGetLocation(const uint8_t *buff, size_t size , char* outChar);

size_t ACPIDeviceGetNamedObjectsCount(const TreeElement* element);

