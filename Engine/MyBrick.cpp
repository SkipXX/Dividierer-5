#include "MyBrick.h"
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
