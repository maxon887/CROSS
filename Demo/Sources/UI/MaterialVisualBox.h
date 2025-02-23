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

using namespace cross;

class FileSelector;

class MaterialVisualBox {
public:
	MaterialVisualBox();
	~MaterialVisualBox();

	void Update();

	void OnFileSelected(String filename);
	void OnShaderSelected(String filename);
	void OnScreenChanged(Screen* newScreen);

private:
	Material* mat = nullptr;
	FileSelector* shader_selector = nullptr;
	bool loaded_from_scene = false;
	
	//key property name 
	Dictionary<String, FileSelector*> texture_selectors;

	void DeleteMaterialIfNeeded();
	void CreateTextureSelectors();
};
