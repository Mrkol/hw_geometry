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

		//    f-----e
		//          |
		//          |
		//    c-----d
		//    |
		//    |
		//    b
		//     \.
		//      \.
		//       a
		
		sortedEvents_.push_back({c.x, (i + 2) % n,
			b.x < c.x || (b.x == c.x && a.x < b.x)
				? EventType::SegmentDelayedStart : EventType::SegmentStart});
		sortedEvents_.push_back({d.x, (i + 2) % n, EventType::SegmentEnd});
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
			size_t n = polygon_.size();


			bool matched = false;

			auto above = segmentSet_.lower_bound({{querry, querry}, index});
			if (above != segmentSet_.end())
			{
				size_t i = above->index;
				matched = matched || 
					PointOnSegment(
						{polygon_[i], polygon_[(i + 1) % n]}, querry)
					|| PointOnSegment(
						{polygon_[(i + 1) % n], polygon_[(i + 2) % n]}, querry)
					|| PointOnSegment(
						{polygon_[(n + i - 1) % n], polygon_[i % n]}, querry);
			}

			if (above != segmentSet_.begin())
			{
				auto below = std::prev(above);
				size_t j = below->index;
				matched = matched ||
					PointOnSegment(
						{polygon_[j], polygon_[(j + 1) % n]}, querry)
					|| PointOnSegment(
						{polygon_[(j + 1) % n], polygon_[(j + 2) % n]}, querry)
					|| PointOnSegment(
						{polygon_[(n + j - 1) % n], polygon_[j % n]}, querry);
			}

			if (matched)
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

