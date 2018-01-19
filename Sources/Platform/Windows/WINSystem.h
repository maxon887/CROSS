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
#include "System.h"
#include "Input.h"

#include <Windows.h>

namespace cross{

class WINSystem : public System {
public:
	WINSystem(HWND wnd = 0);
	~WINSystem() { }

	void Log(const char* msg) override;
	string AssetsPath() override;
	string DataPath() override;
	U64 GetTime() override;
	float GetScreenDPI() override;
	string GetClipboard() override;

	void Messagebox(const string& title, const string& msg) override;
	void Sleep(float milis) override;
	bool IsMobile() override;

	void SetAssetPath(const string& path);
	void FullScreen(bool yes);
	void ResizeWindow(int posX, int posY, int width, int height);

public:
	void SetWND(HWND wnd);
	HWND GetHWND();
	void SetWindowPosition(int x, int y);

	void KeyReleasedHandle(Key key);
	
private:
	HWND wnd			= 0;
	string assets_path	= "";
	string clipboard	= "";
	int window_pos_x	= 0;
	int window_pos_y	= 0;
	bool fullscreen		= false;

	bool EnterFullscreen(HWND hwnd, int fullscreenWidth, int fullscreenHeight, int colourBits, int refreshRate);
	bool ExitFullscreen(HWND hwnd, int windowX, int windowY, int windowedWidth, int windowedHeight, int windowedPaddingX, int windowedPaddingY);
};

}