#include "ContainmentAlgorithmBuilder.hpp"


ContainmentAlgorithmBuilder& ContainmentAlgorithmBuilder::AddPolygonVertex(Point p)
{
	auto& pol = algorithm_.polygon_;
	
	while (pol.size() >= 2 && 
		(pol[pol.size() - 2] - pol[pol.size() - 1]) % (p - pol[pol.size() - 1]) == 0)
	{
		pol.pop_back();
	}

	algorithm_.polygon_.push_back(p);
	return *this;
}

ContainmentAlgorithmBuilder& ContainmentAlgorithmBuilder::AddQuerryPoint(Point p)
{
	algorithm_.querries_.push_back(p);
	return *this;
}

ContainmentAlgorithm ContainmentAlgorithmBuilder::Build()
{
	auto& pol = algorithm_.polygon_;

	size_t i = 0;
	while (pol.size() - i >= 2 && 
		(pol[i + 1] - pol[i]) % (pol.back() - pol[i]) == 0)
	{
		++i;
	}
	pol.erase(pol.begin(), pol.begin() + i);

	while (pol.size() >= 2 && 
		(pol[0] - pol[pol.size() - 1]) % (pol[pol.size() - 2] - pol[pol.size() - 1]) == 0)
	{
		pol.pop_back();
	}

	return std::move(algorithm_);
}
