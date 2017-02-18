#include "Ball.h"

Ball::Ball(Vec2 pos, Vec2 v)
	:
	Rect(pos, 14, 14),
	m_pos(pos),
	m_v(v)
{
}

void Ball::Draw(Graphics& gfx)
{
	SpriteCodex::DrawBall(m_pos, gfx);
}

void Ball::Update(float dt)
{	
	m_pos += m_v * dt;
	m_left += m_v.x * dt;
	m_right += m_v.x *dt;
	m_top += m_v.y * dt;
	m_bottom += m_v.y * dt;
}
