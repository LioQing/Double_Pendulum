#pragma once

#include <cstdint>
#include <vector>

template <typename T>
class Matrix
{
public:

	std::vector<std::vector<T>> matrix;

	uint32_t width, height;

	Matrix(std::vector<std::vector<T>> matrix) : matrix(matrix)
	{
		width = matrix.at(0).size();
		height = matrix.size();
	}

	Matrix(uint32_t width = 1, uint32_t height = 1, T val = 0) : width(width), height(height)
	{
		matrix.resize(height);
		for (auto& h : matrix) h.resize(width);

		Fill(val);
	}

	void SetSize(uint32_t w, uint32_t h, T val)
	{
		matrix.resize(h);
		for (auto& h : matrix) h.resize(w);

		for (auto tmp_h = 0u; tmp_h < h; ++tmp_h)
		{
			for (auto tmp_w = 0u; tmp_w < w; ++tmp_w)
			{
				if (tmp_h < height || tmp_w < width) continue;
				At(tmp_w, tmp_h) = val;
			}
		}

		width = w;
		height = h;
	}

	std::vector<T>& Row(uint32_t r)
	{
		return matrix.at(r);
	}
	// to not affect the performance, returning pointers is necessary
	std::vector<T*> Column(uint32_t c)
	{
		std::vector<T*> col;
		for (auto h = 0u; h < height; ++h) col.push_back(&matrix.at(h).at(c));
		return col;
	}

	T& At(uint32_t x, uint32_t y)
	{
		if (x >= width || x < 0 || y >= height || y < 0) return matrix.at(0).at(0);
		return matrix.at(y).at(x);
	}
	T At(uint32_t x, uint32_t y) const
	{
		if (x >= width || x < 0 || y >= height || y < 0) return 0;
		return matrix.at(y).at(x);
	}

	void Fill(T val)
	{
		for (auto& h : matrix)
		{
			for (auto& w : h)
			{
				w = val;
			}
		}
	}

	void DrawLine(int x1, int y1, int x2, int y2, T val)
	{
		int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		dx = x2 - x1; dy = y2 - y1;
		dx1 = abs(dx); dy1 = abs(dy);
		px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
		if (dy1 <= dx1)
		{
			if (dx >= 0)
				{ x = x1; y = y1; xe = x2; }
			else
				{ x = x2; y = y2; xe = x1;}

			At(x, y) = val;
			
			for (i = 0; x<xe; i++)
			{
				x = x + 1;
				if (px<0)
					px = px + 2 * dy1;
				else
				{
					if ((dx<0 && dy<0) || (dx>0 && dy>0)) y = y + 1; else y = y - 1;
					px = px + 2 * (dy1 - dx1);
				}
				At(x, y) = val;
			}
		}
		else
		{
			if (dy >= 0)
				{ x = x1; y = y1; ye = y2; }
			else
				{ x = x2; y = y2; ye = y1; }

			At(x, y) = val;

			for (i = 0; y<ye; i++)
			{
				y = y + 1;
				if (py <= 0)
					py = py + 2 * dx1;
				else
				{
					if ((dx<0 && dy<0) || (dx>0 && dy>0)) x = x + 1; else x = x - 1;
					py = py + 2 * (dx1 - dy1);
				}
				At(x, y) = val;
			}
		}
	}
};

typedef Matrix<int> Matrixi;
typedef Matrix<float> Matrixf;