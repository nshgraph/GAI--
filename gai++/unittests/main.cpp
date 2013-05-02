//
//  main.cpp
//  unittests
//
//  Created by Nathan Holmberg on 10/03/13.
//  Copyright (c) 2013 nshgraph. All rights reserved.
//

#include <gtest/gtest.h>

#ifdef WIN32
#include <winsock2.h>
#endif

int main(int argc, char** argv)
{
    
#ifdef WIN32
	WSADATA WSAData;
	WSAStartup(0x101, &WSAData);
#endif
    ::testing::InitGoogleTest(&argc, argv);
    int result =  RUN_ALL_TESTS();
#ifdef WIN32
	WSACleanup();
#endif
	return result;
}

