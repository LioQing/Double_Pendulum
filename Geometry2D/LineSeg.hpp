#ifndef LINESEG_HPP
#define LINESEG_HPP

#include <iostream>
#include "Vec2.hpp"

template <typename T>
class LineSeg
{
public:

    Vec2<T> p1, p2;

    LineSeg() : p1(0.0, 0.0), p2(0.0, 0.0) {}
	template <typename U, typename S, typename V, typename R> LineSeg(const U& x1, const S& y1, const V& x2, const R& y2) : p1(x1, y1), p2(x2, y2) {}
	template <typename U, typename S> LineSeg(const U& x, const S& y) : p1(0.0, 0.0), p2(x, y) {}
	template <typename U, typename S> LineSeg(const Vec2<U>& p1, const Vec2<S>& p2) : p1(p1), p2(p2) {}
	template <typename U> LineSeg(const Vec2<U>& p) : p1(0.0, 0.0), p2(p) {}
	template <typename U> LineSeg(const LineSeg<U>& l) : p1(l.p1), p2(l.p2) {}

	template <typename U>
	LineSeg& operator=(const LineSeg<U>& l)
	{
		p1 = l.p1;
		p2 = l.p2;
		return *this;
	}
	template <typename U>
	LineSeg& operator=(const Vec2<U>& v)
	{
		p1 = Vec2<U>::Zero();
		p2 = v;
		return *this;
	}

	template <typename U>
	LineSeg operator+(const U& s) const
	{
		return LineSeg(p1 + s, p2 + s);
	}
	template <typename U>
	LineSeg operator-(const U& s) const
	{
		return LineSeg(p1 - s, p2 - s);
	}
	template <typename U>
	LineSeg operator*(const U& s) const 
	{
		return LineSeg(p1 * s, p2 * s);
	}
	template <typename U>
	LineSeg operator/(const U& s) const
	{
		return LineSeg(p1 / s, p2 / s);
	}
	template <typename U>
	LineSeg operator%(const U& s) const
	{
		return LineSeg(p1 % s, p2 % s);
	}

	template <typename U>
	LineSeg& operator+=(const U& s)
	{
		p1 += s;
		p2 += s;
		return *this;
	}
	template <typename U>
	LineSeg& operator-=(const U& s)
	{
		p1 -= s;
		p2 -= s;
		return *this;
	}
	template <typename U>
	LineSeg& operator*=(const U& s)
	{
		p1 *= s;
		p2 *= s;
		return *this;
	}
	template <typename U>
	LineSeg& operator/=(const U& s)
	{
		p1 /= s;
		p2 /= s;
		return *this;
	}
	template <typename U>
	LineSeg& operator%=(const U& s)
	{
		p1 %= s;
		p2 %= s;
		return *this;
	}

	LineSeg& operator++()
	{
		p1 += 1.0;
		p2 += 1.0;
		return *this;
	}
	LineSeg& operator--()
	{
		p1 -= 1.0;
		p2 -= 1.0;
		return *this;
	}
	LineSeg operator++(int)
	{
		LineSeg tmp(p1, p2);
		p1 += 1.0;
		p2 += 1.0;
		return tmp;
	}
	LineSeg operator--(int)
	{
		LineSeg tmp(p1, p2);
		p1 -= 1.0;
		p2 -= 1.0;
		return tmp;
	}

	template <typename U>
	friend bool operator==(const LineSeg& l1, const LineSeg<U>& l2)
	{
		return l1.p1 == l2.p1 && l1.p2 == l2.p2;
	}
	template <typename U>
	friend bool operator!=(const LineSeg& l1, const LineSeg<U>& l2)
	{
		return !(l1.p1 == l2.p1 && l1.p2 == l2.p2);
	}

	friend std::ostream& operator<<(std::ostream& os, const LineSeg& l)
	{
		os << "(" << l.p1 << ") (" << l.p2 << ")";
		return os;
	}
	friend std::istream& operator>>(std::istream& is, LineSeg& l)
	{
		is >> l.p1 >> l.p2;
		return is;
	}

	template <typename U, typename S>
	LineSeg& Set(const Vec2<U>& p1, const Vec2<S>& p2)
	{
		this->p1 = p1;
		this->p2 = p2;
		return *this;
	}
	template <typename U, typename S, typename V, typename R>
	LineSeg& Set(const U& x1, const S& y1, const V& x2, const R& y2)
	{
		p1.x = x1;
		p1.y = y1;
		p2.x = x2;
		p2.y = x2;
		return *this;
	}

	template <typename U, typename S = double>
	LineSeg<double> Rotated(const U& theta, const Vec2<S>& pt = Vec2<S>::Zero()) const
	{
		Vec2<double> _p1 = this->p1 - pt;
		Vec2<double> _p2 = this->p2 - pt;
		Vec2<double> p1 = _p1.Rotated(theta) + pt;
		Vec2<double> p2 = _p2.Rotated(theta) + pt;

		return LineSeg<double>(p1, p2);
	}
	template <typename U, typename S = double>
	LineSeg& Rotate(const U& theta, const Vec2<S>& pt = Vec2<S>::Zero())
	{
		Vec2<double> _p1 = this->p1 - pt;
		Vec2<double> _p2 = this->p2 - pt;
		p1 = _p1.Rotated(theta) + pt;
		p2 = _p2.Rotated(theta) + pt;

		return *this;
	}

	double Length() const
	{
		return p1.Distance(p2);
	}

	template <typename U>
	bool Lies(const Vec2<U>& pt) const
	{
		if (pt.x <= std::max(p1.x, p2.x) && pt.x >= std::min(p1.x, p2.x) &&
			pt.y <= std::max(p1.y, p2.y) && pt.y >= std::min(p1.y, p2.y) &&
			p1.Orientation(pt, p2) == 0)
			return true;

        return false;
	}

	template <typename U>
	bool Intersect(const LineSeg<U>& l) const
	{
		if (p1.Orientation(p2, l.p1) != p1.Orientation(p2, l.p2) && 
			l.p1.Orientation(l.p2, p1) != l.p1.Orientation(l.p2, p2)) 
			return true;

		if (Lies(l.p1)) return true;
		if (Lies(l.p2)) return true;
		if (l.Lies(p1)) return true;
		if (l.Lies(p2)) return true;

		return false;
	}

	Vec2<double> MidPt() const
	{
		return Vec2<double>((static_cast<double>(p1.x) + p2.x) / 2, (static_cast<double>(p1.y) + p2.y) / 2);
	}
	template <typename U, typename S>
	Vec2<double> Section(const U& r1, const S& r2)
	{
		auto x = (static_cast<double>(r1) * p2.x + static_cast<double>(r2) * p1.x) / (static_cast<double>(r1) + r2);
		auto y = (static_cast<double>(r1) * p2.y + static_cast<double>(r2) * p1.y) / (static_cast<double>(r1) + r2);
		return Vec2<double>(x, y);
	}

	template <typename U>
	LineSeg<U> Cast() const
	{
		return LineSeg<U>(p1, p2);
	}
};

typedef LineSeg<double> LineSegd;
typedef LineSeg<float> LineSegf;
typedef LineSeg<int> LineSegi;

#endif