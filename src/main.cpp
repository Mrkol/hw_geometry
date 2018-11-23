#include <iostream>
#include "ContainmentAlgorithmBuilder.hpp"


ContainmentAlgorithm ReadTest(std::istream& in)
{	
	ContainmentAlgorithmBuilder builder;

	size_t pointCount = 0;
	in >> pointCount;
	for (size_t i = 0; i < pointCount; ++i)
	{
		Point p;
		in >> p;
		builder.AddPolygonVertex(p);
	}

	size_t queryCount = 0;
	in >> queryCount;
	for (size_t i = 0; i < queryCount; ++i)
	{
		Point p;
		in >> p;
		builder.AddQueryPoint(p);
	}

	return builder.Build();
}


std::vector<ContainmentAlgorithm> ReadMultitest(std::istream& in)
{
	size_t multitests = 0;
	in >> multitests;
	std::vector<ContainmentAlgorithm> result;
	result.reserve(multitests);

	for (size_t i = 0; i < multitests; ++i)
	{
		result.push_back(ReadTest(in));
	}

	return result;
}

std::vector<std::vector<QueryResultType>>
	SolveMultitest(std::vector<ContainmentAlgorithm>&& data)
{
	std::vector<std::vector<QueryResultType>> result;

	for (auto& algorithm : data)
	{
		result.push_back(algorithm.Calculate());
	}

	return result;
}

void WriteTest(std::ostream& out, const std::vector<QueryResultType>& result)
{
	for (const auto& type : result)
	{
		switch (type)
		{
			case QueryResultType::Inside:
				out << "INSIDE";
				break;
			case QueryResultType::Outside:
				out << "OUTSIDE";
				break;
			case QueryResultType::Border:
				out << "BORDER";
				break;
		}
		out << std::endl;
	}
}

void WriteMultitest(std::ostream& out, const std::vector<std::vector<QueryResultType>>& result)
{
	for (const auto& test : result)
	{
		WriteTest(out, test);
		out << std::endl;
	}
}

int main()
{
	WriteMultitest(std::cout, SolveMultitest(std::move(ReadMultitest(std::cin))));
}