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
#include "Screen.h"
#include "Texture.h"
#include "Transform.h"
#include "UI/CameraController.h"
#include "UI/Hierarchy.h"
#include "UI/TransformView.h"
#include "UI/Log.h"
#include "UI/Stats.h"
#include "UI/About.h"
#include "Utils/Debugger.h"
#include "MainScreen.h"

#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/imgui_freetype.h"

MenuBar::MenuBar() {
	CameraController* cameraController = new CameraController();
	Hierarchy* hierarchy = new Hierarchy();
	TransformView* transform = new TransformView();
	hierarchy->EntitySelected.Connect((ComponentView<Transform>*)transform, &ComponentView<Transform>::OnEntitySelected);
	views.push_back(cameraController);
	views.push_back(hierarchy);
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
	if(ImGui::BeginMainMenuBar())
	{
		bool mainScreen = false;
		if(game->GetCurrentScreen()->GetName() == "Main") {
			//ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.0f);
			mainScreen = true;
		}
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.90f, 0.52f, 0.24f, 0.0f));
		if(ImGui::Button("Back")) {
			for(View* v : views) {
				v->Hide();
			}
			game->SetScreen(new MainScreen());
		}
		ImGui::PopStyleColor();
		if(mainScreen) {
			ImGui::PopStyleVar();
			//ImGui::PopItemFlag();
		} else {
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
