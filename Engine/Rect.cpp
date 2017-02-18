#include "Rect.h"



Rect::Rect()
{
	m_left = 0;
	m_right = 0;
	m_top = 0;
	m_bottom = 0;
}

Rect::Rect(float left, float right, float top, float bottom)
	:
	m_left(left),
	m_right(right),
	m_top(top),
	m_bottom(bottom)
{
}

Rect::Rect(Vec2 left_top, Vec2 right_bottom)
	:
	Rect(left_top.x, right_bottom.x, left_top.y, right_bottom.y)
{
}

Rect::Rect(Vec2 left_top, float width, float height)
	:
	Rect(left_top, left_top + Vec2(width, height))
{
}


Rect::~Rect()
{
}



//this is kinda problematic as halfWidth is NOT half the width but: width = 2*halfWidth + 1
Rect Rect::fromMiddle(Vec2 middle, float halfWidth, float halfHeight)
{
	return Rect(middle.x - halfWidth, middle.x + halfWidth, middle.y - halfHeight, middle.y + halfHeight);
}