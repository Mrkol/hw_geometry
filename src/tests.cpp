#define CATCH_CONFIG_MAIN
#include <catch/catch.hpp>
#define private public
#include "ContainmentAlgorithm.hpp"
#include "ContainmentAlgorithmBuilder.hpp"


TEST_CASE("Comparator test", "[comparator]")
{
	ContainmentAlgorithm::SegmentSetElementComparator cmp;

	REQUIRE(cmp({{1, 1}, {5, 2}}, {{2, 2}, {4, 3}}));
	REQUIRE(!cmp({{5, 2}, {1, 1}}, {{2, 1}, {5, 0}}));
	REQUIRE(cmp({{16, 7}, {13, 4}}, {{9, 10}, {16, 7}}));
	REQUIRE(!cmp({{9, 10}, {16, 7}}, {{16, 7}, {13, 4}}));
	REQUIRE(cmp({{-1, -6}, {-3, -5}}, {{-3, -3}, {-5, -1}}));
	REQUIRE(cmp({{16, 7}, {9, 4}}, {{9, 10}, {16, 7}}));
	REQUIRE(!cmp({{9, 10}, {16, 7}}, {{16, 7}, {9, 4}}));
	REQUIRE(cmp({{-4, 5}, {4, -1}}, {{0, 3}, {0, 3}}));
	REQUIRE(!cmp({{-4, 5}, {4, -1}}, {{0, -3}, {0, -3}}));

	REQUIRE(cmp({{4, 0}, {0, 0}}, {{0, 4}, {4, 4}}));
	REQUIRE(cmp({{4, 0}, {0, 0}}, {{2, 2}, {2, 2}}));
	REQUIRE(!cmp({{2, 2}, {2, 2}}, {{4, 0}, {0, 0}}));

	//important
	REQUIRE(!cmp({{4, 0}, {0, 0}}, {{0, -1}, {0, -1}}));
	REQUIRE(cmp({{0, -1}, {0, -1}}, {{4, 0}, {0, 0}}));
	REQUIRE(cmp({{4, 0}, {0, 0}}, {{0, 1}, {0, 1}}));
	REQUIRE(!cmp({{0, 1}, {0, 1}}, {{4, 0}, {0, 0}}));
	
	REQUIRE(!cmp({{4, 0}, {0, 0}}, {{4, -1}, {4, -1}}));
	REQUIRE(cmp({{4, -1}, {4, -1}}, {{4, 0}, {0, 0}}));
	REQUIRE(cmp({{4, 0}, {0, 0}}, {{4, 1}, {4, 1}}));
	REQUIRE(!cmp({{4, 1}, {4, 1}}, {{4, 0}, {0, 0}}));

	REQUIRE(!cmp({{1, 3}, {1, 3}}, {{1, 2}, {-1, 3}}));
	REQUIRE(cmp({{1, 2}, {-1, 3}}, {{1, 3}, {1, 3}}));
	

	REQUIRE(!cmp({{0, 7}, {8, 9}}, {{1, 2}, {-1, 3}}));
	REQUIRE(cmp({{1, 2}, {-1, 3}}, {{0, 7}, {8, 9}}));
	REQUIRE(cmp({{1, 2}, {-1, 3}}, {{-1, 3}, {0, 5}}));
	REQUIRE(!cmp({{0, 7}, {8, 9}}, {{-1, 3}, {0, 5}}));
	REQUIRE(cmp({{-1, 3}, {0, 5}}, {{0, 7}, {8, 9}}));

	REQUIRE(!cmp({{-5, -5}, {-5, -5}}, {{-10, 10}, {0, -20}}));
	REQUIRE(!cmp({{-10, 10}, {0, -20}}, {{-5, -5}, {-5, -5}}));
}

