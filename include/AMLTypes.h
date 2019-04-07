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

#include <stdint.h>
#include <stddef.h>

#define SCOPE_STR_SIZE 512

typedef struct// _AMLName
{
    const uint8_t*  originalBuf;
    size_t          originalBufSize;
} AMLName;



uint8_t AMLNameHasPrefixRoot( const AMLName*name);

// count the `^`
uint8_t AMLNameCountParents( const AMLName*name);
uint8_t AMLNameCountSegments( const AMLName*name);
uint8_t AMLNameGetSegment(const AMLName*name, uint8_t index , char toBuffer[5] );

// returned pointer is malloced and needs to be freed
char* AMLNameConstructNormalized( const AMLName* name);
