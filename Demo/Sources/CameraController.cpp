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
#include "CameraController.h"
#include "Camera.h"
#include "Transform.h"
#include "Game.h"
#include "Scene.h"
#include "Entity.h"

#include "ThirdParty/ImGui/imgui.h"

CameraController::CameraController() : Component("CameraController")
{ }

void CameraController::Initialize(Scene *scene) {
	Component::Initialize(scene);
	input->ActionDown.Connect(this, &CameraController::OnActionDown);
	input->ActionMove.Connect(this, &CameraController::OnActionMove);
	input->ActionUp.Connect(this, &CameraController::OnActionUp);
	input->KeyPressed.Connect(this, &CameraController::OnKeyPressed);
	input->KeyReleased.Connect(this, &CameraController::OnKeyReleased);
	input->Scroll.Connect(this, &CameraController::MouseWheelRoll);
	destination = *GetEntity()->GetTransform();
}

void CameraController::Remove() {
	input->KeyReleased.Disconnect(this, &CameraController::OnKeyReleased);
	input->KeyPressed.Disconnect(this, &CameraController::OnKeyPressed);
	input->ActionDown.Disconnect(this, &CameraController::OnActionDown);
	input->ActionMove.Disconnect(this, &CameraController::OnActionMove);
	input->ActionUp.Disconnect(this, &CameraController::OnActionUp);
	input->Scroll.Disconnect(this, &CameraController::MouseWheelRoll);
	Component::Remove();
}

void CameraController::Update(float sec) {
	Component::Update(sec);
	
	ImGuiIO& io = ImGui::GetIO();
	if(!io.WantCaptureKeyboard) {
		if(input->IsPressed(Key::W)) {
			MoveForward(camera_speed * sec);
		}
		if(input->IsPressed(Key::S)) {
			MoveForward(-camera_speed * sec);
		}
		if(input->IsPressed(Key::D)) {
			MoveRight(camera_speed * sec);
		}
		if(input->IsPressed(Key::A)) {
			MoveRight(-camera_speed * sec);
		}
		if(input->IsPressed(Key::E)) {
			MoveUp(camera_speed * sec);
		}
		if(input->IsPressed(Key::Q)) {
			MoveUp(-camera_speed * sec);
		}
	}
	
	Camera* camera = game->GetCurrentScene()->GetCamera();
	if(lerp_time > 0) {
		Vector3D pos = Lerp(camera->GetPosition(), destination.GetPosition(), 1.f - lerp_time);
		Quaternion rot = Lerp(camera->GetTransform()->GetRotate(), destination.GetRotate(), 1.f - lerp_time);
		camera->SetPosition(pos);
		camera->GetTransform()->SetRotate(rot);
		lerp_time -= sec;
	} else {
		camera->SetPosition(destination.GetPosition());
		camera->GetTransform()->SetRotate(destination.GetRotate());
	}
}

void CameraController::MoveForward(float distance){
	Vector3D path = destination.GetDirection() * distance;
	destination.SetPosition(destination.GetPosition() + path);
}

void CameraController::MoveRight(float distance) {
	Vector3D path = destination.GetRight() * distance;
	destination.SetPosition(destination.GetPosition() + path);
}

void CameraController::MoveUp(float distance) {
	Vector3D path = Vector3D::Up * distance;
	destination.SetPosition(destination.GetPosition() + path);
}

void CameraController::MoveCloser(float ratio) {
	MoveForward(focus_distance * ratio);
	focus_distance -= focus_distance * ratio;
}

void CameraController::LookRight(float degree) {
	destination.SetRotate(Quaternion(Vector3D::Up, degree) * destination.GetRotate());
	if(look_at) {
		Camera* camera = game->GetCurrentScene()->GetCamera();
		Vector3D target = camera->GetPosition() + camera->GetTransform()->GetForward() * focus_distance;
		destination.SetPosition(target + destination.GetForward() * focus_distance * (-1.f));
	}
}

void CameraController::LookUp(float degree) {
	destination.SetRotate(Quaternion(destination.GetRight(), -degree) * destination.GetRotate());
	if(look_at) {
		Camera* camera = game->GetCurrentScene()->GetCamera();
		Vector3D target = camera->GetPosition() + camera->GetTransform()->GetForward() * focus_distance;
		destination.SetPosition(target + destination.GetForward() * focus_distance * (-1.f));
	}
}

void CameraController::LookAtCamera(bool enabled) {
	look_at = enabled;
}

void CameraController::LookAtTarget(const Vector3D& target, float distance /* = 3*/) {
	look_at = true;
	lerp_time = 1.f;
	focus_distance = distance;
	Camera* camera = game->GetCurrentScene()->GetCamera();
	Vector3D camObjVec = target - camera->GetPosition();
	Vector3D offset = camObjVec - camObjVec.GetNormalized() * focus_distance;
	destination.SetPosition(camera->GetPosition() + offset);

	destination.LookAt(target);
}

bool CameraController::IsLookAtCamera() const {
	return look_at;
}

void CameraController::OnActionDown(Input::Action action) {
	if(handled_action == -1 && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
		handled_action = action.id;
		touch_position = action.pos;
	}
}

void CameraController::OnActionMove(Input::Action action) {
	Vector2D delta = touch_position - action.pos;
	touch_position = action.pos;

	if(handled_action == 0) {
		LookAtCamera(true);
		LookRight(delta.x / 10.f);
		LookUp(delta.y / 10.f);
	}
	if(handled_action == 1) {
		LookAtCamera(false);
		LookRight(delta.x / 10.f);
		LookUp(delta.y / 10.f);
	}
	if(handled_action == 2) {
		delta /= 200.f;
		MoveRight(delta.x);
		MoveUp(delta.y);
	}
}

void CameraController::OnActionUp(Input::Action action) {
	if(handled_action == action.id) {
		handled_action = -1;
	}
}

void CameraController::OnKeyPressed(Key key) {
	if(key == Key::ALT) {
		LookAtCamera(false);
	}
}

void CameraController::OnKeyReleased(Key key) {
	if(key == Key::ALT) {
		LookAtCamera(true);
	}
}

void CameraController::MouseWheelRoll(float delta) {
	if(!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
		MoveCloser(0.1f * delta);
	}
}