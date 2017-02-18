#pragma once
#include "Rect.h"

class Pad : public Rect
{
private:
public:
	const float m_width;
public:
	Pad() = default;
	Pad(float x, float y);
	Pad(Vec2 vec, float width = 100.0f);
	~Pad();
};

