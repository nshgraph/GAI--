
#include "gtest/gtest.h"
#include "utilities.h"

#include "GAIDefines.h"
#include "Hit.h"

TEST( HitTest, create_hit )
{
    const std::string test_version = "2";
    const std::string test_url = "url";
    const double test_timestamp = 100.0;
	
	GAITest::TestHit hit( test_version, test_url, test_timestamp );
	
	EXPECT_TRUE( hit.getGaiVersion() == test_version );
	EXPECT_TRUE( hit.getDispatchURL() == test_url );
	EXPECT_EQ( hit.getTimestamp(), test_timestamp );
}

TEST( HitTest, parameters_add_to_url )
{
	std::map<std::string, std::string> parameters;
	parameters[ "key1" ] = "val1";
	
    GAITest::TestHit hit;
	hit.setParameters( parameters );
	
	EXPECT_FALSE( hit.getDispatchURL() == "" );
}

TEST( HitTest, multiple_parameters_add_to_url )
{
	std::string key;
	std::string value;
	
	std::map<std::string, std::string> parameters;
	parameters[ "key1" ] = "val1";
	parameters[ "key2" ] = "val2";
	parameters[ "key3" ] = "val3";
	
    GAITest::TestHit hit;
	hit.setParameters( parameters );
	std::string url = hit.getDispatchURL();
	
	// version is not passed in with setParameters so test it here
	const int version_pos = url.find("v=");
	std::string version = url.substr( version_pos );
	
	const int equals_pos = version.find( "=" );
	key = version.substr( 0, equals_pos );
	value = version.substr( equals_pos + 1 );
	
	// check version parameter
	EXPECT_TRUE( key == "v" );
	EXPECT_TRUE( value == kGAIVersion );
	
	// remove version from url
	url = url.substr( 0, version_pos - 1 );
	
	// tokenize and test parameters
	std::stringstream url_stream( url );
	std::string parameter;
	
	while( std::getline( url_stream, parameter, '&' ) )
	{
		const int location = parameter.find( "=" );
		key = parameter.substr( 0, location );
		value = parameter.substr( location + 1 );
		
		// check parameter is correct
		EXPECT_TRUE( parameters[ key ] == value );
    }
}

TEST( HitTest, multiple_parameters_add_to_encoded_url )
{
	std::string key;
	std::string value;
	
	std::map<std::string, std::string> parameters;
	parameters[ "key1" ] = "home page";
	parameters[ "key2" ] = "salt&pepper";
	parameters[ "key3" ] = "val@val.com";
	
	std::map<std::string, std::string> encoded_parameters;
	encoded_parameters[ "key1" ] = "home%20page";
	encoded_parameters[ "key2" ] = "salt%26pepper";
	encoded_parameters[ "key3" ] = "val%40val.com";
	
    GAITest::TestHit hit;
	hit.setParameters( parameters );
	std::string url = hit.getDispatchURL();
	
	// version is not passed in with setParameters so test it here
	const int version_pos = url.find("v=");
	std::string version = url.substr( version_pos );
	
	const int equals_pos = version.find( "=" );
	key = version.substr( 0, equals_pos );
	value = version.substr( equals_pos + 1 );
	
	// check version parameter
	EXPECT_TRUE( key == "v" );
	EXPECT_TRUE( value == kGAIVersion );
	
	// remove version from url
	url = url.substr( 0, version_pos - 1 );
	
	// tokenize and test parameters
	std::stringstream url_stream( url );
	std::string parameter;
	
	while( std::getline( url_stream, parameter, '&' ) )
	{
		const int location = parameter.find( "=" );
		key = parameter.substr( 0, location );
		value = parameter.substr( location + 1 );
		
		// check parameter is encoded
		EXPECT_TRUE( encoded_parameters[ key ] == value );
    }
}

TEST( HitTest, timestamp )
{
	const double timestamp = 9999;
	
	GAITest::TestHit hit;
	hit.setTimestamp( timestamp );
	
	EXPECT_EQ( hit.getTimestamp(), timestamp );
}
