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

#include "EISAID.h"

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
int isEisaId( uint64_t val)
{
    return GetEisaId(val) != NULL;
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
