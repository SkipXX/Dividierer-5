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
	move(m_v * dt);
}

void Ball::ReboundX()
{
	m_v.x = -m_v.x;
}

void Ball::ReboundY()
{
	m_v.y = -m_v.y;
}

void Ball::doContainRebound(const Rect & walls)
{
	if (m_left < walls.m_left)
	{
		move(Vec2(walls.m_left - m_left, 0.0f));
		ReboundX();
	}
	else if (m_right > walls.m_right)
	{
		move(Vec2(walls.m_right - m_right, 0.0f));
		ReboundX();
	}

	if (m_top < walls.m_top)
	{
		move(Vec2(0.0f, walls.m_top - m_top));
		ReboundY();
	}
	else if (m_bottom > walls.m_bottom)
	{
		move(Vec2(0.0f, walls.m_bottom - m_bottom));
		ReboundY();
	}
}
float Ball::doContainReboundPhysical(const Rect & walls, float dt)
{	
	constexpr float precision = 1000.0f;
	bool fixed = false;
	float correction_output = 1.0f;

	do
	{

		if (m_left < walls.m_left || m_right > walls.m_right)
		{
			move(m_v * -dt);

			int ii = 0;
			while (!(m_left < walls.m_left || m_right > walls.m_right))
			{
				move(m_v * (dt / precision));
				ii++;
			}

			ReboundX();

			move(m_v * (dt * (precision - float(ii)) / precision));

			correction_output = float(ii) / precision;
			continue;
		}
		else if (m_top < walls.m_top || m_bottom > walls.m_bottom)
		{
			move(m_v * -dt);

			int ii = 0;
			while (!(m_top < walls.m_top || m_bottom > walls.m_bottom))
			{
				move(m_v * (dt / precision));
				ii++;
			}

			ReboundY();

			move(m_v * (dt * (precision - float(ii)) / precision));

			correction_output = float(ii) / precision;
			continue;
		}
		else fixed = true;

	} while (!fixed);

	return correction_output;
}

void Ball::move(Vec2 dv)
{
	Rect::move(dv);
	m_pos.x = m_left;
	m_pos.y = m_top;
}
