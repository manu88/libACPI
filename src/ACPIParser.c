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

#include <string.h>
#include <assert.h>
#include "ACPIParser.h"
#include "AMLHelpers.h"

#define MAXElements 64
static TreeElement Elements[MAXElements] = {0};
static int NextElementIndex = 0;


int ACPIDocumentInit(ACPIDocument* doc)
{
    memset(doc , 0 , sizeof(ACPIDocument) );
    
    NextElementIndex = 0;
    memset(Elements, 0, sizeof(TreeElement) *MAXElements );
    return 1;
}

static TreeElement* _AllocateElement(AMLParserState* parser , ACPIObject_Type forObjectType , TreeElement*parent , const uint8_t* bufferPos , size_t bufferSize)
{
    
    
    assert(parent || (parent == NULL &&  forObjectType == ACPIObject_Type_Root));
    assert(parser);
    //printf("Allocate request for type %i (count %i)\n" , forObjectType , NextElementIndex);
    
    ACPIDocument* doc = (ACPIDocument*) parser->userData;
    assert(doc);
    
    if (NextElementIndex >= MAXElements)
    {
        return NULL;
    }
    TreeElement* next = &Elements[NextElementIndex++];
    
    if (forObjectType == ACPIObject_Type_Root)
    {
        assert(doc->root == NULL);
        doc->root = next;
    }
    return next;
}

static AMLParserError _DidReadDevice(AMLParserState* parser  ,const ACPIDevice*device)
{
    assert(parser);
    assert(device);
    
    
    ACPIDocument* doc = (ACPIDocument*) parser->userData;
    assert(doc);
    
    //memcpy(&doc->devices[ ACPIDocumentGetDevicesCount(doc)/* doc->devicesCount*/], device, sizeof(ACPIDevice));
    //doc->devices[doc->devicesCount] = *device;
    //doc->devicesCount++;
    
    return AMLParserError_None;
}

static AMLParserError _DidReadDefBlock(AMLParserState* parser,const ACPIDefinitionBlock* desc)
{
    assert(parser);
    assert(desc);
    
    
    ACPIDocument* doc = (ACPIDocument*) parser->userData;
    assert(doc);
    doc->desc = *desc;
    doc->hasDesc = 1;
    return AMLParserError_None;
}

AMLParserError ACPIParseAMLByteCode(ACPIDocument* doc,const uint8_t* buffer , size_t bufferSize)
{
    AMLParserState parser;
    
    AMLParserInit(&parser);
    parser.startBuffer = buffer;
    parser.totalSize = bufferSize;
    
    parser.userData = doc;
    
    parser.callbacks.DidReadObject = _DidReadDevice;
    parser.callbacks.DidReadDefBlock = _DidReadDefBlock;
    parser.callbacks.AllocateElement = _AllocateElement;
    
    return AMLParserProcessBuffer(&parser , buffer , bufferSize);
}

TreeElement* ACPIDocumentGetRoot(const ACPIDocument* doc)
{
    return doc->root;
}



int ACPIScopeGetLocation(const uint8_t *buff, size_t size ,char* outChar)
{
    return ExtractName(buff, size, outChar);
}

int ACPIDeviceGetName( const TreeElement* element , char* outChar)
{
    return ExtractName(element->ptr, element->size, outChar);
}

size_t ACPIDeviceGetNamedObjectsCount(const TreeElement* element)
{
    return 0;
}

size_t ACPIDocumentGetDevicesCount(const ACPIDocument* doc)
{
    
    size_t n = 0;
    
    for (int i = 0;i<NextElementIndex;i++)
    {
        TreeElement* el = &Elements[i];
        if (el->type == ACPIObject_Type_Device)
        {
            assert(el->parent !=  NULL);
            n++;
        }
    }
    /*
    while (n<MAX_DEVICES_PER_DOC)
    {
        if (doc->devices[n].id[0] == 0)
        {
            break;
        }
        n++;
    }
     */
    
    return n;
}

const TreeElement* ACPIDocumentGetNthDevice(const ACPIDocument* doc, size_t index)
{
    size_t n = 0;
    
    for (int i = 0;i<NextElementIndex;i++)
    {
        TreeElement* el = &Elements[i];
        if (el->type == ACPIObject_Type_Device)
        {
            assert(el->parent !=  NULL);
            if(n++ == index)
            {
                return el;
            }
        }
    }
    
    return NULL;
}
