//
//  Decompiler.hpp
//  Decompiler
//
//  Created by Manuel Deneu on 09/02/2019.
//  Copyright © 2019 Manuel Deneu. All rights reserved.
//

#pragma once

#include <stdint.h>
#include <inttypes.h>
#include <cstddef>

namespace AML
{


class Decompiler
{
public:
    bool process( const uint8_t* buffer , std::size_t bufferSize);
    
    
    
};
    
} // end namespace AML
