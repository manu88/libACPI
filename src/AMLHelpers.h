//
//  AMLHelpers.h
//  KernelTaskV1
//
//  Created by Manuel Deneu on 30/12/2018.
//  Copyright © 2018 Manuel Deneu. All rights reserved.
//

#ifndef AMLHelpers_h
#define AMLHelpers_h

#include "ACPIDesc.h"
#include <stdint.h>

int GetDWord(const uint8_t* buffer , ACPIDWord* word);

#endif /* AMLHelpers_h */
