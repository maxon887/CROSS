/*	Copyright © 2015 Lukyanau Maksim

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
#include "CameraController.h"
#include "Game.h"
#include "System.h"
#include "Camera.h"
#include "Utils/FreeCameraScene.h"

#include "ThirdParty/ImGui/imgui.h"

#define SCALED(x) x * system->GetScreenScale()

CameraController::CameraController() : View("Camera")
{
	window_width = SCALED(225.f);
	window_height = SCALED(200.f);
}

void CameraController::Shown() {
	FreeCameraScene* scene = dynamic_cast<FreeCameraScene*>(game->GetCurrentScene());
	if(scene) {
		scene->LookAtCamera(false);
	}
}

void CameraController::WillContent() {
	ImGui::SetNextWindowSize(ImVec2(window_width, window_height));
	ImGui::SetNextWindowPos(ImVec2(system->GetWindowWidth() - window_width, system->GetWindowHeight() - window_height));

	SetWindowFlags(ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
}

void CameraController::DidContent() {
	ImGui::PopStyleVar();
}

void CameraController::Content(float sec) {
	FreeCameraScene* scene = dynamic_cast<FreeCameraScene*>(game->GetCurrentScene());
	if(scene) {
		const ImVec2 cursor = ImGui::GetCursorScreenPos();
		float value = 0.f;
		ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, SCALED(40.f));
		if(ImGui::VSliderFloat("", ImVec2(SCALED(35.f), SCALED(190.f)), &value, -1.f, 1.f, "%.2f")) {
			scene->MoveUp(value * sec);
		}

		ImGui::PopStyleVar();

		ImGui::SetCursorPos(ImVec2(SCALED(47.f), SCALED(5.f)));
		bool lookAt = scene->IsLookAtCamera();
		if(ImGui::Checkbox("Look At", &lookAt)) {
			scene->LookAtCamera(lookAt);
		}

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		const ImVec2 w = ImGui::GetWindowSize();
		float radius = SCALED(80.0f);
		Vector2D center(cursor.x - SCALED(16.0f) + w.x - radius, cursor.y - SCALED(16.0f) + w.y - radius);
		const ImU32 col32 = ImColor(1.f, 1.f, 0.33f);
		drawList->AddCircle(center, radius, col32, 30, SCALED(4.f));

		ImGuiIO &io = ImGui::GetIO();
		Vector2D mousePos = io.MousePos;
		Vector2D centerMouse = mousePos - center;

		if(io.MouseReleased[0]) {
			nav_pressed = false;
		}

		if(centerMouse.Length() < radius || nav_pressed) {
			if(centerMouse.Length() > radius) {
				centerMouse = centerMouse.GetNormalized() * radius;
			}
			if(io.MouseDown[0]) {
				nav_pressed = true;
				const ImU32 red32 = ImColor(1.f, 0.f, 0.0f);
				drawList->AddCircle(center + centerMouse, SCALED(6.f), red32, 12, SCALED(4.f));

				centerMouse /= radius;

				scene->MoveForward(-centerMouse.y * sec);
				scene->MoveRight(centerMouse.x * sec);
			}
		}

	}
}

bool CameraController::MobileOnly() {
	return true;
}