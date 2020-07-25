#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>

#include "Tools/LConsoleScreen.hpp"
#include "Geometry2D/Vec2.hpp"
#include "Geometry2D/LineSeg.hpp"
#include "Matrix.hpp"

#ifndef M_PI
#define M_PI 3.1415926535897932
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif

using DeltaTime = uint32_t;

const int w = 300;
const int h = 150;

LConsoleScreen lcs;

namespace tu
{
	template <typename T>
	Vec2<int> adjust(const Vec2<T>& v)
	{
		Vec2<int> tmp;
		tmp.x = v.x;
		tmp.y = v.y;
		
		tmp.y *= -1;
		tmp += Vec2<int>(w / 2, h / 2 - 35);

		return tmp;
	}

	template <typename T>
	void draw(const Vec2<T>& v, const char& c = 0xdb, const short& col = 0x000F)
	{
		lcs.Draw(adjust(v).x, adjust(v).y, c, col);
	}

	template <typename T>
	void draw(const LineSeg<T>& l, const char& c = 0xdb, const short& col = 0x000F)
	{
		lcs.DrawLine(adjust(l.p1).x, adjust(l.p1).y, adjust(l.p2).x, adjust(l.p2).y, c, col);
	}

	template <typename T>
	void draw(const Matrix<T>& m, const char& c = 0xdb)
	{
		for (auto y = 0; y < m.height; ++y)
		{
			for (auto x = 0; x < m.width; ++x)
			{
				lcs.Draw(x, y, ' ', m.At(x, y));
			}
		}
	}
}

const double GRAVITY = 1;

const double r1 = 50.0;
const double r2 = 50.0;

double m1 = 20.0;
double m2 = 20.0;
double t1 = M_PI / 2;
double t2 = M_PI / 2;
double v1 = 0.0;
double v2 = 0.0;
LineSegd _s1{ Vec2d{ 0, -r1 } };
LineSegd _s2{ Vec2d{ 0, -r1 }  };
LineSegd s1{ Vec2d{ 0, -r2 }  };
LineSegd s2{ Vec2d{ 0, -r2 }  };

Vec2<double> pv{ -1, -1 };

Matrix<short> trace = Matrix<short>(w, h, BG_BLACK);

void Init()
{

}

void Update(DeltaTime delta_time)
{
	// cal angular acceleration
	double a1 = (-GRAVITY * (2 * m1 + m2) * sin(t1) - m2 * GRAVITY * sin(t1 - 2 * t2) - 2 * sin(t1 - t2) * m2 * (v2 * v2 * r2 + v1 * v1 * r1 * cos(t1 - t2))) / 
		(r1 * (2 * m1 + m2 - m2 * cos(2 * t1 - 2 * t2)));
	double a2 = (2 * sin(t1 - t2) * (v1 * v1 * r1 * (m1 + m2) + GRAVITY * (m1 + m2) * cos(t1) + v2 * v2 * r2 * m2 * cos(t1 - t2))) /
		(r2 * (2 * m1 + m2 - m2 * cos(2 * t1 - 2 * t2)));

	// sync the pendulum
	s1 = _s1.Rotated(t1);
	s2 = _s2.Rotated(t2) + s1.p2;

	// cal move
	v1 += a1 * delta_time / 70;
	v2 += a2 * delta_time / 70;
	t1 += v1 * delta_time / 70;
	t2 += v2 * delta_time / 70;

	// trace
	if (pv.x != -1 && pv.y != -1)
		trace.DrawLine(tu::adjust(pv).x, tu::adjust(pv).y, tu::adjust(s2.p2).x, tu::adjust(s2.p2).y, BG_DARK_GREY);

	// record last point
	pv = s2.p2;
}

void Draw()
{
	tu::draw(trace);
	tu::draw(s1);
	tu::draw(s2);
	tu::draw(s1.p2, PIXEL_SOLID, FG_BLUE);
	tu::draw(s2.p2, PIXEL_SOLID, FG_RED);
}

int main()
{
    using clock = std::chrono::high_resolution_clock;

    lcs.Init(w, h, 6, 6, "Geometry2D");

    auto start_time = clock::now();

	Init();

    for (;;)
    {
        auto delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - start_time).count();
        start_time = clock::now();

		Update(delta_time);

        lcs.Clear();
        Draw();

        lcs.Display();
    }

    return 0;
}
