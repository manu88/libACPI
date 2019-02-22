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

#include <ctype.h> // isprint
#include <stdio.h> // snprintf
#include <stdlib.h> // strtoll
#include <string.h> // strlen
#include "EISAID.h"

int getEisaidString( uint64_t value , char* toBuff)
{
    const uint64_t vendor  = value & 0xFFFF;
    const uint64_t device = value >> 16;
    const uint8_t device1 = device & 0xff;
    const uint8_t device2 = device >> 8;
    
    const uint64_t vendorRev = ((vendor & 0xFF) << 8) | vendor >> 8;
    const uint8_t vendor1   = ((vendorRev >> 10)&0x1f)+64;
    const uint8_t vendor2   = ((vendorRev >> 5)&0x1f)+64;
    const uint8_t vendor3   = ((vendorRev >> 0)&0x1f)+64;
    
    int ret = snprintf(toBuff, 8, "%c%c%c%.2X%.2X" ,  vendor1 , vendor2 , vendor3 , device1 , device2);
    
    return ret == 7;
}

int getValueFromEisaidString(const char* buffer , uint64_t* toVal)
{
    if (strlen(buffer) <7)
        return 0;
    
    union
    {
        uint16_t b[2];
        uint32_t v;
        
    }conv;
    
    const uint64_t vendorRev = ((buffer[0] - 64) << 10) | ((buffer[1] - 64) << 5) | buffer[2] - 64;
    
    conv.b[0]  = ((vendorRev & 0xFF) << 8) | vendorRev >> 8;
    
    const  char p1[5] = { buffer[5] , buffer[6] , buffer[3] , buffer[4] , 0};
    
    conv.b[1] =(uint16_t) strtoul(p1, NULL, 16);
    
    *toVal = conv.v;
    return 1;
}
/*
const char* GetEisaId( uint64_t val)
{
    switch (val)
    {
        case PNP0A01:
            return "PNP0A01";
        case PNP0A03:
            return "PNP0A03";
        case PNP0103:
            return "PNP0103";
        case PNP0B00:
            return "PNP0B00";
        case PNP0303:
            return "PNP0303";
        case PNP0F13:
            return "PNP0F13";
        case PNP0700:
            return "PNP0700";
        case PNP0400:
            return "PNP0400";
        case PNP0501:
            return "PNP0501";
        case PNP0C0F:
            return "PNP0C0F";
            
        default:
            break;
    }
    
    return NULL;
}
*/
int isUpperCase(int ch)
{
    return (ch >= 'A' && ch <= 'Z');
}
int isEisaId( uint64_t val)
{
    char buf[8] = {0};
    if (getEisaidString(val, buf)
        &&  isUpperCase(buf[0]) &&  isUpperCase(buf[1]) &&  isUpperCase(buf[2])
        )
    {
        uint64_t retVal = 0;
        if (getValueFromEisaidString(buf, &retVal) && retVal == val)
        {
            
            return 1;
        }
    }
    return 0;
    //return GetEisaId(val) != NULL;
    /*
    switch (val)
    {
        case PNP0A01:
        case PNP0A03:
        case PNP0103:
        case PNP0B00:
        case PNP0303:
        case PNP0F13:
        case PNP0700:
            return 1;
            
            
        default:
            break;
    }
    
    return 0;
     */
}
