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
#include <cassert>

#include "drawLine.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	ball(Vec2(400.0f, 300.0f), Vec2(-300.0f, -300.0f)),
	pad(Vec2(gfx.ScreenWidth / 2, gfx.ScreenHeight - 50)),
	Music(L"DesDividierersGebeerde2.wav",0.0f,21.338f)
{
	timer.Mark();

	Music.Play(1.0f,0.5f);

	resetBall();

	setupBricks1();
	//m_bricks.push_back(MyBrick(Vec2(160.0f,100.0f), 100,100,Colors::Green));
}

void Game::Go()
{
	gfx.BeginFrame();	
	///
	inputHandling();

	float dt = timer.Mark() / float(Iterations);
	assert(dt < 0.01f);

	if (!pause)
	{
		for (int ii = 0; ii < Iterations; ++ii)
		{
			UpdateModel(dt);
		}
	}
	///
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel(float dt)
{
	//generic pos Update
	ball.Update(dt);

	//Options
	//a is automatic movement


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
	//
	//Brick respawning for testing
	//bool respawn = false;
	//if (respawn)
	//{
	//	m_bricks.push_back(MyBrick(Vec2(float(rand() % 700), float(rand() % 500)), 100.0f, 100.0f, Colors::Green));
	//}
	
	//Hiting a Brick
	for (auto& ii : m_bricks)
	{	
		bool isHit = false;
		if (ball.isOverlappingWith(ii) && !ii.isDead)
		{	
			if (ball.isSideRebound(ii))
			{	

				//CRUSH THE BRICK
				if (abs(ball.m_v.x) > brickCrushFaktor * abs(ball.m_v.y))
				{
					ball.m_v.x *= brickCrushSpeedLoss;
				}
				else ball.ReboundX();
			}
			else ball.ReboundY();

			ii.isDead = true;
			isHit = true;
			ball.PadCooldown = false;
		}
		//Only bounce once
		if (isHit) break;
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
	if (automaticMovement && !wnd.kbd.KeyIsPressed(VK_LEFT) && !wnd.kbd.KeyIsPressed(VK_RIGHT))
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
	if (!ball.PadCooldown && ball.isOverlappingWith(pad))
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
		}

		ball.PadCooldown = true;
	}


	//test for Ball == RIP
	if (ball.m_bottom > walls.m_bottom)
	{
		resetBall();
		Lives--;

		if (Lives == 0)
		{
			setupBricks1();
			Lives = 3;
		}
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
	//BRICKCRUSH
	if (abs(ball.m_v.x) > brickCrushFaktor * abs(ball.m_v.y))
	{
		int x = int(ball.GetPos().x + ball.m_radius);
		int y = int(ball.GetPos().y + ball.m_radius);
		int rad = 10;
		Color color = Colors::Red;

		gfx.DrawCircle(x, y, rad, color);

		Vec2 SchweifDir = -ball.m_v.GetNormalized();
		int ScheifAnzahl = 5;
		int MaxScheifLaenge = 30;
		int MinScheifLaenge = 10;

		for (int ii = 0; ii < ScheifAnzahl; ii++)
		{
			int x0 = x + int((rand() % rad - rad / 2) * 1.8f);
			int y0 = y + int((rand() % rad - rad / 2) * 1.8f);

			Vec2 p0 = Vec2(x0, y0);
			Vec2 p1 = p0 + SchweifDir *float(10 + rand() % (MaxScheifLaenge - MinScheifLaenge + 1));

			gfx.DrawLine(p0,p1,color);
		}
	}

	//Ball
	ball.Draw(gfx);

	//Bricks
	for (auto& ii : m_bricks)
	{
		if(!ii.isDead) ii.Draw(gfx);
	}

	//Pad
	pad.Draw(gfx, Colors::Magenta);

	//?
	//gfx.DrawLine(Vec2(pad.m_left + pad.m_width / 2, pad.m_top), Vec2(ball.m_left + 7, ball.m_top + 7));

	//Lives
	for (int ii = 0; ii < Lives; ii++)
	{
		int x = 20 + 7 * ii;
		int y = gfx.ScreenHeight - 20;

		gfx.PutPixel(x+0,y + 1, Colors::Red);
		gfx.PutPixel(x+0,y + 2, Colors::Red);
		gfx.PutPixel(x+1,y + 0, Colors::Red);
		gfx.PutPixel(x+1,y + 1, Colors::Red);
		gfx.PutPixel(x+1,y + 2, Colors::Red);
		gfx.PutPixel(x+1,y + 3, Colors::Red);
		gfx.PutPixel(x+2,y + 0, Colors::Red);
		gfx.PutPixel(x+2,y + 1, Colors::Red);
		gfx.PutPixel(x+2,y + 2, Colors::Red);
		gfx.PutPixel(x+2,y + 3, Colors::Red);
		gfx.PutPixel(x+2,y + 4, Colors::Red);
		gfx.PutPixel(x+3,y + 2, Colors::Red);
		gfx.PutPixel(x+3,y + 3, Colors::Red);
		gfx.PutPixel(x+3,y + 4, Colors::Red);
		gfx.PutPixel(x+4,y + 0, Colors::Red);
		gfx.PutPixel(x+4,y + 1, Colors::Red);
		gfx.PutPixel(x+4,y + 2, Colors::Red);
		gfx.PutPixel(x+4,y + 3, Colors::Red);
		gfx.PutPixel(x+4,y + 4, Colors::Red);
		gfx.PutPixel(x+5,y + 0, Colors::Red);
		gfx.PutPixel(x+5,y + 1, Colors::Red);
		gfx.PutPixel(x+5,y + 2, Colors::Red);
		gfx.PutPixel(x+5,y + 3, Colors::Red);
		gfx.PutPixel(x+6,y + 1, Colors::Red);
		gfx.PutPixel(x+6,y + 2, Colors::Red);
	}
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
	const float speed = 300.0f;
	ball = Ball(Vec2(400.0f, 200.0f), Vec2(rand() % 201 - 100.0f,  speed));

	//test code
	//ball = Ball(Vec2(400.0f - 10, 300.0f), Vec2(-300.0f, -300.0f));

}

void Game::inputHandling()
{
	//Esc to exit ... NOT BUFFERED
	if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
	{
		exit(1337);
	}

	///Options
	//pause funktion
	if (wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		inputBuffer |= 0x1;
	}

	//if RETURN is pressed (reset objects)
	if (wnd.kbd.KeyIsPressed(VK_RETURN))
	{
		inputBuffer |= 0x2;
	}

	//A (automatic movement)
	if (wnd.kbd.KeyIsPressed(0x41))
	{
		inputBuffer |= 0x4;
	}

	//M (music toggle)
	if (wnd.kbd.KeyIsPressed(0x4D))
	{
		inputBuffer |= 0x8;
	}

	if (inputBuffer)
	{
		// 0x1 = 'SPACE'-Key ... pause funktion
		if ((inputBuffer & 0x1) && !wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			pause = !pause;
			inputBuffer &= ~0x1;
		}

		// 0x2 = RETURN-Key ... RESET
		if ((inputBuffer & 0x2) && !wnd.kbd.KeyIsPressed(VK_RETURN))
		{
			Lives = 3;
			resetBall();
			setupBricks1();

			inputBuffer &= ~0x2;
		}

		// 0x4 = 'a'-Key ... automatic movement ON/OFF
		if ((inputBuffer & 0x4) && !wnd.kbd.KeyIsPressed(0x41))
		{
			automaticMovement = !automaticMovement;
			inputBuffer &= ~0x4;
		}

		// 0x8 = 'M'-Key ... PLAY/STOP MUSIC
		if ((inputBuffer & 0x8) && !wnd.kbd.KeyIsPressed(0x4D))
		{
			if (m_music)
			{
				Music.StopAll();
				m_music = false;
			}
			else
			{
				Music.Play(1.0f,0.5f);
				m_music = true;
			}
			inputBuffer &= ~0x8;
		}
	}
	///
}