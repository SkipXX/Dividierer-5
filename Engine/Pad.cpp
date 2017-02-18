#include "Pad.h"

Pad::Pad(float x, float y)
	:
	Pad(Vec2(x,y))
{
}

Pad::Pad(Vec2 vec, float width)
	:
	Rect(vec,width,25.0f),
	m_width(width)
{
}


Pad::~Pad()
{
}
