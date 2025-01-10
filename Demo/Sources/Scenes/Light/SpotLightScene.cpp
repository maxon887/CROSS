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
#include "SpotLightScene.h"
#include "Material.h"
#include "Entity.h"
#include "Light.h"
#include "Camera.h"
#include "Mesh.h"
#include "Graphics.h"
	
void SpotLightScene::Start(){
	DemoScene::Start();

	GetCamera()->SetPosition(Vector3D(-8.f, 0.f, -15.f));
	GetCamera()->GetTransform()->LookAt(Vector3D::Zero);

	Entity* light = CREATE Entity("SpotLight");
	light->AddComponent(CREATE Transform());
	Light* lightComponent = CREATE Light(Light::Type::SPOT);
	light->GetTransform()->SetPosition(Vector3D(0.f, 0.f, -8.f));
	light->GetTransform()->LookAt(Vector3D(0.f));
	lightComponent->SetCutOff(20.f);
	light->AddComponent(lightComponent);
	AddEntity(light);

	shader = gfx->LoadShader("Engine/Shaders/SingleLight.sha");
	shader->AddMacro("SPOT_LIGHT");
	shader->AddMacro("POINT_OR_SPOT_LIGHT");
	shader->AddProperty("Diffuse Texture", "uDiffuseTexture");
	shader->AddProperty("Specular Map", "uSpecularMap");
	shader->Compile();
	material = CREATE Material(shader);
	material->SetPropertyValue("Diffuse Texture", GetTexture("Textures/ContainerDiffuse.png"));
	material->SetPropertyValue("Specular Map", GetTexture("Textures/ContainerSpecular.png"));
	material->SetPropertyValue("Shininess", 0.5f * 128.f);
	cube = LoadPrimitive(Model::Primitive::CUBE);
	cube->GetComponent<Mesh>()->SetMaterial(material);
	Entity* cubes = CREATE Entity("Cubes");
	cubes->AddComponent(CREATE Transform());
	for(U32 i = 0; i < 40; ++i){
		Entity* clone = cube->Clone();
		clone->GetTransform()->SetPosition(Vector3D(Random(-5.f, 5.f), Random(-5.f, 5.f), Random(-5.f, 5.f)));
		clone->GetTransform()->SetRotate(Vector3D(Random(-1.f, 1.f), Random(-1.f, 1.f), Random(-1.f, 1.f)), Random(0.f, 360.f));
		cubes->AddChild(clone);
	}
	AddEntity(cubes);
}

void SpotLightScene::Stop(){
	delete material;
	delete shader;
	delete cube;
	DemoScene::Stop();
}