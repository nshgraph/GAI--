//
//  test_model.cpp
//  unittests
//
//  Created by Nathan Holmberg on 24/03/13.
//  Copyright (c) 2013 nshgraph. All rights reserved.
//

#include "gtest/gtest.h"

#include "Model.h"


TEST( ModelTest, model_as_kv_store )
{
    std::string test_key1 = "test_key1";
    std::string test_key2 = "test_key2";
    std::string test_value1 = "test_value1";
    std::string test_value2 = "test_value2";
    // Create a model object
    GAI::Model model = GAI::Model();
    // put parameter one into the model
    model.set( test_key1, test_value1);
    // check that it (but not param two are there)
    EXPECT_TRUE( model.hasParameter(test_key1));
    EXPECT_FALSE(model.hasParameter(test_key2));
    // put second param in
    model.set( test_key2, test_value2);
    // ensure that the retrieved values are correct
    EXPECT_EQ( model.get(test_key1),test_value1);
    EXPECT_EQ( model.get(test_key2),test_value2);
    
    //now retrieve all parameters and check they are there
    GAI::Model::ParameterMap parameters = model.getKeysAndValues();
    
    EXPECT_EQ( parameters.size(), 2);
    
    ASSERT_TRUE( parameters.find(test_key1) != parameters.end());
    ASSERT_TRUE( parameters.find(test_key2) != parameters.end());
    
    EXPECT_EQ( parameters[test_key1], test_value1);
    EXPECT_EQ( parameters[test_key2], test_value2);
    
}

TEST( ModelTest, model_as_temp_kv_store )
{
    std::string test_key1 = "test_key1";
    std::string test_key2 = "test_key2";
    std::string test_value1 = "test_value1";
    std::string test_value2 = "test_value2";
    GAI::Model::ParameterMap parameters;
    // Create a model object
    GAI::Model model = GAI::Model();
    // put parameter one into the model
    model.set( test_key1, test_value1);
    // check that it (but not param two are there)
    EXPECT_TRUE( model.hasParameter(test_key1));
    EXPECT_FALSE(model.hasParameter(test_key2));
    // put second param in as a temp value
    model.setForNextHit( test_key2, test_value2);
    // ensure that the retrieved values are correct
    EXPECT_EQ( model.get(test_key1),test_value1);
    EXPECT_EQ( model.get(test_key2),test_value2);
    
    //now retrieve all parameters and check they are there
    parameters = model.getKeysAndValues();
    
    EXPECT_EQ( parameters.size(), 2);
    
    ASSERT_TRUE( parameters.find(test_key1) != parameters.end());
    ASSERT_TRUE( parameters.find(test_key2) != parameters.end());
    
    EXPECT_EQ( parameters[test_key1], test_value1);
    EXPECT_EQ( parameters[test_key2], test_value2);
    
    // now clear the temp values
    model.clearTemporaryValues();
    // check that the temp one has gone
    EXPECT_TRUE( model.hasParameter(test_key1));
    EXPECT_FALSE(model.hasParameter(test_key2));
    
    //now retrieve all parameters and check that two is gone
    parameters = model.getKeysAndValues();
    
    EXPECT_EQ( parameters.size(), 1);
    
    ASSERT_TRUE( parameters.find(test_key1) != parameters.end());
    EXPECT_TRUE( parameters.find(test_key2) == parameters.end());
    
}

TEST( ModelTest, model_setall )
{
    std::string test_key1 = "test_key1";
    std::string test_key2 = "test_key2";
    std::string test_value1 = "test_value1";
    std::string test_value2 = "test_value2";
    GAI::Model::ParameterMap parameters, parameters_set;
    // Create a model object
    GAI::Model model = GAI::Model();
    
    // create the parameter map for insertion
    parameters_set[test_key1] = test_value1;
    parameters_set[test_key2] = test_value2;
    
    // put parameter one into the model
    model.setAll( parameters_set, false );
    
    
    parameters = model.getKeysAndValues();
    
    EXPECT_EQ( parameters, parameters_set);
    
}


TEST( ModelTest, model_setall_temp )
{
    std::string test_key1 = "test_key1";
    std::string test_key2 = "test_key2";
    std::string test_value1 = "test_value1";
    std::string test_value2 = "test_value2";
    GAI::Model::ParameterMap parameters, parameters_set;
    // Create a model object
    GAI::Model model = GAI::Model();
    
    // create the parameter map for insertion
    parameters_set[test_key1] = test_value1;
    parameters_set[test_key2] = test_value2;
    
    // put parameter one into the model
    model.setAll( parameters_set, true );
    
    
    parameters = model.getKeysAndValues();
    
    EXPECT_EQ( parameters, parameters_set);
    
    // now clear temporaries
    model.clearTemporaryValues();
    
    parameters = model.getKeysAndValues();
    
    EXPECT_EQ( parameters.size(), 0 );
    
    
}


