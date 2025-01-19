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
#include "SceneView.h"
#include "Entity.h"
#include "Demo.h"
#include "Scene.h"
#include "UI/MenuBar.h"
#include "System.h"
#include "Camera.h"
#include "Transform.h"
#include "Scenes/DemoScene.h"
#include "CameraController.h"

#include "ThirdParty/ImGui/imgui.h"

SceneView::SceneView() : View("Scene", true)
{
	game->ScreenChanged.Connect(this, &SceneView::OnSceneChanged);
}

void SceneView::PreUpdate() {
	if(os->IsMobile()) {
		if(os->GetDeviceOrientation() == System::Orientation::LANDSCAPE) {
			ImGui::SetNextWindowSize(ImVec2((float)os->GetWindowWidth() / 3.f,
				(float)(os->GetWindowHeight() - demo->GetMenuBar()->GetHeight())),
				ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowPos(ImVec2(0, demo->GetMenuBar()->GetHeight()), ImGuiCond_FirstUseEver);
		} else {
			ImGui::PushFont(demo->big_font);
			ImGui::SetNextWindowSize(ImVec2((float)os->GetWindowWidth(), (float)(os->GetWindowHeight() - demo->GetMenuBar()->GetHeight())));
			ImGui::SetNextWindowPos(ImVec2(0, demo->GetMenuBar()->GetHeight()));
		}
	}
}

void SceneView::Update(float sec) {
	DemoScene* scene = static_cast<DemoScene*>(demo->GetCurrentScene());

	if(game->GetCurrentScene()) {
		for(Entity* child : game->GetCurrentScene()->GetRoot()->GetChildren()) {
			if(child != scene->service_root) {
				BuildNode(child);
			}
		}
	} else {
		selected_entity = nullptr;
	}

	if(ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered()) {
		selected_entity = nullptr;
		editing = false;
		EntitySelected.Emit(selected_entity);
	}

	ContextMenu();

	if(selected_entity && selected_entity->HasComponent<Transform>() && !selected_entity->HasComponent<Camera>()) {
		Transform* transform = selected_entity->GetComponent<Transform>();
		scene->DrawVector(transform->GetWorldDirection(), transform->GetWorldPosition());
	}
}

void SceneView::PostUpdate() {
	if(os->IsMobile()) {
		if(os->GetDeviceOrientation() != System::Orientation::LANDSCAPE) {
			ImGui::PopFont();
		}
	}
}

Entity* SceneView::GetSelectedEntity() {
	return selected_entity;
}

void SceneView::OnSceneChanged(Screen*) {
	selected_entity = nullptr;
}

void SceneView::LookAtObject() {
	Camera* camera = game->GetCurrentScene()->GetCamera();
	CameraController* cameraController = camera->GetEntity()->GetComponent<CameraController>();
	if(cameraController && !selected_entity->HasComponent<Camera>() && selected_entity->HasComponent<Transform>()) {
		Transform* transform = selected_entity->GetComponent<Transform>();
		cameraController->LookAtTarget(transform->GetWorldPosition());
	}
}

void SceneView::BuildNode(Entity* entity) {

	if(os->IsMobile() && os->GetDeviceOrientation() == System::Orientation::LANDSCAPE) {
		ImGui::PushFont(demo->normal_font);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, SCALED(10.f));
	}

	static const ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	static const ImGuiTreeNodeFlags leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	ImGuiTreeNodeFlags selected = entity == selected_entity ? ImGuiTreeNodeFlags_Selected : 0;

	ImGuiTreeNodeFlags flags = 0;
	bool isLeaf = entity->GetChildren().size() == 0;
	if(!isLeaf) {
		flags = node_flags | selected;
	} else {
		flags = leaf_flags | selected;
	}

	bool open = false;
	if(editing && entity == selected_entity) {
		ImVec2 cursorPos = ImGui::GetCursorPos();
		cursorPos.x += ImGui::GetStyle().IndentSpacing;
		ImGui::SetCursorPos(cursorPos);

		char buffer[256];
		strcpy(buffer, entity->GetName().ToCStr());
		String label = "##" + entity->GetName();
		ImGui::InputText(label.ToCStr(), buffer, 256);

		if(ImGui::IsKeyPressed(ImGuiKey_Escape)) {
			editing = false;
		}

		if(ImGui::IsKeyPressed(ImGuiKey_Enter)) {
			entity->SetName(buffer);
			editing = false;
		}

		if(!ImGui::IsItemActive() && !clicked) {
			editing = false;
		}

		if(clicked) {
			ImGui::SetKeyboardFocusHere();
			clicked = false;
		}

	} else {
		open = ImGui::TreeNodeEx(entity->GetName(), flags);
		if(ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
			ImGui::SetDragDropPayload("SceneViewDRAG", &entity, sizeof(Entity*));
			ImGui::TextUnformatted(entity->GetName());
			ImGui::EndDragDropSource();
		}
		if(ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SceneViewDRAG")) {
				Entity* draggedEntity = nullptr;
				memcpy(&draggedEntity, payload->Data, sizeof(Entity*));
				Entity* parent = draggedEntity->GetParent();
				parent->RemoveChild(draggedEntity);
				entity->AddChild(draggedEntity);
			}
			ImGui::EndDragDropTarget();
		}
	}

	if((ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1)) && ImGui::IsItemHovered()) {
		clicked = true;
		if(selected_entity != entity) {
			editing = false;
		}
		selected_entity = entity;
		editing = false;
		EntitySelected.Emit(entity);
	}
	if(ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered()) {
		LookAtObject();
	}

	if(open && !isLeaf) {
		for(Entity* child : entity->GetChildren()) {
			BuildNode(child);
		}
		ImGui::TreePop();
	}

	if(os->IsMobile() && os->GetDeviceOrientation() == System::Orientation::LANDSCAPE) {
		ImGui::PopFont();
		ImGui::PopStyleVar(2);
	}
}

void SceneView::ContextMenu() {
	if(ImGui::BeginPopupContextWindow("SceneOptions")) {
		bool haveScene = game->GetCurrentScene() != nullptr;
		if(ImGui::MenuItem("New Entity", nullptr, false, haveScene)) {
			Entity* newEntity = CREATE Entity("NewEntity");
			game->GetCurrentScene()->AddEntity(newEntity);
		}
		bool entitySelected = haveScene && selected_entity != nullptr;
		if(ImGui::MenuItem("Delete Entity", nullptr, false, entitySelected)) {
			Entity* parent = selected_entity->GetParent();
			parent->RemoveChild(selected_entity);
			delete selected_entity;
			selected_entity = nullptr;
		}
		if(ImGui::MenuItem("Rename")) {
			editing = true;
		}
		ImGui::Separator();
		if(ImGui::MenuItem("Import Model", nullptr, false, haveScene)) {
			String filename = os->OpenFileDialog();
			if(!filename.IsEmpty()) {
				Model* model = game->GetCurrentScene()->GetModel(filename);
				if(model) {
					Entity* entity = model->GetHierarchy();
					DemoScene* demoScene = dynamic_cast<DemoScene*>(game->GetCurrentScene());
					demoScene->ApplyMaterial(entity, demoScene->GetDefaultMaterial());
					demoScene->AddEntity(entity);
				}
			}
		}

		ImGui::EndPopup();
	}
}
