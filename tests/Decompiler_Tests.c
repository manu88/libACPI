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

#include <stdio.h>
#include <assert.h>
#include "AMLDecompiler.h"
#include "Decompiler_Tests.h"




static const uint8_t table[] =
{
    0x10,0x49,0x4,0x5c,0x0,0x5b,0x80,0x44,0x42,0x47,0x5f,0x1,0xb,0x2,0x4,0x1,
    0x5b,0x81,0xb,0x44,0x42,0x47,0x5f,0x1,0x44,0x42,0x47,0x42,0x8,0x14,0x2c,0x44,
    0x42,0x55,0x47,0x1,0x98,0x68,0x60,0x96,0x60,0x60,0x74,0x87,0x60,0x1,0x61,0x70,
    0x0,0x62,0xa2,0x10,0x95,0x62,0x61,0x70,0x83,0x88,0x60,0x62,0x0,0x44,0x42,0x47,
    0x42,0x75,0x62,0x70,0xa,0xa,0x44,0x42,0x47,0x42,0x10,0x22,0x5f,0x53,0x42,0x5f,
    0x5b,0x82,0x1b,0x50,0x43,0x49,0x30,0x8,0x5f,0x48,0x49,0x44,0xc,0x41,0xd0,0xa,
    0x3,0x8,0x5f,0x41,0x44,0x52,0x0,0x8,0x5f,0x55,0x49,0x44,0x1,0x10,0x4d,0x8,
    0x5f,0x53,0x42,0x5f,0x5b,0x82,0x45,0x8,0x48,0x50,0x45,0x54,0x8,0x5f,0x48,0x49,
    0x44,0xc,0x41,0xd0,0x1,0x3,0x8,0x5f,0x55,0x49,0x44,0x0,0x5b,0x80,0x48,0x50,
    0x54,0x4d,0x0,0xc,0x0,0x0,0xd0,0xfe,0xb,0x0,0x4,0x5b,0x81,0x10,0x48,0x50,
    0x54,0x4d,0x13,0x56,0x45,0x4e,0x44,0x20,0x50,0x52,0x44,0x5f,0x20,0x14,0x36,0x5f,
    0x53,0x54,0x41,0x0,0x70,0x56,0x45,0x4e,0x44,0x60,0x70,0x50,0x52,0x44,0x5f,0x61,
    0x7a,0x60,0xa,0x10,0x60,0xa0,0xc,0x91,0x93,0x60,0x0,0x93,0x60,0xb,0xff,0xff,
    0xa4,0x0,0xa0,0xe,0x91,0x93,0x61,0x0,0x94,0x61,0xc,0x0,0xe1,0xf5,0x5,0xa4,
    0x0,0xa4,0xa,0xf,0x8,0x5f,0x43,0x52,0x53,0x11,0x11,0xa,0xe,0x86,0x9,0x0,
    0x0,0x0,0x0,0xd0,0xfe,0x0,0x4,0x0,0x0,0x79,0x0,0x10,0x25,0x2e,0x5f,0x53,
    0x42,0x5f,0x50,0x43,0x49,0x30,0x5b,0x82,0x19,0x50,0x58,0x31,0x33,0x8,0x5f,0x41,
    0x44,0x52,0xc,0x3,0x0,0x1,0x0,0x5b,0x80,0x50,0x31,0x33,0x43,0x2,0x0,0xa,
    0xff,0x10,0x40,0x5,0x2e,0x5f,0x53,0x42,0x5f,0x50,0x43,0x49,0x30,0x5b,0x82,0x43,
    0x4,0x49,0x53,0x41,0x5f,0x8,0x5f,0x41,0x44,0x52,0xc,0x0,0x0,0x1,0x0,0x5b,
    0x80,0x50,0x34,0x30,0x43,0x2,0xa,0x60,0xa,0x4,0x5b,0x81,0x26,0x5e,0x2e,0x50,
    0x58,0x31,0x33,0x50,0x31,0x33,0x43,0x0,0x0,0x48,0x2f,0x0,0x7,0x4c,0x50,0x45,
    0x4e,0x1,0x0,0x38,0x0,0x3,0x43,0x41,0x45,0x4e,0x1,0x0,0x3,0x43,0x42,0x45,
    0x4e,0x1,0x10,0x43,0x1f,0x2f,0x3,0x5f,0x53,0x42,0x5f,0x50,0x43,0x49,0x30,0x49,
    0x53,0x41,0x5f,0x5b,0x82,0x2d,0x52,0x54,0x43,0x5f,0x8,0x5f,0x48,0x49,0x44,0xc,
    0x41,0xd0,0xb,0x0,0x8,0x5f,0x43,0x52,0x53,0x11,0x18,0xa,0x15,0x47,0x1,0x70,
    0x0,0x70,0x0,0x10,0x2,0x22,0x0,0x1,0x47,0x1,0x72,0x0,0x72,0x0,0x2,0x6,
    0x79,0x0,0x5b,0x82,0x37,0x4b,0x42,0x44,0x5f,0x8,0x5f,0x48,0x49,0x44,0xc,0x41,
    0xd0,0x3,0x3,0x14,0x9,0x5f,0x53,0x54,0x41,0x0,0xa4,0xa,0xf,0x8,0x5f,0x43,
    0x52,0x53,0x11,0x18,0xa,0x15,0x47,0x1,0x60,0x0,0x60,0x0,0x1,0x1,0x47,0x1,
    0x64,0x0,0x64,0x0,0x1,0x1,0x22,0x2,0x0,0x79,0x0,0x5b,0x82,0x27,0x4d,0x4f,
    0x55,0x5f,0x8,0x5f,0x48,0x49,0x44,0xc,0x41,0xd0,0xf,0x13,0x14,0x9,0x5f,0x53,
    0x54,0x41,0x0,0xa4,0xa,0xf,0x8,0x5f,0x43,0x52,0x53,0x11,0x8,0xa,0x5,0x22,
    0x0,0x10,0x79,0x0,0x5b,0x82,0x41,0x8,0x46,0x44,0x43,0x30,0x8,0x5f,0x48,0x49,
    0x44,0xc,0x41,0xd0,0x7,0x0,0x8,0x5f,0x43,0x52,0x53,0x11,0x1b,0xa,0x18,0x47,
    0x1,0xf2,0x3,0xf2,0x3,0x0,0x4,0x47,0x1,0xf7,0x3,0xf7,0x3,0x0,0x1,0x22,
    0x40,0x0,0x2a,0x4,0x0,0x79,0x0,0x5b,0x82,0x31,0x46,0x4c,0x50,0x41,0x8,0x5f,
    0x41,0x44,0x52,0x0,0x8,0x5f,0x46,0x44,0x49,0x12,0x20,0x10,0x0,0xa,0x5,0xa,
    0x4f,0xa,0x30,0x1,0xa,0xaf,0xa,0x2,0xa,0x25,0xa,0x2,0xa,0x12,0xa,0x1b,
    0xa,0xff,0xa,0x6c,0xa,0xf6,0xa,0xf,0xa,0x8,0x8,0x5f,0x46,0x44,0x45,0x11,
    0x17,0xa,0x14,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x2,0x0,0x0,0x0,0x5b,0x82,0x3e,0x4c,0x50,0x54,0x5f,0x8,0x5f,
    0x48,0x49,0x44,0xc,0x41,0xd0,0x4,0x0,0x14,0x18,0x5f,0x53,0x54,0x41,0x0,0x70,
    0x4c,0x50,0x45,0x4e,0x60,0xa0,0x6,0x93,0x60,0x0,0xa4,0x0,0xa1,0x4,0xa4,0xa,
    0xf,0x8,0x5f,0x43,0x52,0x53,0x11,0x10,0xa,0xd,0x47,0x1,0x78,0x3,0x78,0x3,
    0x8,0x8,0x22,0x80,0x0,0x79,0x0,0x5b,0x82,0x45,0x4,0x43,0x4f,0x4d,0x31,0x8,
    0x5f,0x48,0x49,0x44,0xc,0x41,0xd0,0x5,0x1,0x8,0x5f,0x55,0x49,0x44,0x1,0x14,
    0x18,0x5f,0x53,0x54,0x41,0x0,0x70,0x43,0x41,0x45,0x4e,0x60,0xa0,0x6,0x93,0x60,
    0x0,0xa4,0x0,0xa1,0x4,0xa4,0xa,0xf,0x8,0x5f,0x43,0x52,0x53,0x11,0x10,0xa,
    0xd,0x47,0x1,0xf8,0x3,0xf8,0x3,0x0,0x8,0x22,0x10,0x0,0x79,0x0,0x5b,0x82,
    0x46,0x4,0x43,0x4f,0x4d,0x32,0x8,0x5f,0x48,0x49,0x44,0xc,0x41,0xd0,0x5,0x1,
    0x8,0x5f,0x55,0x49,0x44,0xa,0x2,0x14,0x18,0x5f,0x53,0x54,0x41,0x0,0x70,0x43,
    0x42,0x45,0x4e,0x60,0xa0,0x6,0x93,0x60,0x0,0xa4,0x0,0xa1,0x4,0xa4,0xa,0xf,
    0x8,0x5f,0x43,0x52,0x53,0x11,0x10,0xa,0xd,0x47,0x1,0xf8,0x2,0xf8,0x2,0x0,
    0x8,0x22,0x8,0x0,0x79,0x0,0x10,0x48,0x8,0x2e,0x5f,0x53,0x42,0x5f,0x50,0x43,
    0x49,0x30,0x5b,0x80,0x50,0x43,0x53,0x54,0x1,0xb,0x0,0xae,0xa,0x8,0x5b,0x81,
    0x10,0x50,0x43,0x53,0x54,0x43,0x50,0x43,0x49,0x55,0x20,0x50,0x43,0x49,0x44,0x20,
    0x5b,0x80,0x53,0x45,0x4a,0x5f,0x1,0xb,0x8,0xae,0xa,0x4,0x5b,0x81,0xb,0x53,
    0x45,0x4a,0x5f,0x43,0x42,0x30,0x45,0x4a,0x20,0x5b,0x80,0x42,0x4e,0x4d,0x52,0x1,
    0xb,0x10,0xae,0xa,0x4,0x5b,0x81,0xb,0x42,0x4e,0x4d,0x52,0x43,0x42,0x4e,0x55,
    0x4d,0x20,0x5b,0x1,0x42,0x4c,0x43,0x4b,0x0,0x14,0x25,0x50,0x43,0x45,0x4a,0x2,
    0x5b,0x23,0x42,0x4c,0x43,0x4b,0xff,0xff,0x70,0x68,0x42,0x4e,0x55,0x4d,0x70,0x79,
    0x1,0x69,0x0,0x42,0x30,0x45,0x4a,0x5b,0x27,0x42,0x4c,0x43,0x4b,0xa4,0x0,0x10,
    0x4e,0x36,0x5f,0x53,0x42,0x5f,0x10,0x4b,0xa,0x50,0x43,0x49,0x30,0x14,0x44,0xa,
    0x5f,0x50,0x52,0x54,0x0,0x70,0x12,0x2,0x80,0x60,0x70,0x0,0x61,0xa2,0x42,0x9,
    0x95,0x61,0xa,0x80,0x70,0x7a,0x61,0xa,0x2,0x0,0x62,0x70,0x7b,0x72,0x61,0x62,
    0x0,0xa,0x3,0x0,0x63,0xa0,0x10,0x93,0x63,0x0,0x70,0x12,0x9,0x4,0x0,0x0,
    0x4c,0x4e,0x4b,0x44,0x0,0x64,0xa0,0x24,0x93,0x63,0x1,0xa0,0x11,0x93,0x61,0xa,
    0x4,0x70,0x12,0x9,0x4,0x0,0x0,0x4c,0x4e,0x4b,0x53,0x0,0x64,0xa1,0xd,0x70,
    0x12,0x9,0x4,0x0,0x0,0x4c,0x4e,0x4b,0x41,0x0,0x64,0xa0,0x11,0x93,0x63,0xa,
    0x2,0x70,0x12,0x9,0x4,0x0,0x0,0x4c,0x4e,0x4b,0x42,0x0,0x64,0xa0,0x11,0x93,
    0x63,0xa,0x3,0x70,0x12,0x9,0x4,0x0,0x0,0x4c,0x4e,0x4b,0x43,0x0,0x64,0x70,
    0x7d,0x79,0x62,0xa,0x10,0x0,0xb,0xff,0xff,0x0,0x88,0x64,0x0,0x0,0x70,0x7b,
    0x61,0xa,0x3,0x0,0x88,0x64,0x1,0x0,0x70,0x64,0x88,0x60,0x61,0x0,0x75,0x61,
    0xa4,0x60,0x5b,0x81,0x24,0x2f,0x3,0x50,0x43,0x49,0x30,0x49,0x53,0x41,0x5f,0x50,
    0x34,0x30,0x43,0x1,0x50,0x52,0x51,0x30,0x8,0x50,0x52,0x51,0x31,0x8,0x50,0x52,
    0x51,0x32,0x8,0x50,0x52,0x51,0x33,0x8,0x14,0x13,0x49,0x51,0x53,0x54,0x1,0xa0,
    0x9,0x7b,0xa,0x80,0x68,0x0,0xa4,0xa,0x9,0xa4,0xa,0xb,0x14,0x36,0x49,0x51,
    0x43,0x52,0x9,0x8,0x50,0x52,0x52,0x30,0x11,0xe,0xa,0xb,0x89,0x6,0x0,0x9,
    0x1,0x0,0x0,0x0,0x0,0x79,0x0,0x8a,0x50,0x52,0x52,0x30,0xa,0x5,0x50,0x52,
    0x52,0x49,0xa0,0xb,0x95,0x68,0xa,0x80,0x70,0x68,0x50,0x52,0x52,0x49,0xa4,0x50,
    0x52,0x52,0x30,0x5b,0x82,0x4c,0x7,0x4c,0x4e,0x4b,0x41,0x8,0x5f,0x48,0x49,0x44,
    0xc,0x41,0xd0,0xc,0xf,0x8,0x5f,0x55,0x49,0x44,0x0,0x8,0x5f,0x50,0x52,0x53,
    0x11,0x16,0xa,0x13,0x89,0xe,0x0,0x9,0x3,0x5,0x0,0x0,0x0,0xa,0x0,0x0,
    0x0,0xb,0x0,0x0,0x0,0x79,0x0,0x14,0xf,0x5f,0x53,0x54,0x41,0x0,0xa4,0x49,
    0x51,0x53,0x54,0x50,0x52,0x51,0x30,0x14,0x11,0x5f,0x44,0x49,0x53,0x0,0x7d,0x50,
    0x52,0x51,0x30,0xa,0x80,0x50,0x52,0x51,0x30,0x14,0xf,0x5f,0x43,0x52,0x53,0x0,
    0xa4,0x49,0x51,0x43,0x52,0x50,0x52,0x51,0x30,0x14,0x17,0x5f,0x53,0x52,0x53,0x1,
    0x8a,0x68,0xa,0x5,0x50,0x52,0x52,0x49,0x70,0x50,0x52,0x52,0x49,0x50,0x52,0x51,
    0x30,0x5b,0x82,0x4c,0x7,0x4c,0x4e,0x4b,0x42,0x8,0x5f,0x48,0x49,0x44,0xc,0x41,
    0xd0,0xc,0xf,0x8,0x5f,0x55,0x49,0x44,0x1,0x8,0x5f,0x50,0x52,0x53,0x11,0x16,
    0xa,0x13,0x89,0xe,0x0,0x9,0x3,0x5,0x0,0x0,0x0,0xa,0x0,0x0,0x0,0xb,
    0x0,0x0,0x0,0x79,0x0,0x14,0xf,0x5f,0x53,0x54,0x41,0x0,0xa4,0x49,0x51,0x53,
    0x54,0x50,0x52,0x51,0x31,0x14,0x11,0x5f,0x44,0x49,0x53,0x0,0x7d,0x50,0x52,0x51,
    0x31,0xa,0x80,0x50,0x52,0x51,0x31,0x14,0xf,0x5f,0x43,0x52,0x53,0x0,0xa4,0x49,
    0x51,0x43,0x52,0x50,0x52,0x51,0x31,0x14,0x17,0x5f,0x53,0x52,0x53,0x1,0x8a,0x68,
    0xa,0x5,0x50,0x52,0x52,0x49,0x70,0x50,0x52,0x52,0x49,0x50,0x52,0x51,0x31,0x5b,
    0x82,0x4d,0x7,0x4c,0x4e,0x4b,0x43,0x8,0x5f,0x48,0x49,0x44,0xc,0x41,0xd0,0xc,
    0xf,0x8,0x5f,0x55,0x49,0x44,0xa,0x2,0x8,0x5f,0x50,0x52,0x53,0x11,0x16,0xa,
    0x13,0x89,0xe,0x0,0x9,0x3,0x5,0x0,0x0,0x0,0xa,0x0,0x0,0x0,0xb,0x0,
    0x0,0x0,0x79,0x0,0x14,0xf,0x5f,0x53,0x54,0x41,0x0,0xa4,0x49,0x51,0x53,0x54,
    0x50,0x52,0x51,0x32,0x14,0x11,0x5f,0x44,0x49,0x53,0x0,0x7d,0x50,0x52,0x51,0x32,
    0xa,0x80,0x50,0x52,0x51,0x32,0x14,0xf,0x5f,0x43,0x52,0x53,0x0,0xa4,0x49,0x51,
    0x43,0x52,0x50,0x52,0x51,0x32,0x14,0x17,0x5f,0x53,0x52,0x53,0x1,0x8a,0x68,0xa,
    0x5,0x50,0x52,0x52,0x49,0x70,0x50,0x52,0x52,0x49,0x50,0x52,0x51,0x32,0x5b,0x82,
    0x4d,0x7,0x4c,0x4e,0x4b,0x44,0x8,0x5f,0x48,0x49,0x44,0xc,0x41,0xd0,0xc,0xf,
    0x8,0x5f,0x55,0x49,0x44,0xa,0x3,0x8,0x5f,0x50,0x52,0x53,0x11,0x16,0xa,0x13,
    0x89,0xe,0x0,0x9,0x3,0x5,0x0,0x0,0x0,0xa,0x0,0x0,0x0,0xb,0x0,0x0,
    0x0,0x79,0x0,0x14,0xf,0x5f,0x53,0x54,0x41,0x0,0xa4,0x49,0x51,0x53,0x54,0x50,
    0x52,0x51,0x33,0x14,0x11,0x5f,0x44,0x49,0x53,0x0,0x7d,0x50,0x52,0x51,0x33,0xa,
    0x80,0x50,0x52,0x51,0x33,0x14,0xf,0x5f,0x43,0x52,0x53,0x0,0xa4,0x49,0x51,0x43,
    0x52,0x50,0x52,0x51,0x33,0x14,0x17,0x5f,0x53,0x52,0x53,0x1,0x8a,0x68,0xa,0x5,
    0x50,0x52,0x52,0x49,0x70,0x50,0x52,0x52,0x49,0x50,0x52,0x51,0x33,0x5b,0x82,0x4f,
    0x4,0x4c,0x4e,0x4b,0x53,0x8,0x5f,0x48,0x49,0x44,0xc,0x41,0xd0,0xc,0xf,0x8,
    0x5f,0x55,0x49,0x44,0xa,0x4,0x8,0x5f,0x50,0x52,0x53,0x11,0xe,0xa,0xb,0x89,
    0x6,0x0,0x9,0x1,0x9,0x0,0x0,0x0,0x79,0x0,0x14,0x9,0x5f,0x53,0x54,0x41,
    0x0,0xa4,0xa,0xb,0x14,0x6,0x5f,0x44,0x49,0x53,0x0,0x14,0xb,0x5f,0x43,0x52,
    0x53,0x0,0xa4,0x5f,0x50,0x52,0x53,0x14,0x6,0x5f,0x53,0x52,0x53,0x1,0x10,0x49,
    0x37,0x5f,0x53,0x42,0x5f,0x5b,0x82,0x43,0xa,0x5c,0x2f,0x3,0x5f,0x53,0x42,0x5f,
    0x50,0x43,0x49,0x30,0x50,0x52,0x45,0x53,0x8,0x5f,0x48,0x49,0x44,0xc,0x41,0xd0,
    0xa,0x6,0x8,0x5f,0x55,0x49,0x44,0xd,0x43,0x50,0x55,0x20,0x48,0x6f,0x74,0x70,
    0x6c,0x75,0x67,0x20,0x72,0x65,0x73,0x6f,0x75,0x72,0x63,0x65,0x73,0x0,0x5b,0x1,
    0x43,0x50,0x4c,0x4b,0x0,0x8,0x5f,0x43,0x52,0x53,0x11,0xd,0xa,0xa,0x47,0x1,
    0x0,0xaf,0x0,0xaf,0x1,0xc,0x79,0x0,0x5b,0x80,0x50,0x52,0x53,0x54,0x1,0xb,
    0x0,0xaf,0xa,0xc,0x5b,0x81,0x23,0x50,0x52,0x53,0x54,0x41,0x0,0x20,0x43,0x50,
    0x45,0x4e,0x1,0x43,0x49,0x4e,0x53,0x1,0x43,0x52,0x4d,0x56,0x1,0x43,0x45,0x4a,
    0x30,0x1,0x0,0x4,0x43,0x43,0x4d,0x44,0x8,0x5b,0x81,0x12,0x50,0x52,0x53,0x54,
    0x3,0x43,0x53,0x45,0x4c,0x20,0x0,0x20,0x43,0x44,0x41,0x54,0x20,0x14,0xc,0x5f,
    0x49,0x4e,0x49,0x8,0x70,0x0,0x43,0x53,0x45,0x4c,0x5b,0x82,0x4c,0x2c,0x5c,0x2e,
    0x5f,0x53,0x42,0x5f,0x43,0x50,0x55,0x53,0x8,0x5f,0x48,0x49,0x44,0xd,0x41,0x43,
    0x50,0x49,0x30,0x30,0x31,0x30,0x0,0x8,0x5f,0x43,0x49,0x44,0xc,0x41,0xd0,0xa,
    0x5,0x14,0x11,0x43,0x54,0x46,0x59,0x2,0xa0,0xa,0x93,0x68,0x0,0x86,0x43,0x30,
    0x30,0x30,0x69,0x14,0x48,0x6,0x43,0x53,0x54,0x41,0x9,0x5b,0x23,0x5c,0x2f,0x4,
    0x5f,0x53,0x42,0x5f,0x50,0x43,0x49,0x30,0x50,0x52,0x45,0x53,0x43,0x50,0x4c,0x4b,
    0xff,0xff,0x70,0x68,0x5c,0x2f,0x4,0x5f,0x53,0x42,0x5f,0x50,0x43,0x49,0x30,0x50,
    0x52,0x45,0x53,0x43,0x53,0x45,0x4c,0x70,0x0,0x60,0xa0,0x1a,0x93,0x5c,0x2f,0x4,
    0x5f,0x53,0x42,0x5f,0x50,0x43,0x49,0x30,0x50,0x52,0x45,0x53,0x43,0x50,0x45,0x4e,
    0x1,0x70,0xa,0xf,0x60,0x5b,0x27,0x5c,0x2f,0x4,0x5f,0x53,0x42,0x5f,0x50,0x43,
    0x49,0x30,0x50,0x52,0x45,0x53,0x43,0x50,0x4c,0x4b,0xa4,0x60,0x14,0x4d,0x5,0x43,
    0x45,0x4a,0x30,0x9,0x5b,0x23,0x5c,0x2f,0x4,0x5f,0x53,0x42,0x5f,0x50,0x43,0x49,
    0x30,0x50,0x52,0x45,0x53,0x43,0x50,0x4c,0x4b,0xff,0xff,0x70,0x68,0x5c,0x2f,0x4,
    0x5f,0x53,0x42,0x5f,0x50,0x43,0x49,0x30,0x50,0x52,0x45,0x53,0x43,0x53,0x45,0x4c,
    0x70,0x1,0x5c,0x2f,0x4,0x5f,0x53,0x42,0x5f,0x50,0x43,0x49,0x30,0x50,0x52,0x45,
    0x53,0x43,0x45,0x4a,0x30,0x5b,0x27,0x5c,0x2f,0x4,0x5f,0x53,0x42,0x5f,0x50,0x43,
    0x49,0x30,0x50,0x52,0x45,0x53,0x43,0x50,0x4c,0x4b,0x14,0x48,0xe,0x43,0x53,0x43,
    0x4e,0x8,0x5b,0x23,0x5c,0x2f,0x4,0x5f,0x53,0x42,0x5f,0x50,0x43,0x49,0x30,0x50,
    0x52,0x45,0x53,0x43,0x50,0x4c,0x4b,0xff,0xff,0x70,0x1,0x60,0xa2,0x41,0xb,0x93,
    0x60,0x1,0x70,0x0,0x60,0x70,0x0,0x5c,0x2f,0x4,0x5f,0x53,0x42,0x5f,0x50,0x43,
    0x49,0x30,0x50,0x52,0x45,0x53,0x43,0x43,0x4d,0x44,0xa0,0x47,0x4,0x93,0x5c,0x2f,
    0x4,0x5f,0x53,0x42,0x5f,0x50,0x43,0x49,0x30,0x50,0x52,0x45,0x53,0x43,0x49,0x4e,
    0x53,0x1,0x43,0x54,0x46,0x59,0x5c,0x2f,0x4,0x5f,0x53,0x42,0x5f,0x50,0x43,0x49,
    0x30,0x50,0x52,0x45,0x53,0x43,0x44,0x41,0x54,0x1,0x70,0x1,0x5c,0x2f,0x4,0x5f,
    0x53,0x42,0x5f,0x50,0x43,0x49,0x30,0x50,0x52,0x45,0x53,0x43,0x49,0x4e,0x53,0x70,
    0x1,0x60,0xa1,0x4b,0x4,0xa0,0x48,0x4,0x93,0x5c,0x2f,0x4,0x5f,0x53,0x42,0x5f,
    0x50,0x43,0x49,0x30,0x50,0x52,0x45,0x53,0x43,0x52,0x4d,0x56,0x1,0x43,0x54,0x46,
    0x59,0x5c,0x2f,0x4,0x5f,0x53,0x42,0x5f,0x50,0x43,0x49,0x30,0x50,0x52,0x45,0x53,
    0x43,0x44,0x41,0x54,0xa,0x3,0x70,0x1,0x5c,0x2f,0x4,0x5f,0x53,0x42,0x5f,0x50,
    0x43,0x49,0x30,0x50,0x52,0x45,0x53,0x43,0x52,0x4d,0x56,0x70,0x1,0x60,0x5b,0x27,
    0x5c,0x2f,0x4,0x5f,0x53,0x42,0x5f,0x50,0x43,0x49,0x30,0x50,0x52,0x45,0x53,0x43,
    0x50,0x4c,0x4b,0x14,0x4d,0x9,0x43,0x4f,0x53,0x54,0xc,0x5b,0x23,0x5c,0x2f,0x4,
    0x5f,0x53,0x42,0x5f,0x50,0x43,0x49,0x30,0x50,0x52,0x45,0x53,0x43,0x50,0x4c,0x4b,
    0xff,0xff,0x70,0x68,0x5c,0x2f,0x4,0x5f,0x53,0x42,0x5f,0x50,0x43,0x49,0x30,0x50,
    0x52,0x45,0x53,0x43,0x53,0x45,0x4c,0x70,0x1,0x5c,0x2f,0x4,0x5f,0x53,0x42,0x5f,
    0x50,0x43,0x49,0x30,0x50,0x52,0x45,0x53,0x43,0x43,0x4d,0x44,0x70,0x69,0x5c,0x2f,
    0x4,0x5f,0x53,0x42,0x5f,0x50,0x43,0x49,0x30,0x50,0x52,0x45,0x53,0x43,0x44,0x41,
    0x54,0x70,0xa,0x2,0x5c,0x2f,0x4,0x5f,0x53,0x42,0x5f,0x50,0x43,0x49,0x30,0x50,
    0x52,0x45,0x53,0x43,0x43,0x4d,0x44,0x70,0x6a,0x5c,0x2f,0x4,0x5f,0x53,0x42,0x5f,
    0x50,0x43,0x49,0x30,0x50,0x52,0x45,0x53,0x43,0x44,0x41,0x54,0x5b,0x27,0x5c,0x2f,
    0x4,0x5f,0x53,0x42,0x5f,0x50,0x43,0x49,0x30,0x50,0x52,0x45,0x53,0x43,0x50,0x4c,
    0x4b,0x5b,0x83,0x45,0x4,0x43,0x30,0x30,0x30,0x0,0x0,0x0,0x0,0x0,0x0,0x14,
    0xc,0x5f,0x53,0x54,0x41,0x8,0xa4,0x43,0x53,0x54,0x41,0x0,0x8,0x5f,0x4d,0x41,
    0x54,0x11,0xb,0xa,0x8,0x0,0x8,0x0,0x0,0x1,0x0,0x0,0x0,0x14,0xb,0x5f,
    0x45,0x4a,0x30,0x1,0x43,0x45,0x4a,0x30,0x0,0x14,0xe,0x5f,0x4f,0x53,0x54,0xb,
    0x43,0x4f,0x53,0x54,0x0,0x68,0x69,0x6a,0x14,0x1b,0x5c,0x2e,0x5f,0x47,0x50,0x45,
    0x5f,0x45,0x30,0x32,0x0,0x5c,0x2f,0x3,0x5f,0x53,0x42,0x5f,0x43,0x50,0x55,0x53,
    0x43,0x53,0x43,0x4e,0x10,0x4f,0x4,0x5f,0x47,0x50,0x45,0x8,0x5f,0x48,0x49,0x44,
    0xd,0x41,0x43,0x50,0x49,0x30,0x30,0x30,0x36,0x0,0x14,0x39,0x5f,0x45,0x30,0x31,
    0x0,0x5b,0x23,0x5c,0x2f,0x3,0x5f,0x53,0x42,0x5f,0x50,0x43,0x49,0x30,0x42,0x4c,
    0x43,0x4b,0xff,0xff,0x5c,0x2f,0x3,0x5f,0x53,0x42,0x5f,0x50,0x43,0x49,0x30,0x50,
    0x43,0x4e,0x54,0x5b,0x27,0x5c,0x2f,0x3,0x5f,0x53,0x42,0x5f,0x50,0x43,0x49,0x30,
    0x42,0x4c,0x43,0x4b,0x10,0x43,0x14,0x5c,0x2e,0x5f,0x53,0x42,0x5f,0x50,0x43,0x49,
    0x30,0x8,0x5f,0x43,0x52,0x53,0x11,0x40,0xa,0xa,0x9c,0x88,0xd,0x0,0x2,0xc,
    0x0,0x0,0x0,0x0,0x0,0xff,0x0,0x0,0x0,0x0,0x1,0x47,0x1,0xf8,0xc,0xf8,
    0xc,0x1,0x8,0x88,0xd,0x0,0x1,0xc,0x3,0x0,0x0,0x0,0x0,0xf7,0xc,0x0,
    0x0,0xf8,0xc,0x88,0xd,0x0,0x1,0xc,0x3,0x0,0x0,0x0,0xd,0xff,0xff,0x0,
    0x0,0x0,0xf3,0x87,0x17,0x0,0x0,0xc,0x3,0x0,0x0,0x0,0x0,0x0,0x0,0xa,
    0x0,0xff,0xff,0xb,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x87,0x17,0x0,
    0x0,0xc,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20,0xff,0xff,0xbf,0xfe,0x0,
    0x0,0x0,0x0,0x0,0x0,0xc0,0xde,0x8a,0x2b,0x0,0x0,0xc,0x3,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0xff,0xff,0xff,
    0x7f,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x80,0x0,0x0,0x0,0x0,0x79,0x0,0x5b,0x82,0x43,0x4,0x47,0x50,0x45,0x30,0x8,
    0x5f,0x48,0x49,0x44,0xd,0x50,0x4e,0x50,0x30,0x41,0x30,0x36,0x0,0x8,0x5f,0x55,
    0x49,0x44,0xd,0x47,0x50,0x45,0x30,0x20,0x72,0x65,0x73,0x6f,0x75,0x72,0x63,0x65,
    0x73,0x0,0x8,0x5f,0x53,0x54,0x41,0xa,0xb,0x8,0x5f,0x43,0x52,0x53,0x11,0xd,
    0xa,0xa,0x47,0x1,0xe0,0xaf,0xe0,0xaf,0x1,0x4,0x79,0x0,0x5b,0x82,0x4a,0x4,
    0x50,0x48,0x50,0x52,0x8,0x5f,0x48,0x49,0x44,0xd,0x50,0x4e,0x50,0x30,0x41,0x30,
    0x36,0x0,0x8,0x5f,0x55,0x49,0x44,0xd,0x50,0x43,0x49,0x20,0x48,0x6f,0x74,0x70,
    0x6c,0x75,0x67,0x20,0x72,0x65,0x73,0x6f,0x75,0x72,0x63,0x65,0x73,0x0,0x8,0x5f,
    0x53,0x54,0x41,0xa,0xb,0x8,0x5f,0x43,0x52,0x53,0x11,0xd,0xa,0xa,0x47,0x1,
    0x0,0xae,0x0,0xae,0x1,0x14,0x79,0x0,0x10,0x29,0x5c,0x0,0x8,0x5f,0x53,0x33,
    0x5f,0x12,0x6,0x4,0x1,0x1,0x0,0x0,0x8,0x5f,0x53,0x34,0x5f,0x12,0x8,0x4,
    0xa,0x2,0xa,0x2,0x0,0x0,0x8,0x5f,0x53,0x35,0x5f,0x12,0x6,0x4,0x0,0x0,
    0x0,0x0,0x10,0x3b,0x5c,0x2e,0x5f,0x53,0x42,0x5f,0x50,0x43,0x49,0x30,0x5b,0x82,
    0x2e,0x46,0x57,0x43,0x46,0x8,0x5f,0x48,0x49,0x44,0xd,0x51,0x45,0x4d,0x55,0x30,
    0x30,0x30,0x32,0x0,0x8,0x5f,0x53,0x54,0x41,0xa,0xb,0x8,0x5f,0x43,0x52,0x53,
    0x11,0xd,0xa,0xa,0x47,0x1,0x10,0x5,0x10,0x5,0x1,0xc,0x79,0x0,0x10,0x45,
    0x70,0x5c,0x5f,0x53,0x42,0x5f,0x10,0x4d,0x6f,0x50,0x43,0x49,0x30,0x8,0x42,0x53,
    0x45,0x4c,0x0,0x5b,0x82,0xb,0x53,0x30,0x30,0x5f,0x8,0x5f,0x41,0x44,0x52,0x0,
    0x5b,0x82,0x2a,0x53,0x31,0x30,0x5f,0x8,0x5f,0x41,0x44,0x52,0xc,0x0,0x0,0x2,
    0x0,0x14,0x8,0x5f,0x53,0x31,0x44,0x0,0xa4,0x0,0x14,0x8,0x5f,0x53,0x32,0x44,
    0x0,0xa4,0x0,0x14,0x8,0x5f,0x53,0x33,0x44,0x0,0xa4,0x0,0x5b,0x82,0x29,0x53,
    0x31,0x38,0x5f,0x8,0x5f,0x41,0x44,0x52,0xc,0x0,0x0,0x3,0x0,0x8,0x5f,0x53,
    0x55,0x4e,0xa,0x3,0x14,0x12,0x5f,0x45,0x4a,0x30,0x1,0x50,0x43,0x45,0x4a,0x42,
    0x53,0x45,0x4c,0x5f,0x53,0x55,0x4e,0x5b,0x82,0x29,0x53,0x32,0x30,0x5f,0x8,0x5f,
    0x53,0x55,0x4e,0xa,0x4,0x8,0x5f,0x41,0x44,0x52,0xc,0x0,0x0,0x4,0x0,0x14,
    0x12,0x5f,0x45,0x4a,0x30,0x1,0x50,0x43,0x45,0x4a,0x42,0x53,0x45,0x4c,0x5f,0x53,
    0x55,0x4e,0x5b,0x82,0x29,0x53,0x32,0x38,0x5f,0x8,0x5f,0x53,0x55,0x4e,0xa,0x5,
    0x8,0x5f,0x41,0x44,0x52,0xc,0x0,0x0,0x5,0x0,0x14,0x12,0x5f,0x45,0x4a,0x30,
    0x1,0x50,0x43,0x45,0x4a,0x42,0x53,0x45,0x4c,0x5f,0x53,0x55,0x4e,0x5b,0x82,0x29,
    0x53,0x33,0x30,0x5f,0x8,0x5f,0x53,0x55,0x4e,0xa,0x6,0x8,0x5f,0x41,0x44,0x52,
    0xc,0x0,0x0,0x6,0x0,0x14,0x12,0x5f,0x45,0x4a,0x30,0x1,0x50,0x43,0x45,0x4a,
    0x42,0x53,0x45,0x4c,0x5f,0x53,0x55,0x4e,0x5b,0x82,0x29,0x53,0x33,0x38,0x5f,0x8,
    0x5f,0x53,0x55,0x4e,0xa,0x7,0x8,0x5f,0x41,0x44,0x52,0xc,0x0,0x0,0x7,0x0,
    0x14,0x12,0x5f,0x45,0x4a,0x30,0x1,0x50,0x43,0x45,0x4a,0x42,0x53,0x45,0x4c,0x5f,
    0x53,0x55,0x4e,0x5b,0x82,0x29,0x53,0x34,0x30,0x5f,0x8,0x5f,0x53,0x55,0x4e,0xa,
    0x8,0x8,0x5f,0x41,0x44,0x52,0xc,0x0,0x0,0x8,0x0,0x14,0x12,0x5f,0x45,0x4a,
    0x30,0x1,0x50,0x43,0x45,0x4a,0x42,0x53,0x45,0x4c,0x5f,0x53,0x55,0x4e,0x5b,0x82,
    0x29,0x53,0x34,0x38,0x5f,0x8,0x5f,0x53,0x55,0x4e,0xa,0x9,0x8,0x5f,0x41,0x44,
    0x52,0xc,0x0,0x0,0x9,0x0,0x14,0x12,0x5f,0x45,0x4a,0x30,0x1,0x50,0x43,0x45,
    0x4a,0x42,0x53,0x45,0x4c,0x5f,0x53,0x55,0x4e,0x5b,0x82,0x29,0x53,0x35,0x30,0x5f,
    0x8,0x5f,0x53,0x55,0x4e,0xa,0xa,0x8,0x5f,0x41,0x44,0x52,0xc,0x0,0x0,0xa,
    0x0,0x14,0x12,0x5f,0x45,0x4a,0x30,0x1,0x50,0x43,0x45,0x4a,0x42,0x53,0x45,0x4c,
    0x5f,0x53,0x55,0x4e,0x5b,0x82,0x29,0x53,0x35,0x38,0x5f,0x8,0x5f,0x53,0x55,0x4e,
    0xa,0xb,0x8,0x5f,0x41,0x44,0x52,0xc,0x0,0x0,0xb,0x0,0x14,0x12,0x5f,0x45,
    0x4a,0x30,0x1,0x50,0x43,0x45,0x4a,0x42,0x53,0x45,0x4c,0x5f,0x53,0x55,0x4e,0x5b,
    0x82,0x29,0x53,0x36,0x30,0x5f,0x8,0x5f,0x53,0x55,0x4e,0xa,0xc,0x8,0x5f,0x41,
    0x44,0x52,0xc,0x0,0x0,0xc,0x0,0x14,0x12,0x5f,0x45,0x4a,0x30,0x1,0x50,0x43,
    0x45,0x4a,0x42,0x53,0x45,0x4c,0x5f,0x53,0x55,0x4e,0x5b,0x82,0x29,0x53,0x36,0x38,
    0x5f,0x8,0x5f,0x53,0x55,0x4e,0xa,0xd,0x8,0x5f,0x41,0x44,0x52,0xc,0x0,0x0,
    0xd,0x0,0x14,0x12,0x5f,0x45,0x4a,0x30,0x1,0x50,0x43,0x45,0x4a,0x42,0x53,0x45,
    0x4c,0x5f,0x53,0x55,0x4e,0x5b,0x82,0x29,0x53,0x37,0x30,0x5f,0x8,0x5f,0x53,0x55,
    0x4e,0xa,0xe,0x8,0x5f,0x41,0x44,0x52,0xc,0x0,0x0,0xe,0x0,0x14,0x12,0x5f,
    0x45,0x4a,0x30,0x1,0x50,0x43,0x45,0x4a,0x42,0x53,0x45,0x4c,0x5f,0x53,0x55,0x4e,
    0x5b,0x82,0x29,0x53,0x37,0x38,0x5f,0x8,0x5f,0x53,0x55,0x4e,0xa,0xf,0x8,0x5f,
    0x41,0x44,0x52,0xc,0x0,0x0,0xf,0x0,0x14,0x12,0x5f,0x45,0x4a,0x30,0x1,0x50,
    0x43,0x45,0x4a,0x42,0x53,0x45,0x4c,0x5f,0x53,0x55,0x4e,0x5b,0x82,0x29,0x53,0x38,
    0x30,0x5f,0x8,0x5f,0x53,0x55,0x4e,0xa,0x10,0x8,0x5f,0x41,0x44,0x52,0xc,0x0,
    0x0,0x10,0x0,0x14,0x12,0x5f,0x45,0x4a,0x30,0x1,0x50,0x43,0x45,0x4a,0x42,0x53,
    0x45,0x4c,0x5f,0x53,0x55,0x4e,0x5b,0x82,0x29,0x53,0x38,0x38,0x5f,0x8,0x5f,0x53,
    0x55,0x4e,0xa,0x11,0x8,0x5f,0x41,0x44,0x52,0xc,0x0,0x0,0x11,0x0,0x14,0x12,
    0x5f,0x45,0x4a,0x30,0x1,0x50,0x43,0x45,0x4a,0x42,0x53,0x45,0x4c,0x5f,0x53,0x55,
    0x4e,0x5b,0x82,0x29,0x53,0x39,0x30,0x5f,0x8,0x5f,0x53,0x55,0x4e,0xa,0x12,0x8,
    0x5f,0x41,0x44,0x52,0xc,0x0,0x0,0x12,0x0,0x14,0x12,0x5f,0x45,0x4a,0x30,0x1,
    0x50,0x43,0x45,0x4a,0x42,0x53,0x45,0x4c,0x5f,0x53,0x55,0x4e,0x5b,0x82,0x29,0x53,
    0x39,0x38,0x5f,0x8,0x5f,0x53,0x55,0x4e,0xa,0x13,0x8,0x5f,0x41,0x44,0x52,0xc,
    0x0,0x0,0x13,0x0,0x14,0x12,0x5f,0x45,0x4a,0x30,0x1,0x50,0x43,0x45,0x4a,0x42,
    0x53,0x45,0x4c,0x5f,0x53,0x55,0x4e,0x5b,0x82,0x29,0x53,0x41,0x30,0x5f,0x8,0x5f,
    0x53,0x55,0x4e,0xa,0x14,0x8,0x5f,0x41,0x44,0x52,0xc,0x0,0x0,0x14,0x0,0x14,
    0x12,0x5f,0x45,0x4a,0x30,0x1,0x50,0x43,0x45,0x4a,0x42,0x53,0x45,0x4c,0x5f,0x53,
    0x55,0x4e,0x5b,0x82,0x29,0x53,0x41,0x38,0x5f,0x8,0x5f,0x53,0x55,0x4e,0xa,0x15,
    0x8,0x5f,0x41,0x44,0x52,0xc,0x0,0x0,0x15,0x0,0x14,0x12,0x5f,0x45,0x4a,0x30,
    0x1,0x50,0x43,0x45,0x4a,0x42,0x53,0x45,0x4c,0x5f,0x53,0x55,0x4e,0x5b,0x82,0x29,
    0x53,0x42,0x30,0x5f,0x8,0x5f,0x53,0x55,0x4e,0xa,0x16,0x8,0x5f,0x41,0x44,0x52,
    0xc,0x0,0x0,0x16,0x0,0x14,0x12,0x5f,0x45,0x4a,0x30,0x1,0x50,0x43,0x45,0x4a,
    0x42,0x53,0x45,0x4c,0x5f,0x53,0x55,0x4e,0x5b,0x82,0x29,0x53,0x42,0x38,0x5f,0x8,
    0x5f,0x53,0x55,0x4e,0xa,0x17,0x8,0x5f,0x41,0x44,0x52,0xc,0x0,0x0,0x17,0x0,
    0x14,0x12,0x5f,0x45,0x4a,0x30,0x1,0x50,0x43,0x45,0x4a,0x42,0x53,0x45,0x4c,0x5f,
    0x53,0x55,0x4e,0x5b,0x82,0x29,0x53,0x43,0x30,0x5f,0x8,0x5f,0x53,0x55,0x4e,0xa,
    0x18,0x8,0x5f,0x41,0x44,0x52,0xc,0x0,0x0,0x18,0x0,0x14,0x12,0x5f,0x45,0x4a,
    0x30,0x1,0x50,0x43,0x45,0x4a,0x42,0x53,0x45,0x4c,0x5f,0x53,0x55,0x4e,0x5b,0x82,
    0x29,0x53,0x43,0x38,0x5f,0x8,0x5f,0x53,0x55,0x4e,0xa,0x19,0x8,0x5f,0x41,0x44,
    0x52,0xc,0x0,0x0,0x19,0x0,0x14,0x12,0x5f,0x45,0x4a,0x30,0x1,0x50,0x43,0x45,
    0x4a,0x42,0x53,0x45,0x4c,0x5f,0x53,0x55,0x4e,0x5b,0x82,0x29,0x53,0x44,0x30,0x5f,
    0x8,0x5f,0x53,0x55,0x4e,0xa,0x1a,0x8,0x5f,0x41,0x44,0x52,0xc,0x0,0x0,0x1a,
    0x0,0x14,0x12,0x5f,0x45,0x4a,0x30,0x1,0x50,0x43,0x45,0x4a,0x42,0x53,0x45,0x4c,
    0x5f,0x53,0x55,0x4e,0x5b,0x82,0x29,0x53,0x44,0x38,0x5f,0x8,0x5f,0x53,0x55,0x4e,
    0xa,0x1b,0x8,0x5f,0x41,0x44,0x52,0xc,0x0,0x0,0x1b,0x0,0x14,0x12,0x5f,0x45,
    0x4a,0x30,0x1,0x50,0x43,0x45,0x4a,0x42,0x53,0x45,0x4c,0x5f,0x53,0x55,0x4e,0x5b,
    0x82,0x29,0x53,0x45,0x30,0x5f,0x8,0x5f,0x53,0x55,0x4e,0xa,0x1c,0x8,0x5f,0x41,
    0x44,0x52,0xc,0x0,0x0,0x1c,0x0,0x14,0x12,0x5f,0x45,0x4a,0x30,0x1,0x50,0x43,
    0x45,0x4a,0x42,0x53,0x45,0x4c,0x5f,0x53,0x55,0x4e,0x5b,0x82,0x29,0x53,0x45,0x38,
    0x5f,0x8,0x5f,0x53,0x55,0x4e,0xa,0x1d,0x8,0x5f,0x41,0x44,0x52,0xc,0x0,0x0,
    0x1d,0x0,0x14,0x12,0x5f,0x45,0x4a,0x30,0x1,0x50,0x43,0x45,0x4a,0x42,0x53,0x45,
    0x4c,0x5f,0x53,0x55,0x4e,0x5b,0x82,0x29,0x53,0x46,0x30,0x5f,0x8,0x5f,0x53,0x55,
    0x4e,0xa,0x1e,0x8,0x5f,0x41,0x44,0x52,0xc,0x0,0x0,0x1e,0x0,0x14,0x12,0x5f,
    0x45,0x4a,0x30,0x1,0x50,0x43,0x45,0x4a,0x42,0x53,0x45,0x4c,0x5f,0x53,0x55,0x4e,
    0x5b,0x82,0x29,0x53,0x46,0x38,0x5f,0x8,0x5f,0x53,0x55,0x4e,0xa,0x1f,0x8,0x5f,
    0x41,0x44,0x52,0xc,0x0,0x0,0x1f,0x0,0x14,0x12,0x5f,0x45,0x4a,0x30,0x1,0x50,
    0x43,0x45,0x4a,0x42,0x53,0x45,0x4c,0x5f,0x53,0x55,0x4e,0x14,0x48,0x1b,0x44,0x56,
    0x4e,0x54,0x2,0xa0,0xc,0x7b,0x68,0xa,0x8,0x0,0x86,0x53,0x31,0x38,0x5f,0x69,
    0xa0,0xc,0x7b,0x68,0xa,0x10,0x0,0x86,0x53,0x32,0x30,0x5f,0x69,0xa0,0xc,0x7b,
    0x68,0xa,0x20,0x0,0x86,0x53,0x32,0x38,0x5f,0x69,0xa0,0xc,0x7b,0x68,0xa,0x40,
    0x0,0x86,0x53,0x33,0x30,0x5f,0x69,0xa0,0xc,0x7b,0x68,0xa,0x80,0x0,0x86,0x53,
    0x33,0x38,0x5f,0x69,0xa0,0xd,0x7b,0x68,0xb,0x0,0x1,0x0,0x86,0x53,0x34,0x30,
    0x5f,0x69,0xa0,0xd,0x7b,0x68,0xb,0x0,0x2,0x0,0x86,0x53,0x34,0x38,0x5f,0x69,
    0xa0,0xd,0x7b,0x68,0xb,0x0,0x4,0x0,0x86,0x53,0x35,0x30,0x5f,0x69,0xa0,0xd,
    0x7b,0x68,0xb,0x0,0x8,0x0,0x86,0x53,0x35,0x38,0x5f,0x69,0xa0,0xd,0x7b,0x68,
    0xb,0x0,0x10,0x0,0x86,0x53,0x36,0x30,0x5f,0x69,0xa0,0xd,0x7b,0x68,0xb,0x0,
    0x20,0x0,0x86,0x53,0x36,0x38,0x5f,0x69,0xa0,0xd,0x7b,0x68,0xb,0x0,0x40,0x0,
    0x86,0x53,0x37,0x30,0x5f,0x69,0xa0,0xd,0x7b,0x68,0xb,0x0,0x80,0x0,0x86,0x53,
    0x37,0x38,0x5f,0x69,0xa0,0xf,0x7b,0x68,0xc,0x0,0x0,0x1,0x0,0x0,0x86,0x53,
    0x38,0x30,0x5f,0x69,0xa0,0xf,0x7b,0x68,0xc,0x0,0x0,0x2,0x0,0x0,0x86,0x53,
    0x38,0x38,0x5f,0x69,0xa0,0xf,0x7b,0x68,0xc,0x0,0x0,0x4,0x0,0x0,0x86,0x53,
    0x39,0x30,0x5f,0x69,0xa0,0xf,0x7b,0x68,0xc,0x0,0x0,0x8,0x0,0x0,0x86,0x53,
    0x39,0x38,0x5f,0x69,0xa0,0xf,0x7b,0x68,0xc,0x0,0x0,0x10,0x0,0x0,0x86,0x53,
    0x41,0x30,0x5f,0x69,0xa0,0xf,0x7b,0x68,0xc,0x0,0x0,0x20,0x0,0x0,0x86,0x53,
    0x41,0x38,0x5f,0x69,0xa0,0xf,0x7b,0x68,0xc,0x0,0x0,0x40,0x0,0x0,0x86,0x53,
    0x42,0x30,0x5f,0x69,0xa0,0xf,0x7b,0x68,0xc,0x0,0x0,0x80,0x0,0x0,0x86,0x53,
    0x42,0x38,0x5f,0x69,0xa0,0xf,0x7b,0x68,0xc,0x0,0x0,0x0,0x1,0x0,0x86,0x53,
    0x43,0x30,0x5f,0x69,0xa0,0xf,0x7b,0x68,0xc,0x0,0x0,0x0,0x2,0x0,0x86,0x53,
    0x43,0x38,0x5f,0x69,0xa0,0xf,0x7b,0x68,0xc,0x0,0x0,0x0,0x4,0x0,0x86,0x53,
    0x44,0x30,0x5f,0x69,0xa0,0xf,0x7b,0x68,0xc,0x0,0x0,0x0,0x8,0x0,0x86,0x53,
    0x44,0x38,0x5f,0x69,0xa0,0xf,0x7b,0x68,0xc,0x0,0x0,0x0,0x10,0x0,0x86,0x53,
    0x45,0x30,0x5f,0x69,0xa0,0xf,0x7b,0x68,0xc,0x0,0x0,0x0,0x20,0x0,0x86,0x53,
    0x45,0x38,0x5f,0x69,0xa0,0xf,0x7b,0x68,0xc,0x0,0x0,0x0,0x40,0x0,0x86,0x53,
    0x46,0x30,0x5f,0x69,0xa0,0xf,0x7b,0x68,0xc,0x0,0x0,0x0,0x80,0x0,0x86,0x53,
    0x46,0x38,0x5f,0x69,0x14,0x1f,0x50,0x43,0x4e,0x54,0x0,0x70,0x0,0x42,0x4e,0x55,
    0x4d,0x44,0x56,0x4e,0x54,0x50,0x43,0x49,0x55,0x1,0x44,0x56,0x4e,0x54,0x50,0x43,
    0x49,0x44,0xa,0x3,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x41,0x14,0x0,0x0,
    0x0,0x0,0x0,0x0,0xa1,0x0,0x0,0x0
};

