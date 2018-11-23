#include "ContainmentAlgorithm.hpp"
#include <functional>
#include <iostream>

void ContainmentAlgorithm::generateEvents()
{
	sortedEvents_.reserve(2 * polygon_.size() + querries_.size());

	for (size_t i = 0; i < polygon_.size(); ++i)
	{
		auto n = polygon_.size();

		auto a = polygon_[i];
		auto b = polygon_[(i + 1) % n];

		auto c = polygon_[(i + 2) % n];
		auto d = polygon_[(i + 3) % n];
		
		auto e = polygon_[(i + 4) % n];
		auto f = polygon_[(i + 5) % n];

		if (c.x == d.x)
			continue;

		if (c.x > d.x)
		{
			std::swap(a, f);
			std::swap(b, e);
			std::swap(c, d);
		}

		sortedEvents_.push_back({c.x, (i + 2) % n,
			b.x < c.x || (b.x == c.x && a.x < b.x)
				? EventType::SegmentDelayedStart : EventType::SegmentStart});
		sortedEvents_.push_back({d.x, (i + 2) % n, EventType::SegmentEnd});
	}

	for (size_t i = 0; i < querries_.size(); ++i)
	{
		sortedEvents_.push_back({querries_[i].x, i, EventType::Query});	
	}

}

void ContainmentAlgorithm::handleQuery(size_t index)
{
	Point querry = querries_[index];

	auto checkEdge =
		[querry, this](size_t index)
		{
			size_t n = polygon_.size();
			for (ssize_t i = -1; i <= 1; ++i)
			{
				if (IsPointOnSegment(
					{polygon_[(n + index + i) % n],
						polygon_[(n + index + i + 1) % n]},
							querry))
					return true;
			}

			return false;
		};

	auto above = segmentSet_.lower_bound({{querry, querry}, index});

	bool matched = false;

	if (above != segmentSet_.end())
		matched = matched || checkEdge(above->index);

	if (above != segmentSet_.begin())
		matched = matched || checkEdge(std::prev(above)->index);

	size_t k = segmentSet_.order_of_key({{querry, querry}, index});

	if (matched)
	{
		result_[index] = QueryResultType::Border;
	}
	else if (k % 2)
	{
		result_[index] = QueryResultType::Inside;
	}
	else
	{
		result_[index] = QueryResultType::Outside;
	}
}


std::vector<QueryResultType> ContainmentAlgorithm::Calculate()
{
	result_.resize(querries_.size());

	generateEvents();

	std::sort(sortedEvents_.begin(), sortedEvents_.end(),
		[](const Event& first, const Event& second)
		{
			return first.x < second.x
				|| (first.x == second.x && first.type < second.type);
		});

	for (size_t scanline = 0; scanline < sortedEvents_.size(); ++scanline)
	{
		auto index = sortedEvents_[scanline].index;
		auto type = sortedEvents_[scanline].type;

		if (type == EventType::Query)
		{
			handleQuery(index);
		}
		else
		{
			Segment seg {polygon_[index], polygon_[(index + 1) % polygon_.size()]};
			
			if (type == EventType::SegmentEnd)
				segmentSet_.erase({seg, index});
			else
				segmentSet_.insert({seg, index});
		}
	}

	segmentSet_.clear();
	sortedEvents_.clear();
	return std::move(result_);
}

bool ContainmentAlgorithm::SegmentSetElementComparator::
	operator()(const SegmentSetElement& first, const SegmentSetElement& second) const
{
	return operator()(first.segment, second.segment);
}

bool ContainmentAlgorithm::SegmentSetElementComparator::
	operator()(const Segment& first, const Segment& second) const
{
	auto[p11, p12] = first;
	auto[p21, p22] = second;
	if (p11.x > p12.x) std::swap(p11, p12);
	if (p21.x > p22.x) std::swap(p21, p22);

	Point p1[] {p11, p12};
	Point p2[] {p21, p22};

	for (size_t i = 0; i < 2; ++i)
	{
		bool result = true;
		if ((p1[i].x > p2[i].x) != i)
		{
			std::swap(p1, p2);
			result = false;
		}

		if (p1[1].y * std::abs(p2[i].x - p1[0].x) + p1[0].y * std::abs(p1[1].x - p2[i].x)
				== p2[i].y * std::abs(p1[1].x - p1[0].x))
		{
			if (!result) std::swap(p1, p2);
			continue;
		}

		return (p1[1].y * std::abs(p2[i].x - p1[0].x) + p1[0].y * std::abs(p1[1].x - p2[i].x)
				> p2[i].y * std::abs(p1[1].x - p1[0].x)) != result;
	}

	return false;
}