TEST_CASE("Algo test", "[algo]")
{
	ContainmentAlgorithm algorithm;

	SECTION("Algo 1")
	{
		algorithm.polygon_ = {{0, 0}, {0, 4}, {4, 4}, {4, 0}};
		algorithm.querries_ = {{-1, -1}, {0, -1}, {1, 1},
			{3, 5}, {3, 1}, {1, 4}, {4, 0}};
		std::vector<QuerryResultType> result;
		REQUIRE_NOTHROW((result = algorithm.Calculate()));

		std::vector<QuerryResultType> proper = 
			{
				QuerryResultType::Outside,
				QuerryResultType::Outside, 
				QuerryResultType::Inside,
				QuerryResultType::Outside,
				QuerryResultType::Inside,
				QuerryResultType::Border,
				QuerryResultType::Border
			};
		REQUIRE(result == proper);
	}

	SECTION("Algo 2")
	{
		algorithm.polygon_ = 
			{{0, 7}, {8, 9}, {3, 7}, {8, 7}, {3, 5}, {6, 5}, {1, 4},
			{7, 4}, {2, 3}, {5, 3}, {1, 2}, {-1, 3}, {0, 5}};

		algorithm.querries_ = {{1, 6}, {5, 6}, {1, 3}, {5, 8}, {1, 7},
			{4, 8}, {0, 5}, {4, 5}, {6, 8}};
		std::vector<QuerryResultType> result;
		REQUIRE_NOTHROW((result = algorithm.Calculate()));

		std::vector<QuerryResultType> proper = 
			{
				QuerryResultType::Inside,
				QuerryResultType::Inside,
				QuerryResultType::Inside,
				QuerryResultType::Inside,
				QuerryResultType::Inside,
				QuerryResultType::Border,
				QuerryResultType::Border,
				QuerryResultType::Border,
				QuerryResultType::Outside
			};
		REQUIRE(result == proper);
	}
}

TEST_CASE("Builder test", "[builder]")
{
	SECTION("Builder 1")
	{
		std::vector<Point> pol
			{{0, 0}, {0, 3}, {0, 6}, {3, 6},
			{6, 6}, {6, 3}, {6, 0}, {3, 0}};

		std::vector<Point> quer {{1, 6}, {5, 6}, {1, 3}, {5, 8}, {1, 7},
			{4, 8}, {0, 5}, {4, 5}, {6, 8}, {0, 3}, {3, 0}};

		ContainmentAlgorithmBuilder builder;
		std::for_each(pol.begin(), pol.end(),
			std::bind(&ContainmentAlgorithmBuilder::AddPolygonVertex,
				&builder, std::placeholders::_1));
		std::for_each(quer.begin(), quer.end(),
			std::bind(&ContainmentAlgorithmBuilder::AddQuerryPoint,
				&builder, std::placeholders::_1));

		auto algorithm = builder.Build();

		std::vector<QuerryResultType> result;
		REQUIRE_NOTHROW((result = algorithm.Calculate()));

		std::vector<QuerryResultType> proper = 
			{
				QuerryResultType::Border,
				QuerryResultType::Border,
				QuerryResultType::Inside,
				QuerryResultType::Outside,
				QuerryResultType::Outside,
				QuerryResultType::Outside,
				QuerryResultType::Border,
				QuerryResultType::Inside,
				QuerryResultType::Outside,
				QuerryResultType::Border,
				QuerryResultType::Border
			};
		REQUIRE(result == proper);
	}

	SECTION("Builder 2")
	{
		std::vector<Point> pol
			{{0, 9}, {0, 12}, {0, 15}, {0, 18},
			{-2000000, 9}, {0, 0}, {0, 3}, {0, 6}};

		std::vector<Point> quer {{0, 12}, {0, 15}, {0, 18}, {0, 8}, {0, 13}};

		ContainmentAlgorithmBuilder builder;
		std::for_each(pol.begin(), pol.end(),
			std::bind(&ContainmentAlgorithmBuilder::AddPolygonVertex,
				&builder, std::placeholders::_1));
		std::for_each(quer.begin(), quer.end(),
			std::bind(&ContainmentAlgorithmBuilder::AddQuerryPoint,
				&builder, std::placeholders::_1));

		auto algorithm = builder.Build();

		std::vector<QuerryResultType> result;
		REQUIRE_NOTHROW((result = algorithm.Calculate()));

		std::vector<QuerryResultType> proper = 
			{
				QuerryResultType::Border,
				QuerryResultType::Border,
				QuerryResultType::Border,
				QuerryResultType::Border,
				QuerryResultType::Border
			};
		REQUIRE(result == proper);
	}

	SECTION("Builder 3")
	{
		std::vector<Point> pol
			{{10, 10}, {-10, 10}, {0, -20}};

		std::vector<Point> quer {{5, -5}, {-5, -5}, {0, 10}};

		ContainmentAlgorithmBuilder builder;
		std::for_each(pol.begin(), pol.end(),
			std::bind(&ContainmentAlgorithmBuilder::AddPolygonVertex,
				&builder, std::placeholders::_1));
		std::for_each(quer.begin(), quer.end(),
			std::bind(&ContainmentAlgorithmBuilder::AddQuerryPoint,
				&builder, std::placeholders::_1));

		auto algorithm = builder.Build();

		std::vector<QuerryResultType> result;
		REQUIRE_NOTHROW((result = algorithm.Calculate()));

		std::vector<QuerryResultType> proper = 
			{
				QuerryResultType::Border,
				QuerryResultType::Border,
				QuerryResultType::Border
			};
		REQUIRE(result == proper);
	}
}
