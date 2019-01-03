//
//  TreeRef.c
//  KernelTaskV1
//
//  Created by Manuel Deneu on 30/12/2018.
//  Copyright © 2018 Manuel Deneu. All rights reserved.
//

#include "TreeRef.h"
#include <string.h>

void TreeElementInit( TreeElement* element)
{
    memset(element, 0, sizeof(TreeElement));
}
