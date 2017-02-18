#pragma once
#include "Rect.h"
#include "Vec2.h"
#include "Colors.h"

class MyBrick : public Rect
{
private:
	//float m_width;
	//float m_height;
	//Vec2 m_pos;

	Color m_color;
public:
	bool isDead = false;

	MyBrick();
	MyBrick(Vec2 left_top, float width, float height, Color color);
	~MyBrick();
};

