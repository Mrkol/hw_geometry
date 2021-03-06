#pragma once
#ifndef CONTAINMENT_ALGORITHM_HPP
#define CONTAINMENT_ALGORITHM_HPP

#include <vector>
#include "Geometry.hpp"
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>


enum class QueryResultType
{
	Inside,
	Border,
	Outside
};

class ContainmentAlgorithm
{
private:
	enum class EventType
	{
		SegmentStart,
		Query,
		SegmentEnd,
		SegmentDelayedStart
	};

	struct Event
	{
		Scalar x;
		size_t index;
		EventType type;
	};

	struct SegmentSetElement
	{
		Segment segment;
		size_t index;
	};

	class SegmentSetElementComparator
	{
		friend class ContainmentAlgorithm;

	public:
		bool operator()(const Segment& first, const Segment& second) const;
		bool operator()(const SegmentSetElement& first,
			const SegmentSetElement& second) const;
	};

	using SegmentSet = __gnu_pbds::tree<SegmentSetElement,
		__gnu_pbds::null_type, SegmentSetElementComparator,
		__gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update>;

	SegmentSet segmentSet_;

	std::vector<Point> polygon_;
	std::vector<Point> querries_;

	std::vector<Event> sortedEvents_;

	std::vector<QueryResultType> result_;

	ContainmentAlgorithm() = default;

	void generateEvents();

	void handleQuery(size_t index);

	friend class ContainmentAlgorithmBuilder;

public:

	std::vector<QueryResultType> Calculate();
};

#endif // CONTAINMENT_ALGORITHM_HPP
