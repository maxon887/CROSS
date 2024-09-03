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
#pragma once
#include "UI/View.h"
#include "Event.h"

class FilesView : public View {
public:
	Event<String> FileSelected;

	FilesView();

	void Shown() override;

	void Update(float sec) override;

private:
	struct Node {
		String path = "";
		String name = "";
		String full_path = "";
		bool initialized = false;
		//first string is filename second full paht + filename
		Array<pair<String, String> > files;
		Array<Node> folders;
	};

	Node file_tree;
	String current_path;

	Array<String> all_shader_files;

	void InitNode(Node& node);
	void Refresh();
	void BuildNote(Node& node);
	void FileDoubleClicked(const String& filename);

	void ContextMenu();
};
