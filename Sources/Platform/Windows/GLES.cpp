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
#ifdef GLES
#include "GLES.h"
#include "Internals/GraphicsGL.h"
#include "Internals/Audio.h"
#include "Native.h"
#include "Game.h"
#include "Screen.h"
#include "WINSystem.h"
#include "Config.h"
#include "Utils/Debugger.h"
#include "Platform/CrossEGL.h"

#include <time.h>

using namespace cross;

CrossEGL* crossEGL = nullptr;

int GLES_Main(){
#ifdef CROSS_MEMORY_PROFILE
	MemoryManager::dead = false;
#endif // CROSS_MEMORY_PROFILE

	crossEGL = CREATE CrossEGL();
	crossEGL->BindWindow(WinCreate());

	srand((U32)time(0));
	WINSystem* winSys = CREATE WINSystem(crossEGL->GetWindow());
	cross::os = winSys;
	game = CrossMain();
	input->KeyReleased.Connect(winSys, &WINSystem::KeyReleasedHandle);

	int winX = config->GetInt("WIN_POS_X", 100);
	int winY = config->GetInt("WIN_POS_Y", 100);
	int winWidth = config->GetInt("WIN_WIDTH", 960);
	int winHeight = config->GetInt("WIN_HEIGHT", 512);
	winSys->ResizeWindow(winX, winY, winWidth, winHeight);

	crossEGL->CreateContext(true);

	ShowWindow(crossEGL->GetWindow(), TRUE);

	audio = CREATE Audio();
	gfxGL = CREATE GraphicsGL();
	game->Start();

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while(msg.message != WM_QUIT) {
		while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if(msg.message == WM_QUIT) break;
		}
		game->EngineUpdate();
		if(!game->IsSuspended()) {
			crossEGL->SwapBuffers();
		}
	}

	game->GetCurrentScreen()->Stop();
	game->Stop();
	delete gfxGL;
	crossEGL->DestroyContext(true);
	delete crossEGL;
	delete audio;
	delete game;
	delete cross::os;
	Debugger::Release();
#ifdef CROSS_MEMORY_PROFILE
	MemoryManager::Instance()->Dump();
	MemoryManager::dead = true;
#endif // CROSS_MEMORY_PROFILE
	return 0;
}

#endif