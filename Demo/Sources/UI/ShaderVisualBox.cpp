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
#include "ShaderVisualBox.h"
#include "Shaders/Shader.h"
#include "File.h"

#include "ThirdParty/ImGui/imgui.h"

ShaderVisualBox::~ShaderVisualBox() {
	delete shader;
}

void ShaderVisualBox::Update() {
	if(shader) {
		ImGui::Text("Shader file selected");
	}
}

void ShaderVisualBox::OnFileSelected(String filename) {
	if(File::ExtensionFromFile(filename) == "sha") {
		shader = new Shader();
		shader->Load(filename);
	} else {
		delete shader;
		shader = nullptr;
	}
}
