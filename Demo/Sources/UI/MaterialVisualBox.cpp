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
#include "MaterialVisualBox.h"
#include "Material.h"
#include "File.h"
#include "Demo.h"
#include "System.h"
#include "Scene.h"

#include "ThirdParty/ImGui/imgui.h"

MaterialVisualBox::~MaterialVisualBox() {
	DeleteMaterialIfNeeded();
}

void MaterialVisualBox::Update() {
	if(mat) {
		ImGui::PushFont(demo->big_font);
		ImGui::SameLine(ImGui::GetWindowWidth() / 3);
		ImGui::Text("Material");
		ImGui::PopFont();

		ImGui::Text("Shader:");
		ImGui::SameLine(SCALED(70.f));
		String shaderFilename = mat->GetShader()->GetFilename();
		ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "%s", shaderFilename.ToCStr());

		bool trans = mat->IsTransparent();
		if(ImGui::Checkbox("Transparent", &trans)) {
			mat->SetTransparent(trans);
		}

		ImGui::NewLine();
		ImGui::Text("Properties");
		ImGui::Separator();

		for(Shader::Property& prop : mat->GetProperties()) {
			ImGui::TextUnformatted(prop.GetName());
			ImGui::SameLine(ImGui::GetWindowWidth() / 3.f);
			switch(prop.GetType()) {
			case Shader::Property::Type::COLOR: {
				ImGui::ColorEdit4(prop.GetName(), prop.GetValue().color.GetData(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				break;
			}
			case Shader::Property::Type::TEXTURE: {
				String textureFilename = "No Texture selected";
				Texture* texture = prop.GetValue().texture;
				if(texture) {
					textureFilename = texture->GetName();
				}
				
				static bool selected = false;
				if(ImGui::Selectable(textureFilename.ToCStr(), &selected)) {
					selected = false;
					textureFilename = os->OpenFileDialog();
					if(textureFilename) {
						if(game->GetCurrentScene()) {
							prop.GetValue().texture = game->GetCurrentScene()->GetTexture(textureFilename);
						} else {
							delete texture;
							texture = new Texture();
							texture->Load(textureFilename);
							prop.GetValue().texture = texture;
						}
					}
				}

				break;
			}
			default:
				CROSS_ASSERT(false, "Can not display material property of type #", Shader::Property::TypeToString(prop.GetType()));
				break;
			}
		}

		float availableWidth = ImGui::GetWindowWidth();
		ImGui::NewLine();
		ImGui::SameLine(availableWidth / 2);
		if(ImGui::Button("Revert", ImVec2(availableWidth / 4 - SCALED(10.f), 0))) {
			if(loaded_from_scene) {
				mat->Load(mat->GetFilename(), game->GetCurrentScene());
			} else {
				mat->Load(mat->GetFilename(), nullptr);
			}
		}
		ImGui::SameLine(availableWidth / 4 * 3);
		if(ImGui::Button("Save", ImVec2(-1, 0))) {
			mat->Save(os->AssetsPath() + mat->GetFilename());
		}
	}
}

void MaterialVisualBox::OnFileSelected(String filename) {
	DeleteMaterialIfNeeded();
	if(File::ExtensionFromFile(filename) == "mat") {
		Scene* scene = game->GetCurrentScene();
		if(scene) {
			mat = scene->GetMaterial(filename);
			loaded_from_scene = true;
		} else {
			mat = new Material();
			bool success = mat->Load(filename, nullptr);
			loaded_from_scene = false;
			if(!success) {
				delete mat;
				mat = nullptr;
			}
		}
	} else {
		mat = nullptr;
	}
}

void MaterialVisualBox::OnScreenChanged(Screen* newScreen) {
	if(loaded_from_scene) {
		mat = nullptr;
	}
}

void MaterialVisualBox::DeleteMaterialIfNeeded() {
	if(!loaded_from_scene && mat) {
		for(Shader::Property& prop : mat->GetProperties()) {
			if(prop.GetType() == Shader::Property::Type::TEXTURE) {
				delete prop.GetValue().texture;
			}
		}
		delete mat->GetShader();
		delete mat;
	}
}
