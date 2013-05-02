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

static bool sbDebugPrint = true;

#define DEBUG_PRINT( x ) if( sbDebugPrint ) std::cout << x;

#endif /* defined(__gai__DebugPrint__) */
