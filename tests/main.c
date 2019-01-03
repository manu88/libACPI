//
//  main.c
//  libACPI
//
//  Created by Manuel Deneu on 03/01/2019.
//  Copyright © 2019 Manuel Deneu. All rights reserved.
//

#include <stdio.h>
#include "ACPI_Tests.h"
#include "AMLParser_Tests.h"

int main(int argc, const char * argv[])
{
    AMLParserTests();
    ACPI_Tests();
    return 0;
}
