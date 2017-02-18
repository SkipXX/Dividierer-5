#include "Pad.h"

Pad::Pad(float x, float y)
	:
	Pad(Vec2(x,y))
{
}

Pad::Pad(Vec2 vec)
	:
	Rect(vec,100.0f,25.0f)
{
}


Pad::~Pad()
{
}
