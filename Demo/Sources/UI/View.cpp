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
#include "View.h"
#include "System.h"
#include "Config.h"

#include "ThirdParty/ImGui/imgui.h"

View::View(const String& name)
{
	landscape_name = name + "##Landscape";
	portrait_name = name + "##Portrait";
	String windowID = String::Format("VIEW_#_VISIBLE", GetName().Hash());
	visible = config->GetBool(windowID, false);
}

View::~View() {
	String windowID = String::Format("VIEW_#_VISIBLE", GetName().Hash());
	config->SetBool(windowID, visible);
}

void View::Show() {
	visible = true;
	Shown();
}

void View::Hide() {
	visible = false;
	Hidden();
}

bool View::IsVisible() const {
	return visible;
}

const String& View::GetName() const {
	return landscape_name;
}

void View::Run(float sec) {
	if(visible) {
		PreUpdate();
		switch(system->GetDeviceOrientation()) {
		case System::Orientation::LANDSCAPE:
			ImGui::Begin(landscape_name, &visible, flags);
			break;
		case System::Orientation::PORTRAIT:
			ImGui::Begin(portrait_name, &visible, flags);
			break;
		default:
			CROSS_FAIL(false, "Unknown device orientation");
			break;
		}

		Update(sec);
		ImGui::End();
		PostUpdate();
	}
}

void View::SetFlags(U32 f) {
	this->flags = f;
}
