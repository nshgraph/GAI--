//
//  main.cpp
//  unittests
//
//  Created by Nathan Holmberg on 10/03/13.
//  Copyright (c) 2013 nshgraph. All rights reserved.
//

#include "gtest/gtest.h"

#include "DataStoreSqlite.h"

TEST (DataStoreSqlLite, properties)
{
    std::string test_db = "test.db";

    // delete existing test db
    int rc = unlink( test_db.c_str() ); // we don't test the return code becuase we aren't guaranteed the file exists before the test
    // create db
    GAI::DataStoreSqlite db = GAI::DataStoreSqlite(test_db);
    EXPECT_EQ(db.open(),true);
    // should be open
    EXPECT_EQ(db.isOpen(), true);
}