static void Decompile_Qemu()
{
    printf("----Decompiler tests : QEMU ----\n");
    AMLDecompiler decomp;
    
    
    assert( AMLDecompilerInit(&decomp));
    
    
    assert(AMLDecompilerStart(&decomp, table, sizeof(table)) == AMLParserError_None);
}



/*
 DefinitionBlock ("test.aml", "DSDT", 10, "IDOEM", "SOMEAID", 0x00001234)
 {
 Device (PCI0)
 {
 Name (_HID, EisaId ("PNP0A03"))
 }
 Device (PCI1)
 {
 Name (_HID, EisaId ("PNP0A01"))
 Name (_ADR, 0x00)
 }
 }
 */
static uint8_t test1[] =
{
    0x44,0x53,0x44,0x54,0x4c,0x00,0x00,0x00,
    0x0a,0x32,0x49,0x44,0x4f,0x45,0x4d,0x00,
    0x53,0x4f,0x4d,0x45,0x41,0x49,0x44,0x00,
    0x34,0x12,0x00,0x00,0x49,0x4e,0x54,0x4c,
    0x13,0x12,0x18,0x20,0x5b,0x82,0x0f,0x50,
    0x43,0x49,0x30,0x08,0x5f,0x48,0x49,0x44,
    0x0c,0x41,0xd0,0x0a,0x03,0x5b,0x82,0x15,
    0x50,0x43,0x49,0x31,0x08,0x5f,0x48,0x49,
    0x44,0x0c,0x41,0xd0,0x0a,0x01,0x08,0x5f,
    0x41,0x44,0x52,0x00
};


