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
#include "MaterialScene.h"
#include "Material.h"
#include "Entity.h"
#include "Light.h"
#include "Mesh.h"
#include "Transform.h"

void MaterialScene::Start() {
	DemoScene::Start();
	//light setups
	Entity* light = CREATE Entity("PointLight");
	light->AddComponent(CREATE Transform(Vector3D(10.f, 7.f, -5.f)));
	light->AddComponent(CREATE Light(Light::Type::POINT));
	AddEntity(light);

	Shader* shader = GetShader("Shaders/Specular.sha");

	material = CREATE Material(shader);
	material->SetPropertyValue("Diffuse Color", Color::Red);
	material->SetPropertyValue("Specular Color", Color::White);
	material->SetPropertyValue("Shininess", 0.5f * 128.f);
	Entity* cube = LoadPrimitive(Model::Primitive::CUBE);
	cube->GetComponent<Mesh>()->SetMaterial(material);
	AddEntity(cube);

	cube->GetTransform()->SetScale(Vector3D(0.73f, 1.0f, 0.55f));
}

void MaterialScene::Stop() {
	delete material;
	DemoScene::Stop();
}
