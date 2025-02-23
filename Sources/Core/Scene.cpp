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
#include "Scene.h"
#include "Camera.h"
#include "Game.h"
#include "Graphics.h"
#include "Light.h"
#include "Entity.h"
#include "Material.h"
#include "Mesh.h"
#include "File.h"
#include "Transform.h"
#include "Factory.h"

#include "Libs/TinyXML2/tinyxml2.h"

using namespace cross;
using namespace tinyxml2;

Scene::Scene()
{
	root = CREATE Entity("Root");
	root->AddComponent(CREATE Transform(), this);
}

void Scene::Start() {
	Screen::Start();

	os->WindowResized.Connect(this, &Scene::OnWindowResize);
}

void Scene::Update(float sec) {
	Screen::Update(sec);
	root->Update(sec);
}

void Scene::Stop() {
	os->WindowResized.Disconnect(this, &Scene::OnWindowResize);
	delete root;
	root = nullptr;
	for(pair<U64, Texture*> pair : textures) {
		delete pair.second;
	}
	for(pair<U64, Material*> pair : materials) {
		delete pair.second;
	}
	for(pair<U64, Model*> pair : models) {
		delete pair.second;
	}
	for(pair<U64, Shader*> pair : shaders) {
		delete pair.second;
	}
	Screen::Stop();
}

bool Scene::Load(const String& file) {
	filename = file;
	File* xmlFile = os->LoadAssetFile(file);
	CROSS_RETURN(xmlFile, false, "Can not load scene xml file");
	XMLDocument doc;
	XMLError error = doc.Parse((const char*)xmlFile->data, (Size)xmlFile->size);
	CROSS_RETURN(error == XML_SUCCESS, false, "Can not parse shader xml file");
	delete xmlFile;

	XMLElement* scene = doc.FirstChildElement("Scene");
	CROSS_RETURN(scene, false, "Can not load scene. Root node Scene not found");
	SetName(File::FileWithoutExtension(File::FileFromPath(file)));
	int version = scene->IntAttribute("version");
	CROSS_ASSERT(version <= scene_loader_version, "Scene loader version mismatch");

	//objects loading
	XMLElement* objectsXML = scene->FirstChildElement("Objects");
	if(objectsXML) {
		XMLElement* objectXML = objectsXML->FirstChildElement("Object");
		while(objectXML) {
			CROSS_RETURN(LoadEntity(root, objectXML), false, "Can't load entity");
			objectXML = objectXML->NextSiblingElement("Object");
		}
	}

	return true;
}

void Scene::Save(const String& filename) {
	XMLDocument doc;

	XMLElement* sceneXML = doc.NewElement("Scene");
	sceneXML->SetAttribute("version", scene_saver_version);
	doc.LinkEndChild(sceneXML);

	if(root->children.size() > 0) {
		XMLElement* objectsXML = doc.NewElement("Objects");
		for(Entity* entity : root->children){
			CROSS_FAIL(SaveEntity(entity, objectsXML, &doc), "Can not save entity");
		}
		sceneXML->LinkEndChild(objectsXML);
	}

	XMLPrinter printer;
	doc.Accept(&printer);
	File saveFile;
	saveFile.name = filename;
	saveFile.size = printer.CStrSize() - 1;//-1 because we don't need to save null-terminated string
	saveFile.data = (Byte*)printer.CStr();
	os->SaveFile(&saveFile);
	saveFile.data = nullptr;
}

void Scene::Clear() {
	root->RemoveChildren();
	lights.clear();
	for(pair<U64, Material*> pair : materials){
		delete pair.second;
	}
	materials.clear();
	for(pair<U64, Texture*> pair : textures) {
		delete pair.second;
	}
	textures.clear();
}

Entity* Scene::GetRoot() {
	return root;
}

Entity* Scene::GetEntity(const String& name) {
	Entity* child = root->FindChild(name);
	if(child){
		return child;
	}
	CROSS_RETURN(false, nullptr, "Can not find entity #", name);
}

void Scene::AddEntity(Entity* entity) {
	entity->Initialize();
	root->AddChild(entity);
	EntityAdded.Emit(entity);//trigger
}

Entity* Scene::LoadPrimitive(Model::Primitive primitive) {
	switch(primitive) {
	case Model::CUBE:
		return GetModel("Engine/Models/Cube.obj")->GetHierarchy();
	case Model::SPHERE:
		return GetModel("Engine/Models/Sphere.obj")->GetHierarchy();
	case Model::PLANE:
		return GetModel("Engine/Models/Plane.obj")->GetHierarchy();
	default:
		CROSS_RETURN(false, nullptr, "Unknown primitive type");
	}
}

Entity* Scene::RemoveEntity(const String& name) {
	return root->RemoveChild(name);
}

List<Light*>& Scene::GetLights() {
	return lights;
}

Camera* Scene::GetCamera() {
	return camera;
}

String Scene::GetFilename() const {
	return filename;
}

void Scene::SetCamera(Camera* cam) {
	this->camera = cam;
}

Color Scene::GetAmbientColor() const {
	return ambient_color;
}

