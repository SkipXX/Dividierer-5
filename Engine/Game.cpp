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

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	ball(Vec2(400.0f, 100.0f), Vec2(500.0f, 500.0f))
{
	timer.Mark();

	m_bricks.push_back(MyBrick(Vec2(160.0f,100.0f), 100,30,Colors::Green));
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

	ball.doContainRebound(walls);

	bool respawn = false;
	for (auto& ii : m_bricks)
	{	
		if (ball.isOverlappingWith(ii) && !ii.isDead)
		{
			ball.ReboundY();
			ii.isDead = true;

			respawn = true;
		}
	}
	if (respawn)
	{
		m_bricks.push_back(MyBrick(Vec2(rand() % 700, rand() % 570), 100, 30, Colors::Green));
	}

}

void Game::ComposeFrame()
{
	ball.Draw(gfx);

	for (auto& ii : m_bricks)
	{
		if(!ii.isDead) ii.Draw(gfx);
	}
}
