#pragma once
#include "Rect.h"

class Pad : public Rect
{
private:

public:
	Pad() = default;
	Pad(float x, float y);
	Pad(Vec2 vec);
	~Pad();
};

