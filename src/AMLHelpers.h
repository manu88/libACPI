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

#include "ACPIDesc.h"
#include <stdint.h>



int IsName(char character);
int IsRealName(char character);
int GetDWord(const uint8_t* buffer , ACPIDWord* word);

//uint8_t GetNameSize(const uint8_t *buff, uint8_t maxSize );
// returns the corrected string size
uint8_t ExtractName(const uint8_t *buff, size_t size ,char* outChar , uint8_t* advanced);
ssize_t ExtractMaxNameSize(const uint8_t *buff, size_t size  );

uint8_t ExtractNameSize(const uint8_t *buff, size_t size  );
uint8_t ExtractNameString(const uint8_t *buff, size_t size ,char* outChar );


size_t GetInteger( const uint8_t *object,size_t objectSize, uint64_t *integer);

size_t ResolvePath(char *fullpath, const uint8_t *path);

typedef struct _AMLName AMLName;
uint8_t AMLNameCreateFromBuffer( AMLName*name, const uint8_t* buffer , size_t bufferSize );


//DeviceIdentifiers GetDeviceIDFromStr(const uint8_t* name);
