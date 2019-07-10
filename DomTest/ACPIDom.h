//
//  ACPIDom.h
//  DomTest
//
//  Created by Manuel Deneu on 25/05/2019.
//  Copyright © 2019 Manuel Deneu. All rights reserved.
//

#pragma once
#include <AMLDecompiler.h>

typedef struct
{
    ACPIDevice root;
} ACPIDom;

int ACPIDomParseBuffer( ACPIDom* dom,const uint8_t* buffer , size_t bufferSize);
