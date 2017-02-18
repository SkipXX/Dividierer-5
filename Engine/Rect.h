#pragma once

#include "Vec2.h"

class Rect
{
private:
	float m_left, m_right, m_top, m_bottom;

public:
	Rect();
	Rect(float left, float right, float top, float bottom);
	Rect(Vec2 left_top, Vec2 right_bottom);
	Rect(Vec2 top_left, float width, float height);
	~Rect();

	static Rect fromMiddle(Vec2 middle, float halfWidth, float halfHeight);
};

