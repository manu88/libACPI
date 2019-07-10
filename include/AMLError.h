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

typedef enum
{
    AMLParserError_None             = 0,
    AMLParserError_BufferTooShort,
    AMLParserError_UnexpectedToken,
    AMLParserError_ElementAllocError,
    AMLParserError_InvalidState,
    AMLParserError_UnexpectedValue,
    AMLParserError_UnexpectedOp,
    AMLParserError_InvalidDefinitionBlock,
    
    
    AMLParserError_ValueNotFound,
    AMLParserError_ValueFound,
    AMLParserUserAbord, // might be returned from Callbacks
    
    
} AMLParserError;
