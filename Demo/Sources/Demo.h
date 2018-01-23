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
#include "Game.h"
#include "Input.h"

#define SCALED(x) x * system->GetScreenScale()

using namespace cross;

struct ImDrawData;
struct ImFont;

class Demo;
class MenuBar;

extern Demo* demo;

class Demo : public Game {
public:
	ImFont* small_font		= null;
	ImFont* normal_font		= null;
	ImFont* big_font		= null;

	void Start() override;
	void Stop() override;
	void PreUpdate(float sec) override;
	void Update(float sec) override;

	MenuBar* GetMenuBar();

private:
	static const char* GetClipboardString(void* userData);

	MenuBar* menu			= null;
	Shader* ui_shader		= null;
	Texture* font_texture	= null;
	string clipboard		= "";
	U32 vertex_buffer		= 0;
	U32 index_buffer		= 0;

	Vector2D action_pos		= Vector2D(0.f, 0.f);
	Array<bool> actions		= Array<bool>(MAX_ACTIONS, false);
	float mouse_wheel		= 0.0f;

	bool CreateDeviceObjects();
	bool CreateFontsTexture();
	void RenderUI(ImDrawData*);

	void ActionDownHandle(Input::Action action);
	void ActionMoveHandle(Input::Action action);
	void ActionUpHandle(Input::Action action);

	void KeyPressed(Key key);
	void KeyReleased(Key key);
	void CharEnter(char c);

	void WheelRoll(float delta);
};