/*
 DefinitionBlock ("test.aml", "DSDT", 40, "OEMID", "PROUT", 0xABCF1234)
 {
     Scope (_SB)
     {
     }
 
     Scope (_SB)
     {
         Device (PCI0)
         {
            Name (_HID, EisaId ("PNP0A03"))
         }
 
         Device (PCI1)
         {
            Name (_HID, EisaId ("PNP0A01"))
            Name (_ADR, 0x4536ABEF)
         }
         Device(PCI2)
         {
            Name (_HID, EisaId ("PNP0A01"))
            Name (_ADR, 0x4536ABEF)
            Name(_CSR , 0)
            Scope (_SB)
            {
 
            }
 
            Scope (_SB)
            {
 
            }
            Scope (_SB)
            {
 
            }
 
            Scope (_SB)
            {
 
            }
        }
    }
 }
 */
static uint8_t test4[] =
{
    0x44,0x53,0x44,0x54,0x96,0x00,0x00,0x00,
    0x28,0xa3,0x4f,0x45,0x4d,0x49,0x44,0x00,
    0x50,0x52,0x4f,0x55,0x54,0x00,0x00,0x00,
    0x34,0x12,0xcf,0xab,0x49,0x4e,0x54,0x4c,
    0x13,0x12,0x18,0x20,0x10,0x05,0x5f,0x53,
    0x42,0x5f,0x10,0x4b,0x06,0x5f,0x53,0x42,
    0x5f,0x5b,0x82,0x0f,0x50,0x43,0x49,0x30,
    0x08,0x5f,0x48,0x49,0x44,0x0c,0x41,0xd0,
    0x0a,0x03,0x5b,0x82,0x19,0x50,0x43,0x49,
    0x31,0x08,0x5f,0x48,0x49,0x44,0x0c,0x41,
    0xd0,0x0a,0x01,0x08,0x5f,0x41,0x44,0x52,
    0x0c,0xef,0xab,0x36,0x45,0x5b,0x82,0x37,
    0x50,0x43,0x49,0x32,0x08,0x5f,0x48,0x49,
    0x44,0x0c,0x41,0xd0,0x0a,0x01,0x08,0x5f,
    0x41,0x44,0x52,0x0c,0xef,0xab,0x36,0x45,
    0x08,0x5f,0x43,0x53,0x52,0x00,0x10,0x05,
    0x5f,0x53,0x42,0x5f,0x10,0x05,0x5f,0x53,
    0x42,0x5f,0x10,0x05,0x5f,0x53,0x42,0x5f,
    0x10,0x05,0x5f,0x53,0x42,0x5f
};


static int onName(AMLDecompiler* decompiler ,const ParserContext* context, const char* name)
{
    return 0;
}

static void Decompile_Test1()
{
    printf("----Decompiler tests : Test1 ----\n");
    AMLDecompiler decomp;
    
    
    assert( AMLDecompilerInit(&decomp));
    
    decomp.callbacks.StartName = onName;
    
    AMLParserError ret = AMLDecompilerStart(&decomp, test1, sizeof(test1));
    ptrdiff_t d = decomp.errorPos - test1;
    assert(ret == AMLParserError_None);
    
    
    
}

static void Decompile_Test4()
{
    printf("----Decompiler tests : Test4 ----\n");
    AMLDecompiler decomp;
    
    
    assert( AMLDecompilerInit(&decomp));
    decomp.callbacks.StartName = onName;
    
    assert(AMLDecompilerStart(&decomp, test4, sizeof(test4)) == AMLParserError_None);
}


void Decompile_Tests()
{
    
    
    //Decompile_Qemu();
    Decompile_Test1();
    Decompile_Test4();
}
