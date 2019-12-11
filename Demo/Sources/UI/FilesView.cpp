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
#include "FilesView.h"
#include "System.h"
#include "File.h"
#include "Game.h"
#include "Demo.h"
#include "Material.h"
#include "Scenes/DemoScene.h"

#include <algorithm>

#include "ThirdParty/ImGui/imgui.h"

FilesView::FilesView() : View("Files") { }

void FilesView::Shown() {
	if(!file_tree.initialized) {
		String assPath = system->AssetsPath();
		file_tree.path = assPath.SubString(0, assPath.Length() - 1);
		InitNode(file_tree);
	}
}

void FilesView::Update(float sec) {
	BuildNote(file_tree);

	if(ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered()) {
		selected_path = "";
		FileSelected.Emit(selected_path);
	}

	ContextMenu();
}

void FilesView::InitNode(Node& node) {
	String path = node.path + node.name + "/";
	Array<String> folders = system->GetSubDirectories(path);
	sort(folders.begin(), folders.end());
	for(const String& folder : folders) {
		Node newNode;
		newNode.path = path;
		newNode.name = folder;
		node.folders.Add(newNode);
	}
	node.files = system->GetFilesInDirectory(path);
	node.initialized = true;
}

void FilesView::BuildNote(Node& node) {
	static const ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	static const ImGuiTreeNodeFlags leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	for(Node& child : node.folders) {
		String filepath = child.path + child.name;
		ImGuiTreeNodeFlags flags = filepath == selected_path ? node_flags | ImGuiTreeNodeFlags_Selected : node_flags;

		bool open = ImGui::TreeNodeEx(child.name, flags);

		if(ImGui::IsItemClicked()) {
			selected_path = filepath;
		}

		if(open) {
			if(!child.initialized) {
				InitNode(child);
			}
			BuildNote(child);
			ImGui::TreePop();
		}
	}

	for(const String& file : node.files) {
		String filepath = node.path + node.name + "/" + file;
		String filesize = Demo::GetCompactSize(system->GetFileSize(filepath));
		ImGuiTreeNodeFlags flags = filepath == selected_path ? leaf_flags | ImGuiTreeNodeFlags_Selected : leaf_flags;
		ImGui::TreeNodeEx(file, flags);
		if(ImGui::IsItemClicked()) {
			selected_path = filepath;
			filepath.Remove(system->AssetsPath());
			FileSelected.Emit(filepath);
		}
		if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
			FileDoubleClicked(filepath);
		}

		ImVec2 labelSize = ImGui::CalcTextSize(filesize);
		ImGui::SameLine(ImGui::GetWindowWidth() - labelSize.x - SCALED(15.f));
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.f), filesize);
	}
}

void FilesView::FileDoubleClicked(const String& filename) {
	String extencion = File::ExtensionFromFile(filename);
	if(extencion == "scn") {
		Scene* scene = new DemoScene();
		if(!scene->Load(filename)) {
			delete scene;
		} else {
			game->SetScreen(scene);
		}
	} else {
		system->OpenFileExternal(filename);
	}
}

void FilesView::ContextMenu() {
	static bool newFolder = false;
	static bool deleteFile = false;
	if(ImGui::BeginPopupContextWindow("FileOptions")) {
		if(ImGui::MenuItem("New Folder")) {
			newFolder = true;
		}
		//if(ImGui::MenuItem("New Material")) {
		//	Material* mat = new Material();
		//	String filename = selected_path + "/NewMaterial.mat";
		//	mat->Save(filename);
		//}
		if(ImGui::MenuItem("Delete")) {
			deleteFile = true;
		}

		ImGui::EndPopup();
	}

	if(newFolder) {
		ImGui::OpenPopup("Folder Name");
		newFolder = false;
	}
	if(ImGui::BeginPopupModal("Folder Name", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static char buffer[256];
		ImGui::Text("Enter folder name");
		if(!ImGui::IsAnyItemActive()) {
			ImGui::SetKeyboardFocusHere(0);
		}
		ImGui::InputText("##FolderName", buffer, 256);

		if(ImGui::Button("Cancel", ImVec2(120, 0)) ||
			input->IsPressed(Key::ESCAPE)) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if(ImGui::Button("Ok", ImVec2(120, 0)) ||
			input->IsPressed(Key::ENTER)) {

			system->CreateAssetDirectory(buffer);

			file_tree.files.Clear();
			file_tree.folders.Clear();
			InitNode(file_tree);

			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if(deleteFile) {
		ImGui::OpenPopup("Delete?");
		deleteFile = false;
	}
	if(ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Are you shure you what to delete this file?");
		//ImGui::Separator();

		//static int dummy_i = 0;
		//ImGui::Combo("Combo", &dummy_i, "Delete\0Delete harder\0");

		//static bool dont_ask_me_next_time = false;
		//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		//ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
		//ImGui::PopStyleVar();

		if(ImGui::Button("OK", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup(); 
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if(ImGui::Button("Cancel", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}
