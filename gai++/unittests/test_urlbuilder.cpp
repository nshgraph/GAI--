
#include "gtest/gtest.h"

#include "Hit.h"
#include "Timestamp.h"
#include "URLBuilder.h"

namespace
{
	class HitTestClass : public GAI::Hit
	{
	public:
		HitTestClass(const std::string url, const uint64_t timestamp) : Hit(1, "1", url, timestamp) {}
	};

	void reset_cache_buster()
	{
		srand(0);
	}

#ifdef WIN32
	const std::string RAND_1 = "38";
	const std::string RAND_2 = "7719";
	const std::string RAND_3 = "21238";
#else
	const std::string RAND_1 = "520932930";
	const std::string RAND_2 = "28925691";
	const std::string RAND_3 = "822784415";
#endif

}

TEST(URLBuilderTest, post_url)
{
	{
		HitTestClass hit("", 0);
		EXPECT_EQ("/collect", GAI::UrlBuilder::createPOSTURL(hit));
	}
	{
		HitTestClass hit("appview", 0);
		EXPECT_EQ("/collect", GAI::UrlBuilder::createPOSTURL(hit));
	}
	{
		HitTestClass hit("appview", 10);
		EXPECT_EQ("/collect", GAI::UrlBuilder::createPOSTURL(hit));
	}
}

TEST(URLBuilderTest, post_payload_queue_time)
{
	reset_cache_buster();
	HitTestClass hit("", 0);
	EXPECT_EQ("&qt=180&z=" + RAND_1, GAI::UrlBuilder::createPOSTPayload(hit, 180));
}

TEST(URLBuilderTest, post_payload_queue_time_is_constrained_to_four_hours)
{
	{
		reset_cache_buster();
		HitTestClass hit("", 0);
		EXPECT_EQ("&qt=14400000&z=" + RAND_1, GAI::UrlBuilder::createPOSTPayload(hit, 14500000));
	}
	{
		reset_cache_buster();
		HitTestClass hit("", 180);
		EXPECT_EQ("&qt=14400000&z=" + RAND_1, GAI::UrlBuilder::createPOSTPayload(hit, 0));
	}
}

TEST(URLBuilderTest, post_payload_cache_buster)
{
	reset_cache_buster();
	HitTestClass hit("", 0);
	EXPECT_EQ("&qt=0&z=" + RAND_1, GAI::UrlBuilder::createPOSTPayload(hit, 0));
	EXPECT_EQ("&qt=0&z=" + RAND_2, GAI::UrlBuilder::createPOSTPayload(hit, 0));
	EXPECT_EQ("&qt=0&z=" + RAND_3, GAI::UrlBuilder::createPOSTPayload(hit, 0));
}

TEST(URLBuilderTest, post_payload_contains_hit_url)
{
	reset_cache_buster();
	HitTestClass hit("appview", 0);
	EXPECT_EQ("appview&qt=180&z=" + RAND_1, GAI::UrlBuilder::createPOSTPayload(hit, 180));
}
