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
#include "Cross.h"

struct aiMesh;
struct aiNode;
struct aiScene;

namespace cross{

class Model {
public:
	enum Primitive {
		CUBE,
		SPHERE,
		PLANE,
		COUNT
	};

	~Model();

	void Load(const string& filename, bool initialize = true);

	const string& GetFilename() const;
	Entity* GetHierarchy() const;
	Mesh* GetMesh(S32 id);

private:
	string filename;
	Dictionary<S32, Mesh*> meshes;
	Entity* hierarchy;

	const aiScene* current_scene = NULL;
	bool initialize_in_load = true;
	S32 mesh_id = 0;

	void ProcessScene(Entity* root, File* sceneFile);
	void ProcessNode(Entity* entity, aiNode* node);
	Mesh* ProcessMesh(aiMesh* mesh);
};

}