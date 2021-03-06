/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

#include <iostream>

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	ball(Vec2(400.0f, 300.0f), Vec2(300.0f + rand() % 201, 400.0f + rand() % 201)),
	pad(Vec2(gfx.ScreenWidth / 2, gfx.ScreenHeight - 50))
{
	timer.Mark();

	resetBall();
	setupBricks1();
	//m_bricks.push_back(MyBrick(Vec2(160.0f,100.0f), 100,100,Colors::Green));
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	float dt = timer.Mark();
	ball.Update(dt);


	//random ball movement
	//{
	//	if (dt > 2.0f) dt = 0;
	//	int dd = int(5000.0f * dt);
	//	if (dd = 0) dd = 1;
	//
	//	ball.m_v += Vec2(rand() % dd, rand() % dd);
	//	ball.m_v -= Vec2(rand() % dd, rand() % dd);
	//	if (ball.m_v.GetLength() > 4000) ball.m_v *= 0.9f;
	//}

	//Brick respawning for testing
	//bool respawn = false;
	//if (respawn)
	//{
	//	m_bricks.push_back(MyBrick(Vec2(float(rand() % 700), float(rand() % 500)), 100.0f, 100.0f, Colors::Green));
	//}
	
	//Hiting a Brick
	for (auto& ii : m_bricks)
	{	
		if (ball.isOverlappingWith(ii) && !ii.isDead)
		{	
			if (ball.isSideRebound(ii)) ball.ReboundX();
			else ball.ReboundY();

			ii.isDead = true;

			//respawn = true;
		}
	}
	

	//Pad Movement
	if (wnd.kbd.KeyIsPressed(VK_LEFT))
	{
		pad.move(Vec2(-700.0f * dt, 0.0f));
		if (pad.m_left < walls.m_left)
		{
			pad.moveTo(Vec2(walls.m_left,pad.m_top));
		}

	}
	if (wnd.kbd.KeyIsPressed(VK_RIGHT))
	{
		pad.move(700.0f * dt,0.0f);
		if (pad.m_right > walls.m_right)
		{
			pad.moveTo(Vec2(walls.m_right - (pad.m_right - pad.m_left), pad.m_top));
		}
	}

	//automatic Pad movement
	if (!wnd.kbd.KeyIsPressed(VK_LEFT) && !wnd.kbd.KeyIsPressed(VK_RIGHT))
	{
		if (ball.m_left + 7 < pad.m_left + pad.m_width / 2)
		{
			pad.move(Vec2(-370.0f * dt, 0.0f));
			if (pad.m_left < walls.m_left)
			{
				pad.moveTo(Vec2(walls.m_left, pad.m_top));
			}

		}
		if (ball.m_left + 7 > pad.m_left + pad.m_width / 2)
		{
			pad.move(370.0f * dt, 0.0f);
			if (pad.m_right > walls.m_right)
			{
				pad.moveTo(Vec2(walls.m_right - (pad.m_right - pad.m_left), pad.m_top));
			}
		}
	}

	//Pad rebound
	if (ball.isOverlappingWith(pad))
	{
		if (ball.isSideRebound(pad))
		{	
			ball.ReboundX();
			ball.ReboundY();
			if (ball.m_left < pad.m_left + pad.m_width / 2)
			{
				ball.move(Vec2(pad.m_left - ball.m_right, -1.0f));
			}
			else
			{
				ball.move(Vec2(pad.m_right - ball.m_left, -1.0f));
			}
		}
		else
		{
			ball.ReboundY();
			float sideReboundFactor = 400.0f;
			float speed = ball.m_v.GetLength();
			ball.m_v += Vec2(((ball.m_left + 7.0f) - (pad.m_left + pad.m_width / 2.0f)) / (pad.m_width / 2.0f) * sideReboundFactor, 0.0f);
			ball.m_v = ball.m_v.GetNormalized() * speed;
		}
	}

	//test for Ball == RIP
	if (ball.m_bottom > walls.m_bottom)
	{
		resetBall();
		setupBricks1();
	}

	//ball.doContainReboundPhysical(walls, dt);
	ball.doContainRebound(walls);

	//win check
	{
		bool isWon = true;
		for (auto& ii : m_bricks)
		{
			if (ii.isDead) continue;
			else isWon = false;
		}

		if (isWon)
		{
			resetBall();
			setupBricks1();
		}
	}

}

void Game::ComposeFrame()
{
	ball.Draw(gfx);

	for (auto& ii : m_bricks)
	{
		if(!ii.isDead) ii.Draw(gfx);
	}

	pad.Draw(gfx, Colors::Magenta);
}


void Game::setupBricks1()
{
	constexpr float space = 6.0f;
	const int numberOfBricks = 12;
	const float brickWidth = (gfx.ScreenWidth - space * numberOfBricks) / 12.0f;
	const float brickHeight = 30.0f;
	Color color = Colors::Red;

	m_bricks.clear();

	for (int ii = 0; ii < 12; ++ii)
	{

		for (int jj = 0;jj < 5;++jj)
		{
			switch (jj)
			{
				case 0:
					color = Colors::Cyan;
					break;
				case 1:
					color = Colors::Blue;
					break;
				case 2:
					color = Colors::Gray;
					break;
				case 3:
					color = Colors::Green;
					break;
				case 4:
					color = Colors::LightGray;
					break;

				default:
					std::cerr << "Unhandled for Color [setupBricks1()] aka jj > 4";
					break;
			}

			m_bricks.push_back(MyBrick(Vec2((brickWidth + space) * ii + space/2, (brickHeight + space) * jj), brickWidth, brickHeight, color));
		}
	}
}

void Game::resetBall()
{
	const float speed = 500.0f;
	ball = Ball(Vec2(400.0f, 300.0f), Vec2(rand() % 201 - 200.0f,  50.0f + rand() % 51).Normalize() * speed);
}