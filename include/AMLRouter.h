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

#include <stddef.h>
#include <stdint.h>
#include <AMLOperations.h>


AMLOperation _GetNextOp(/*AMLParserState* state ,*/ const uint8_t* buffer , size_t bufSize , size_t* advance ,  size_t offset);

AMLOperation _GetNextEXTOp(/*AMLParserState* state , */const uint8_t* buffer , size_t bufSize , size_t* advance ,  size_t offset);
AMLOperation _GetNextLOPOp(/*AMLParserState* state , */const uint8_t* buffer , size_t bufSize , size_t* advance ,  size_t offset);

size_t _GetPackageLength(const uint8_t* buffer , size_t bufSize , size_t* advance ,  size_t offset);
size_t GetPackageLength(const uint8_t* buffer , size_t bufSize , size_t* advance);

uint8_t GetByteValue(const uint8_t* buffer , size_t bufSize , size_t* advance);
uint32_t GetDWordValue(const uint8_t* buffer , size_t bufSize , size_t* advance);
//uint8_t ExtractInteger( const uint8_t* buffer , size_t bufSize , size_t* advance, uint64_t *toVal);
