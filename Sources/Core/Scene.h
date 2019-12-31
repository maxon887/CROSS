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
#include "Screen.h"
#include "Input.h"
#include "Model.h"
#include "Texture.h"

namespace tinyxml2{

class XMLElement;
class XMLDocument;

}

namespace cross{

/*	Scene is place where all Entities live. Scene manages Entity live time, update cycles, utilization.
	Second main task of this class is resource management.Textures, Models, Shaders, Materials also stored there.
	You can obtain required resource by calling Get[ResourceName]() function.
	You don't need to call Entity's or resource destructors manually */
class Scene : public Screen {
public:
	/* Occurs when new Entity added into the Scene */
	Event<Entity*> EntityAdded;

	Scene();

	/* Called once before scene show up. */
	virtual void Start() override;
	/* Called once when scene about to change on new one */
	virtual void Stop() override;
	/* Called every frame update. */
	virtual void Update(float sec) override;

	/* Loads scene from file(.scn). Returns true if succeed */
	virtual bool Load(const String& file);
	/* Save scene into file */
	virtual void Save(const String& file);

	/* Removes all loaded stuff from scene (Textures, Materials, Shader, Models and Entities) */
	void Clear();
	/* Returns root Scene Entity */
	Entity* GetRoot();
	/* Returns scene main 3D camera */
	Camera* GetCamera();
	/* Returns name of scene file if exists */
	String GetFilename() const;
	/* Sets main 3D camera for this Scene */
	void SetCamera(Camera* cam);
	/* Finds specific entity on this Scene by name */
	Entity* GetEntity(const String& name);
	/* Adds an entity into the Scene */
	void AddEntity(Entity* entity);
	/* Loads primitive model as Entity with Mesh Component included */
	Entity* LoadPrimitive(Model::Primitive primitive);
	/* Removes entity from Scene by name. Returns founded Entity or nullptr otherwise. Entity won't be destructed */
	Entity* RemoveEntity(const String& name);
	/* Returns all available Lights on the Scene. */
	List<Light*>& GetLights();
	/* Sets up ambient Scene Light Color */
	void SetAmbientColor(const Color& color);
	/* Returns ambient Scene Light Color */
	Color GetAmbientColor() const;
	/* Obtain loaded into scene Shader or load it by self in other way */
	Shader* GetShader(const String& shaderfile);
	/* Obtain loaded into scene Material or load it by self in other way */
	Material* GetMaterial(const String& xmlFile);
	/* Returns default engine material */
	Material* GetDefaultMaterial();
	/* Obtain loaded into scene Texture or load it by self in other way */
	Texture* GetTexture(const String& textureFile);
	/* Loads texture with provided filter */
	Texture* GetTexture(const String& textureFile, Texture::Filter filter);
	/* Obtain loaded into scene Model or load it by self in other way */
	Model* GetModel(const String& modelFile, bool calcTangents = false);
	/* Resets all materials to default state */
	void ResetMaterials();
	/* Resets all shaders to default state */
	void ResetShaders();

protected:
	Entity* root							= nullptr;
	String filename							= "";

	Dictionary<U64, Shader*> shaders		= Dictionary<U64, Shader*>();
	Dictionary<U64, Texture*> textures		= Dictionary<U64, Texture*>();
	Dictionary<U64, Material*> materials	= Dictionary<U64, Material*>();
	Dictionary<U64, Model*> models			= Dictionary<U64, Model*>();
	List<Light*> lights						= List<Light*>();

	Camera* camera							= nullptr;
	Color ambient_color						= Color(0.1f, 0.1f, 0.1f);

	void CreateDefaultCamera();

private:
	static const U32 scene_loader_version	= 18;
	static const U32 scene_saver_version	= 18;

	bool LoadEntity(Entity* parent, tinyxml2::XMLElement* xml);
	bool SaveEntity(Entity* e, tinyxml2::XMLElement* parent, tinyxml2::XMLDocument* doc);

	void OnWindowResize(S32 width, S32 height);
};

}
