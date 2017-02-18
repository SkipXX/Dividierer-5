#pragma once
#include "Rect.h"
#include "MyBrick.h"
#include "SpriteCodex.h"

class Ball : public Rect
{
private:
	Vec2 m_pos;
public:
	Vec2 m_v;

public:
	Ball() = default;
	Ball(Vec2 pos, Vec2 v);

	void Draw(Graphics& gfx);
	void Update(float dt);

	void ReboundX();
	void ReboundY();
	void doContainRebound(const Rect& walls);
	float doContainReboundPhysical(const Rect & walls, float dt);

	void move(Vec2 dv);
};