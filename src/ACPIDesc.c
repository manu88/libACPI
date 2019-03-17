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
#include <stdio.h>
#include <assert.h>
#include "ACPIDesc.h"
#include "AMLParser.h"


uint8_t ACPIFieldElementIsOffset(const ACPIFieldElement* element)
{
    assert(element);
    return element->name[0] == 0 && element->value > 255;
}
/*
int ACPIDescriptionInit(ACPIDescription* desc)
{
    memset(desc, 0, sizeof(ACPIDescription));
    return 1;
}
 

int ACPIObjectInit(ACPIObject* obj)
{
    memset(obj, 0, sizeof(ACPIObject));
    
    obj->indexParent = -1;
    obj->indexScope = -1;
    
    return 1;
}
 
int ACPIDeviceGetTypeName( const ACPIObject* object , char* bufOut , size_t maxChar)
{
    
    assert(object);
    assert(object->start);
    
    uint8_t* ptr = object->start;
    int acc=0;
    
    while (*ptr != AML_OP_NameChar )
    {
        bufOut[acc] = *ptr;
        if( ++acc >=maxChar)
        {
            return 0;
        }
        
        ptr++;
    }
    
    bufOut[acc] = 0;
    
    return 1;
}

void* ACPIDeviceGetNameAddr(const ACPIObject* object)
{
    assert(object);
    assert(object->start);
    
    if (object->type != ACPIObject_Type_Name)
    {
        return NULL;
    }
    
    int ret = strncmp((const char*)object->start, "_ADR", 4);// strcmp( (const char*)object->start, "Prout");

    if (ret == 0)
    {
        const uint8_t* addr = object->start + strlen("_ADR");
        
        assert(*addr == AML_OP_DWordPrefix);
        
        union DWordVal
        {
            int8_t bytes[4];
            int32_t v;
        }  val;
        
        val.bytes[0] = addr[1];
        val.bytes[1] = addr[2];
        val.bytes[2] = addr[3];
        val.bytes[3] = addr[4];
        
        return (void*) val.v;
        //0xFFFFFFFF
        //0x100010000
        //0x00010000
        //
    }
    
    return NULL;
}
     */
