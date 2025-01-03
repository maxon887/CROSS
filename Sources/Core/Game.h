/*	Copyright © 2018 Maksim Lukyanov

	This file is part of Cross++ Game Engine.

	Cross++ Game Engine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Cross++ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Cross++.  If not, see <http://www.gnu.org/licenses/>			*/
#pragma once
#include "Cross.h"
#include "Event.h"

namespace cross{

template<class BaseClass>
class Factory;

/*  Single Game class instance created at application launch and stays valid until application is closed.
    Based on it lifetime it is good place to store persistent data and logic here. Update virtual functions
    available for code that needs to be in sync with game loop.
    Game also responsible for Screens management and generic game events like Start()/Stop()/Suspend() etc. */
class Game {
public:
	/* Occurs when someone call SetScreen() */
	Event<Screen*> ScreenChanged;

	Game();
	virtual ~Game();

	/* Called once when core modules initialized. Before first Screen()::Start() function. */
	virtual void Start();
	/* Called once before release all engine resources */
	virtual void Stop();
	/* Called before regular Game::Update() */
	virtual void PreUpdate(float sec) { }
	/* Called every frame update. This Update will be called in any game Screen */
	virtual void Update(float sec);
	/* Called after regular Game::Update() */
	virtual void PostUpdate(float sec) { }
	/* Called when game needs to be paused. For example input call or window lost focus */
	virtual void Suspend();
	/* Called when game needs to be restored from previous interrupt. See Suspend() */
	virtual void Resume();
	/* Will change current Screen at the end of current frame. Old Screen will be deleted */
	virtual void SetScreen(Screen* screen);

	/* Returns time in sec since game start */
	float GetRunTime() const;
	/* Returns active game Screen */
	Screen* GetCurrentScreen();
	/* Returns active game Scene if available */
	Scene* GetCurrentScene();
	/* Returns component factory for custom Components registration */
	Factory<Component>* GetComponentFactory();

	/* Engine specific */
	void EngineUpdate();
	bool IsSuspended() const;

protected:
	Factory<Component>* component_factory	= nullptr;
	Screen* current_screen					= nullptr;
	Screen* next_screen						= nullptr;
	U64 timestamp							= 0;
	U64 run_time							= 0;
	bool suspended							= false;

	void LoadNextScreen();
};

}
