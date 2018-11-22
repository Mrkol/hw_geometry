#include "ContainmentAlgorithm.hpp"
#include <functional>
#include <iostream>

std::vector<QuerryResultType> ContainmentAlgorithm::Calculate()
{
	sortedEvents_.reserve(2 * polygon_.size() + querries_.size());

	for (size_t i = 0; i < polygon_.size(); ++i)
	{
		auto n = polygon_.size();
		auto a = polygon_[i];
		auto b = polygon_[(i + 1) % n];
		auto c = polygon_[(i + 2) % n];
		auto d = polygon_[(i + 3) % n];
		if (b.x == c.x)
			continue;
		if (b.x > c.x)
		{
			std::swap(b, c);
			std::swap(a, d);
		}
		
		sortedEvents_.push_back({b.x, (i + 1) % n,
			a.x < b.x ? EventType::SegmentDelayedStart : EventType::SegmentStart});
		sortedEvents_.push_back({c.x, (i + 1) % n, EventType::SegmentEnd});
	}

	for (size_t i = 0; i < querries_.size(); ++i)
	{
		sortedEvents_.push_back({querries_[i].x, i, EventType::Querry});	
	}

	std::sort(sortedEvents_.begin(), sortedEvents_.end(),
		[](const Event& first, const Event& second)
		{
			return first.x < second.x
				|| (first.x == second.x && first.type < second.type);
		});

	std::vector<QuerryResultType> result(querries_.size());
	for (size_t scanline = 0; scanline < sortedEvents_.size(); ++scanline)
	{
		auto index = sortedEvents_[scanline].index;
		auto type = sortedEvents_[scanline].type;

		if (type == EventType::Querry)
		{
			Point querry = querries_[index];
			size_t k = segmentSet_.order_of_key({{querry, querry}, index});
			auto it = segmentSet_.lower_bound({{querry, querry}, index});

			bool border = false;
			if (it != segmentSet_.end())
			{
				if (PointOnSegment(it->segment, querry))
					border = true;
				else if (it != segmentSet_.begin()
					&& std::max(std::prev(it)->index, it->index)
						- std::min(std::prev(it)->index, it->index) == 2)
				{
					border = pointOnVerticalEdge(it->segment, 
						std::prev(it)->segment, querry);
				}
			}

			if (border)
			{
				result[index] = QuerryResultType::Border;
			}
			else if (k % 2)
			{
				result[index] = QuerryResultType::Inside;
			}
			else
			{
				result[index] = QuerryResultType::Outside;
			}
		}
		else
		{
			Segment seg {polygon_[index], polygon_[(index + 1) % polygon_.size()]};
			
			if (type == EventType::SegmentEnd)
				segmentSet_.erase({seg, index});
			else
				segmentSet_.insert({seg, index});
		}


		/*std::cout << "Ev: " << int(type) << " ";
		std::cout << "Sz: " << segmentSet_.size() << " ";
		std::cout << "{";
		size_t i = 0;
		for (auto elemtnt : segmentSet_)
		{
			std::cout << elemtnt.segment;
			if (i + 1 != segmentSet_.size())
				std::cout << "; ";
			++i;
		}
		std::cout << "}\n";*/
	}

	segmentSet_.clear();
	sortedEvents_.clear();
	return result;
}

bool ContainmentAlgorithm::pointOnVerticalEdge(const Segment& after,
	const Segment& before, const Point& point)
{
	auto[p11, p12] = after;
	auto[p21, p22] = before;

	// p11______p12
	//           |
	//           p
	//           |
	//          p21______p22

	if (p12.x != point.x)
		std::swap(p11, p12);
	if (p12.x != point.x)
		return false;

	if (p22.x != point.x)
		std::swap(p21, p22);
	if (p22.x != point.x)
		return false;

	return true;
}


bool ContainmentAlgorithm::SegmentSetElementComparator::
	operator()(const SegmentSetElement& first, const SegmentSetElement& second) const
{
	return operator()(first.segment, second.segment);
}

bool ContainmentAlgorithm::SegmentSetElementComparator::
	operator()(const Segment& first, const Segment& second) const
{
	//   p11___.________________p12
	//       p21__________p22

	
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

