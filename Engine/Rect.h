#pragma once

#include "Vec2.h"

class Rect
{
public:
	float m_left, m_right, m_top, m_bottom;

public:
	Rect();
	Rect(float left, float right, float top, float bottom);
	Rect(Vec2 left_top, Vec2 right_bottom);
	Rect(Vec2 top_left, float width, float height);
	~Rect();

	static Rect fromMiddle(Vec2 middle, float halfWidth, float halfHeight);

	bool isOverlappingWith(const Rect& rekt) const;

	virtual void move(const Vec2& dv);
};