Shader* Scene::GetShader(const String& shaderFile) {
	U64 hash = shaderFile.Hash();
	auto shaderIt = shaders.find(hash);
	if(shaderIt != shaders.end()) {
		return (*shaderIt).second;
	} else {
		Shader* shader = gfx->LoadShader(shaderFile);
		shader->Compile();
		shaders[hash] = shader;
		return shader;
	}
}

Material* Scene::GetMaterial(const String& xmlFile) {
	U64 hash = xmlFile.Hash();
	auto matIt = materials.find(hash);
	if(matIt != materials.end()) {
		return (*matIt).second;
	} else {
		Material* mat = CREATE Material();
		bool success = mat->Load(xmlFile, this);
		if(success) {
			materials[hash] = mat;
			return mat;
		} else {
			delete mat;
			return nullptr;
		}
	}
}

Material* Scene::GetDefaultMaterial() {
	return GetMaterial("Engine/Default.mat");
}

Texture* Scene::GetTexture(const String& textureFile) {
	U64 hash = textureFile.Hash();
	auto textureIt = textures.find(hash);
	if(textureIt != textures.end()) {
		return (*textureIt).second;
	} else {
		Texture* texture = CREATE Texture();
		texture->Load(textureFile);
		textures[hash] = texture;
		return texture;
	}
}

Texture* Scene::GetTexture(const String& textureFile, Texture::Filter filter) {
	U64 hash = textureFile.Hash();
	auto textureIt = textures.find(hash);
	CROSS_RETURN(textureIt == textures.end(), nullptr, "Texture already loaded. Can't load it second time");

	Texture* texture = CREATE Texture();
	texture->Load(textureFile, filter);
	textures[hash] = texture;
	return texture;
}

Model* Scene::GetModel(const String& modelFile, bool calcTangents /* = false*/) {
	U64 hash = modelFile.Hash();
	auto modelIt = models.find(hash);
	if(modelIt != models.end()) {
		return (*modelIt).second;
	} else {
		Model* model = CREATE Model();
		bool success = model->Load(modelFile, calcTangents);
		if(success) {
			models[hash] = model;
			return model;
		} else {
			delete model;
			return nullptr;
		}
	}
}

void Scene::ResetMaterials() {
	for(pair<U64, Material*> pair : materials) {
		pair.second->Reset();
	}
}

void Scene::SetAmbientColor(const Color& color) {
	this->ambient_color = color;
}

bool Scene::LoadEntity(Entity* parent, XMLElement* objectXML) {
	const char* name = objectXML->Attribute("name");
	CROSS_RETURN(name, false, "Attribute 'name' not contain in Entity node");
	Entity* entity = CREATE Entity(name);
	parent->AddChild(entity);

	XMLElement* componentsXML = objectXML->FirstChildElement("Components");
	Factory<Component>* factory = game->GetComponentFactory();
	if(componentsXML) {
		XMLElement* componentXML = componentsXML->FirstChildElement();
		while(componentXML) {
			Component* component = factory->Create(componentXML->Name());
			CROSS_RETURN(component, false, "Can't create component of type #", componentXML->Name());
			CROSS_RETURN(component->Load(componentXML), false, "Can't load component of type '#'", componentXML->Name());
			entity->AddComponent(component, this);
			componentXML = componentXML->NextSiblingElement();
		}
	}

	XMLElement* childrenXML = objectXML->FirstChildElement("Children");
	if(childrenXML) {
		XMLElement* childXML = childrenXML->FirstChildElement("Object");
		while(childXML) {
			CROSS_RETURN(LoadEntity(entity, childXML), false, "Can't load child of entity '#'", parent->GetName());
			childXML = childXML->NextSiblingElement("Object");
		}
	}

	return true;
}

bool Scene::SaveEntity(Entity* entity, XMLElement* parent, XMLDocument* doc) {
	XMLElement* objectXML = doc->NewElement("Object");
	objectXML->SetAttribute("name", entity->GetName());

	const Array<Component*>& components = entity->GetComponents();
	if(components.Size() > 0) {
		XMLElement* componentsXML = doc->NewElement("Components");
		for(Component* component : components) {
			CROSS_RETURN(component->Save(componentsXML, doc), false, "Can't save entity component");
		}
		objectXML->LinkEndChild(componentsXML);
	}

	if(entity->children.size() > 0) {
		XMLElement* childrenXML = doc->NewElement("Children");
		for(Entity* child : entity->children) {
			SaveEntity(child, childrenXML, doc);
		}
		objectXML->LinkEndChild(childrenXML);
	}
	parent->LinkEndChild(objectXML);
	return true;
}

void Scene::OnWindowResize(S32 width, S32 height){
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, os->GetAspectRatio(), 0.1f, camera->GetViewDistance());
	camera->SetProjectionMatrix(projection);
}

void Scene::CreateDefaultCamera() {
	Entity* camEntity = CREATE Entity("Camera");
	Transform* transComp = CREATE Transform(Vector3D(0.f, 0.f, -2.f));
	transComp->SetDirection(Vector3D(0.f, 0.f, 1.f));
	Camera* camComp = CREATE Camera();
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, os->GetAspectRatio(), 0.1f, 100.f);
	camComp->SetProjectionMatrix(projection);
	camEntity->AddComponent(transComp);
	camEntity->AddComponent(camComp);
	AddEntity(camEntity);
	camera = camComp;
}
