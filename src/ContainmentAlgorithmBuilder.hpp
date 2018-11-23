#pragma once
#ifndef CONTAINMENT_ALGORITHM_BUILDER_HPP
#define CONTAINMENT_ALGORITHM_BUILDER_HPP

#include <optional>
#include "Geometry.hpp"
#include "ContainmentAlgorithm.hpp"

class ContainmentAlgorithmBuilder
{
private:
	ContainmentAlgorithm algorithm_;

public:
	ContainmentAlgorithmBuilder& AddPolygonVertex(Point p);
	ContainmentAlgorithmBuilder& AddQueryPoint(Point p);
	ContainmentAlgorithm Build();
};


#endif // CONTAINMENT_ALGORITHM_BUILDER_HPP
