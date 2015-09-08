//
//  DebugPrint.h
//  gai
//
//  Created by Steve Hosking on 25/04/13.
//
//

#ifndef __gai__DebugPrint__
#define __gai__DebugPrint__

#include <iostream>

namespace GAI
{
    extern bool sbDebugPrint;
}

#define DEBUG_PRINT( x ) if( GAI::sbDebugPrint ) std::cout << x;

#endif /* defined(__gai__DebugPrint__) */
