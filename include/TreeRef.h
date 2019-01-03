//
//  TreeRef.h
//  KernelTaskV1
//
//  Created by Manuel Deneu on 30/12/2018.
//  Copyright © 2018 Manuel Deneu. All rights reserved.
//

#ifndef TreeRef_h
#define TreeRef_h

#include <stdint.h>

typedef struct _TreeElement TreeElement;

struct _TreeElement
{
    TreeElement *parent;
    int    type;
    const void   *ptr;
    size_t size;
};

void TreeElementInit( TreeElement* element);


#endif /* TreeRef_h */
