#include <iostream>
#include "ContainmentAlgorithmBuilder.hpp"


void ReadTest(std::istream& in, ContainmentAlgorithmBuilder& builder)
{
	size_t polygonVertices = 0;
	in >> polygonVertices;
	for (size_t i = 0; i < polygonVertices; ++i)
	{
		Point p;
		std::cin >> p;
		builder.AddPolygonVertex(p);
	}

	size_t querries = 0;
	in >> querries;
	for (size_t i = 0; i < querries; ++i)
	{
		Point p;
		std::cin >> p;
		builder.AddQuerryPoint(p);
	}
}

void WriteTest(std::ostream& out, const std::vector<QuerryResultType>& result)
{
	for (auto type : result)
	{
		if (type == QuerryResultType::Inside)
			out << "INSIDE" << std::endl;
		if (type == QuerryResultType::Outside)
			out << "OUTSIDE" << std::endl;
		if (type == QuerryResultType::Border)
			out << "BORDER" << std::endl;
	}
}

int main()
{
	size_t testCount = 0;
	std::cin >> testCount;

	for (size_t t = 0; t < testCount; ++t)
	{
		ContainmentAlgorithmBuilder builder;
		ReadTest(std::cin, builder);
		WriteTest(std::cout, builder.Build().Calculate());
		std::cout << std::endl;
	}
}