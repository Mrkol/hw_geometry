#pragma once
#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP
#include <iostream>


using Scalar = long long;

class Point
{
public:
	Scalar x;
	Scalar y;
};

class Segment
{
public:
	Point a;
	Point b;
};

template<typename TVector>
inline auto operator%(const TVector& first, const TVector& second)
{
	auto[x1, y1] = first;
	auto[x2, y2] = second;
	return x1*y2 - x2*y1;
}

template<typename TVector>
inline auto operator*(const TVector& first, const TVector& second)
{
	auto[x1, y1] = first;
	auto[x2, y2] = second;
	return x1*x2 + y1*y2;
}

template<typename TVector>
inline TVector operator+(const TVector& first, const TVector& second)
{
	auto[x1, y1] = first;
	auto[x2, y2] = second;	
	return {x1 + x2, y1 + y2};
}

template<typename TVector>
inline TVector operator-(const TVector& first, const TVector& second)
{
	auto[x1, y1] = first;
	auto[x2, y2] = second;	
	return {x1 - x2, y1 - y2};
}

template<typename TVector>
inline bool operator!=(const TVector& first, const TVector& second)
{
	return !(first == second);
}

template<typename TVector>
inline bool operator==(const TVector& first, const TVector& second)
{
	auto[x1, y1] = first;
	auto[x2, y2] = second;
	return x1 == x2 && y1 == y2;
}

inline bool PointOnSegment(const Segment& seg, const Point& point)
{
	return (seg.a - seg.b) % (point - seg.b) == 0
		&& (seg.a - seg.b) * (point - seg.b) >= 0
		&& (seg.b - seg.a) * (point - seg.a) >= 0;
}

inline std::ostream& operator<<(std::ostream& out, const Point& point)
{
	out << "(" << point.x << ", " << point.y << ")";
	return out;
}

inline std::istream& operator>>(std::istream& in, Point& point)
{
	in >> point.x >> point.y;
	return in;
}

inline std::ostream& operator<<(std::ostream& out, const Segment& seg)
{
	out << "(" << seg.a << ", " << seg.b << ")";
	return out;
}


#endif // GEOMETRY_HPP
