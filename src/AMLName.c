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
//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <AMLTypes.h>
#include <assert.h>
#include <AMLHelpers.h>
#include "AMLByteCode.h"

ssize_t AMLNameCreateFromBuffer( AMLName*name, const uint8_t* buffer , size_t bufferSize )
{
    
    assert(name);
    assert(buffer);
    
    const ssize_t maxSize =  ExtractMaxNameSize(buffer, bufferSize);
    
    if (maxSize <= 0)
    {
        return maxSize;
    }
    
    if (bufferSize< maxSize)
    {
        return -2;
    }
    
    name->originalBuf = buffer;
    name->originalBufSize = bufferSize;
    return maxSize;
}


uint8_t AMLNameHasPrefixRoot( const AMLName*name)
{
    assert(name);
    if (name->originalBuf && name->originalBufSize )
    {
        if (name->originalBufSize >= 2)
        {
            // we must have some space left on the buffer , and the next byte must be a real name, ie not \ or ^
            if(IsRealName( name->originalBuf[1]  ) == 0)
            {
                return 0;
            }
        }
        return name->originalBuf[0] == AML_OP_RootChar;
    }
    
    return 0;
}

uint8_t AMLNameCountParents( const AMLName*name)
{
    assert(name);
    
    if (name->originalBuf && name->originalBufSize )
    {
        uint8_t c = 0;
        while (c<name->originalBufSize &&  name->originalBuf[c] == AML_OP_ParentPrefixChar)
        {
            c++;
        }
        
        // we must have some space left on the buffer , and the next byte must be a real name, ie not \ or ^
        if ( c && name->originalBufSize >= c)
        {
            if( IsRealName( name->originalBuf[c] )
               || name->originalBuf[c] == AML_OP_DualNamePrefix
               || name->originalBuf[c] == AML_OP_MultiNamePrefix
               )
            {
                return c;
            }
        }
        
        return 0;
    }
    return 0;
}


uint8_t AMLNameCountSegments( const AMLName*name)
{
    assert(name);
 
    if (IsRealName(name->originalBuf[0]))
    {
        return 1;
    }
    else if (AMLNameHasPrefixRoot(name))
    {
        if (IsRealName(name->originalBuf[1]))
        {
            return 1;
        }
    }
    const uint8_t numParents =  AMLNameCountParents(name);
    assert(numParents);
    
    if (IsRealName(name->originalBuf[numParents]))
    {
        return 1;
    }
    else if ( name->originalBuf[numParents] == AML_OP_DualNamePrefix)
    {
        return 2;
    }
    else if ( name->originalBuf[numParents] == AML_OP_MultiNamePrefix)
    {
        return name->originalBuf[numParents+1];
    }
    
    return 0;
}


uint8_t AMLNameGetSegment(const AMLName*name, uint8_t index , char toBuffer[5] )
{
    assert( name);
    assert(toBuffer);
    
    size_t offSetName = 0;
    uint8_t numParent = 0;
    if (AMLNameHasPrefixRoot(name))
    {
        offSetName++;
    }
    else if ((numParent = AMLNameCountParents(name)))
    {
        offSetName += numParent;
    }
    
    const uint8_t numSegs =  AMLNameCountSegments(name);
    uint8_t offSeg = 0;
    
    if (numSegs == 2)
    {
        offSeg+=1;
    }
    else if (numSegs >2)
    {
        offSeg +=2;
    }
    
    const size_t offsetBySegs =  index*4;
    
    const uint8_t *bufPos = name->originalBuf + offSetName + offsetBySegs + offSeg;
    
    size_t remainsSize = name->originalBufSize - offSetName;
    assert( remainsSize >=0);
    
    
    
    
    memset(toBuffer, 0, 5);
    
    const size_t sizeToCopy = remainsSize < 4? remainsSize : 4;
    memcpy(toBuffer, bufPos, sizeToCopy);
    
    
    return 1;
}


char* AMLNameConstructNormalized( const AMLName* name)
{
    assert(name);
    
    
    const uint8_t hasRoot = AMLNameHasPrefixRoot(name);
    const uint8_t  numParents = AMLNameCountParents(name);
    
    const uint8_t numSegs = AMLNameCountSegments(name);
    if ( /* == 0 &&*/ numParents == 0 && numSegs == 1)
    {
        
        size_t size = name->originalBufSize<4? name->originalBufSize :4;
        size += hasRoot;
        
        return strndup( (const char*)name->originalBuf, size);
    }
    
    
    
    size_t advance = hasRoot +  numParents;
    
    if (numSegs == 2)
        advance +=1;
    else if (numSegs >2)
        advance +=2;
    
    size_t remains = name->originalBufSize - advance;
    const uint8_t*  bufPos = name->originalBuf + advance;
    
    const size_t size = remains<4*numSegs? remains :4*numSegs;
    
    const size_t sizeToAlloc = numParents + size +  numSegs  +1 ; // +1 for the fucking NULL byte
    char* ret = malloc( sizeToAlloc);
    if (ret == NULL)
    {
        return NULL;
    }
    
    
    memset(ret, 0, sizeToAlloc);
    
    for(uint8_t i=0;i<numParents ;i++)
    {
        ret[i] = '^';
    }
    
    for(uint8_t seg = 0;seg<numSegs ;seg++)
    {
        //uint8_t offsetfromDot = 0;
        //const size_t writeAt = numParents+(seg*4);// + offsetfromDot;
    
        //char* addAtt = ret+writeAt;
        
        if (seg>0)
        {
            //printf("Add . after '%s'\n" , ret);
            strcat(ret, ".");
            //addAtt++;
        }
        
        const uint8_t* copyFrom = bufPos+(seg*4);

        //memcpy(addAtt, copyFrom, 4);
        strncat(ret, (const char*)copyFrom, 4);
        //size-=4;
        
    }
    
    return ret;
}
