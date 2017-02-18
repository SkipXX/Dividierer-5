#include "MyBrick.h"



MyBrick::MyBrick()
{
}

MyBrick::MyBrick(Vec2 left_top, float width, float height, Color color)
	:
	Rect(left_top, width, height),
	m_color(color)
{
}


MyBrick::~MyBrick()
{
}

void MyBrick::Draw(Graphics & gfx)
{
	gfx.DrawRect(int(m_left), int(m_top), int(m_right), int(m_bottom), m_color);
}
