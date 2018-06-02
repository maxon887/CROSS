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
#include "MenuBar.h"
#include "System.h"
#include "Demo.h"
#include "Transform.h"
#include "UI/CameraController.h"
#include "UI/SceneView.h"
#include "UI/FilesView.h"
#include "UI/ComponentsView.h"
#include "UI/TransformView.h"
#include "UI/Log.h"
#include "UI/Stats.h"
#include "UI/About.h"
#include "MainScreen.h"
#include "Scene.h"

#include "ThirdParty/ImGui/imgui.h"

MenuBar::MenuBar() {
	CameraController* cameraController = new CameraController();
	FilesView* filesView = new FilesView();
	SceneView* sceneView = new SceneView();
	ComponentsView* componentsView = new ComponentsView(sceneView);
	TransformView* transform = new TransformView();
	sceneView->EntitySelected.Connect((ComponentView<Transform>*)transform, &ComponentView<Transform>::OnEntitySelected);
	views.push_back(filesView);
	views.push_back(sceneView);
	views.push_back(componentsView);
	views.push_back(cameraController);
	views.push_back(transform);

	log = new Log();
	stats = new Stats();
	about = new About();
}

MenuBar::~MenuBar() {
	for(View* v : views) {
		delete v;
	}
	views.clear();

	delete about;
	delete stats;
	delete log;
}

void MenuBar::Update(float sec) {
	for(View* v : views) {
		v->Update(sec);
	}
	if(show_style_editor) {
		ImGui::Begin("Style Editor", &show_style_editor, ImVec2(ImGui::GetWindowWidth() / 2.f, ImGui::GetWindowHeight() / 2.f));
		ImGui::ShowStyleEditor();
		ImGui::End();
	}
	
	log->Update(sec);
	stats->Update(sec);
	about->Update(sec);
}

void MenuBar::ShowMenu() {
	if(system->IsMobile()) {
		ImGui::PushFont(demo->big_font);
	}

	if(ImGui::BeginMainMenuBar()) {
		bool mainScreen = false;
		if(game->GetCurrentScreen()->GetName() == "Main") {
			mainScreen = true;
		}

		if(ImGui::BeginMenu("File")) {
			if(ImGui::MenuItem("Open Scene")) {
				system->OpenFileDialog();
			}

			if(ImGui::MenuItem("Save Scene", 0, false, game->GetCurrentScene() != nullptr)) {
				String filename = game->GetCurrentScene()->GetFilename();
				if(filename == "") {
					filename = system->OpenFileDialog(true);
				}
				game->GetCurrentScene()->Save(filename);
			}

			if(ImGui::MenuItem("Close Scene", 0, false, game->GetCurrentScreen()->GetName() != "Main")) {
				game->SetScreen(new MainScreen());
			}


			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("View")) {
			for(View* v : views) {
				if(system->IsMobile() || !v->MobileOnly()) {
					bool selected = v->IsVisible();
					if(ImGui::MenuItem(v->GetName(), "", &selected)) {
						if(selected) {
							v->Show();
						} else {
							v->Hide();
						}
					}
				}
			}
			ImGui::EndMenu();
		}
		
		ImVec2 helpSize = ImGui::CalcTextSize("Help");
		ImGui::SameLine(ImGui::GetWindowWidth() - helpSize.x * 2.f);
		if(ImGui::BeginMenu("Help")) {
			if(ImGui::MenuItem("Log")) {
				log->Show();
			}
			if(ImGui::MenuItem("Stats")) {
				stats->Show();
			}
			if(ImGui::MenuItem("Style Editor")) {
				show_style_editor = true;
			}
			if(ImGui::MenuItem("About")) {
				about->Show();
			}
			ImGui::EndMenu();
		}
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec2 size = ImGui::GetItemRectSize();
		menu_height = size.y +style.FramePadding.y * 2.f - style.ItemSpacing.y * 2.f;// +style.ItemSpacing.y;
		ImGui::EndMainMenuBar();
	}

	if(system->IsMobile()) {
		ImGui::PopFont();
	}
}

float MenuBar::GetHeight() const {
	return menu_height;
}
