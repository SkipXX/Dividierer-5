#pragma once
#include "Rect.h"
#include "MyBrick.h"
#include "SpriteCodex.h"

class Ball : public Rect
{
public:
	Vec2 m_pos;
	Vec2 m_v;

public:
	Ball() = default;
	Ball(Vec2 pos, Vec2 v);

	void Draw(Graphics& gfx);
	void Update(float dt);
	void ReboundX();
	void ReboundY();
	void doContainRebound(const Rect& walls);

	void move(Vec2 dv);
};