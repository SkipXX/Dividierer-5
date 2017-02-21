#include "drawLine.h"

#include <vector>

void drawLine(Graphics& gfx, Vec2 p_1, Vec2 p_2, Color color)
{
	std::vector<Vec2> line_points;
	const Vec2 line_direction = (p_2 - p_1).Normalize();
	float lenght = (p_2 - p_1).GetLength();

	line_points.push_back(p_1);

	for (int ii = 0; ii < static_cast<int>(lenght); ii++)
	{
		p_1 += line_direction;
		line_points.push_back(p_1);
	}
	
	line_points.push_back(p_2);

	for (auto& ii : line_points)
	{
		gfx.PutPixel(int(ii.x),int(ii.y), color);
	}
}