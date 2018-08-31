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
#include "LaunchView.h"
#include "Demo.h"
#include "System.h"
#include "ImGuiScreen.h"
#include "Texture.h"
#include "UI/MenuBar.h"
#include "Scenes/Simple/TriangleScene.h"
#include "Scenes/Simple/TexturedModelScene.h"
#include "Scenes/Light/MaterialScene.h"
#include "Scenes/Light/DirectionalLightScene.h"
#include "Scenes/Light/PointLightScene.h"
#include "Scenes/Light/SpotLightScene.h"
#include "Scenes/Light/MultiLightScene.h"
#include "Scenes/Maps/NakedScene.h"
#include "Scenes/Maps/DiffuseScene.h"
#include "Scenes/Maps/SpecularScene.h"
#include "Scenes/Maps/RoughnessScene.h"
#include "Scenes/Maps/NormalScene.h"
#include "Scenes/Misc/DepthScene.h"
#include "Scenes/Misc/TransparencyScene.h"
#include "Scenes/Misc/SkyboxScene.h"
#include "Scenes/Misc/ApocalypseScene.h"
#include "StringTest.h"
#include "AudioScreen.h"

#include "ThirdParty/ImGui/imgui.h"

LaunchView::LaunchView() : View("Demos")
{ }

void LaunchView::WillContent() {
	ImGui::PushFont(demo->big_font);

	if(!system->IsMobile()) {
		ImGui::SetNextWindowSize(ImVec2(system->GetWindowWidth() / 3.f, system->GetWindowHeight() / 3.f * 2.f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(system->GetWindowWidth() / 2.f, system->GetWindowHeight() / 2.f), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
	} else {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::SetNextWindowSize(ImVec2((float)system->GetWindowWidth(), (float)(system->GetWindowHeight() - demo->GetMenuBar()->GetHeight())));
		ImGui::SetNextWindowPos(ImVec2(0, demo->GetMenuBar()->GetHeight()));
		SetFlags(	ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoTitleBar |
					ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoBringToFrontOnFocus);
	}
}

void LaunchView::DidContent() {
	if(system->IsMobile()) {
		ImGui::PopStyleVar();
	}
	ImGui::PopFont();
}

void LaunchView::Content(float sec) {
	if(ImGui::CollapsingHeader("Scenes", ImGuiTreeNodeFlags_DefaultOpen)) {
		if(ImGui::TreeNode("Simple")) {
			if(ImGui::MenuButton("Triangle")) {
				game->SetScreen(new TriangleScene());
			}
			if(ImGui::MenuButton("Solid Model")) {
				const String filename = "Scenes/Cube.scn";
				CROSS_ASSERT(LoadScene(filename), "Can not load scene(#)", filename);
			}
			if(ImGui::MenuButton("Textured Model")) {
				game->SetScreen(new TexturedModelScene());
			}
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Light")) {
			if(ImGui::MenuButton("Material")) {
				game->SetScreen(new MaterialScene());
			}
			if(ImGui::MenuButton("Directional Light")) {
				game->SetScreen(new DirectionalLightScene());
			}
			if(ImGui::MenuButton("Point Light")) {
				game->SetScreen(new PointLightScene());
			}
			if(ImGui::MenuButton("Spot Light")) {
				game->SetScreen(new SpotLightScene());
			}
			if(ImGui::MenuButton("Multi-Light")) {
				game->SetScreen(new MultiLightScene());
			}
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Maps")) {
			if(ImGui::MenuButton("Naked")) {
				game->SetScreen(new NakedScene());
			}
			if(ImGui::MenuButton("Diffuse")) {
				game->SetScreen(new DiffuseScene());
			}
			if(ImGui::MenuButton("Specular")) {
				game->SetScreen(new SpecularScene());
			}
			if(ImGui::MenuButton("Roughness")) {
				game->SetScreen(new RoughnessScene());
			}
			if(ImGui::MenuButton("Normal")) {
				game->SetScreen(new NormalScene());
			}
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Misc")) {
			if(ImGui::MenuButton("Depth Test")) {
				game->SetScreen(new DepthScene());
			}
			if(ImGui::MenuButton("Transparency")) {
				game->SetScreen(new TransparencyScene());
			}
			if(ImGui::MenuButton("Skybox")) {
				game->SetScreen(new SkyboxScene());
			}
			if(ImGui::MenuButton("Apocalypse Scene")) {
				game->SetScreen(new ApocalypseScene());
			}
			ImGui::TreePop();
		}
	}

	if(ImGui::CollapsingHeader("Tests")) {
		if(ImGui::MenuButton("String Test")) {
			game->SetScreen(new StringTest());
		}
	}

	if(ImGui::Button("Audio", ImVec2(-1, 0))) {
		game->SetScreen(new AudioScreen());
	}
	if(ImGui::Button("GUI", ImVec2(-1, 0))) {
		game->SetScreen(new ImGuiScreen());
	}
}

bool LaunchView::LoadScene(const String& filename) {
	DemoScene* scene = new DemoScene();
	if(scene->Load(filename)) {
		game->SetScreen(scene);
		return true;
	} else {
		delete scene;
		return false;
	}
}
