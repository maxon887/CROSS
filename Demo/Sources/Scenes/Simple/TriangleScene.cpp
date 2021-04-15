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
#include "TriangleScene.h"
#include "Material.h"
#include "VertexBuffer.h"
#include "Mesh.h"
#include "Entity.h"
#include "Transform.h"

void TriangleScene::Start(){
	DemoScene::Start();

	Material* material = GetMaterial("Materials/SimpleRed.mat");

	VertexBuffer* vertexBuffer = new VertexBuffer();

	Vector3D verticesData[3];
	verticesData[0].x = 0.0f;  verticesData[0].y = 1.f;  verticesData[0].z = 0.0f;
	verticesData[1].x = 1.f;  verticesData[2].y = -1.f;  verticesData[2].z = 0.0f;
	verticesData[2].x = -1.f;  verticesData[1].y = -1.f;  verticesData[1].z = 0.0f;

	vertexBuffer->PushData((Byte*)&verticesData[0], 3 * sizeof(Vector3D));

	Array<GLushort> indices(3, 0);
	indices.Add(0);
	indices.Add(1);
	indices.Add(2);

	Mesh* triangle = new Mesh();
	triangle->PushData(vertexBuffer, indices);
	triangle->InitializeVideoData();
	triangle->SetMaterial(material);
	triangle->EnableFaceCulling(false);
	
	delete vertexBuffer;

	Entity* entity = new Entity("Triangle");
	entity->AddComponent(new Transform());
	entity->AddComponent(triangle);
	AddEntity(entity);
}