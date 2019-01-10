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
#include "ACPI_Tests.h"
#include "AMLParser_Tests.h"
#include "Decompiler_Tests.h"
#include "QemuTest.h"

int main(int argc, const char * argv[])
{
    AMLParserTests();
    //ACPI_Tests();
    Decompile_Tests();
    //QemuDecompTest();
    return 0;
}
