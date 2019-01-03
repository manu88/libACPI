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

#include <assert.h>
#include "AMLHelpers.h"


int GetDWord(const uint8_t* buffer , ACPIDWord* word)
{
    assert(word);
    assert(buffer);
    (void) buffer[3]; // 
    
    union
    {
        uint8_t b[4];
        int32_t v;
    } val;
    
    
    val.b[0] = buffer[0];
    val.b[1] = buffer[1];
    val.b[2] = buffer[2];
    val.b[3] = buffer[3];
    
    *word = val.v;
    return 1;
}
