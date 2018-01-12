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
#include "Shaders/Shader.h"

namespace cross{

/*	Adjustable property of an object visualization. Shader must be provided in order to use Material. 
	Material life time managed by Scene. Use Scene::GetMaterial() to optain new Material. 
	Material can be used with Mesh class to provide visual representation of the Mesh */
class Material {
public:
	Material() = default;
	Material(Shader* shader);
	~Material();

	/* Sets Shader to Material to be draw with. All previous one properties will be deleted */
	void SetShader(Shader* shader);
	/* Gets current Material's Shader */
	Shader* GetShader();

	/* Returns filename of file from which this Material was loaded */
	const string& GetFilename() const;
	/* Loads Material from file. Expects .mat file extension */
	void Load(const string& filename);
	/* Saves this Material to a material file (.mat extension) */
	void Save(const string& filename);
	/* Resets current material properties to the Shader's default */
	void Reset();

	/* Checks if this Material contains provided property */
	bool HaveProperty(const string& name);
	/* Returns Material property by name or null if can't find*/
	Shader::Property* GetProperty(const string& name);
	/* Returns Material property by internal graphics ID or null if can't find */
	Shader::Property* GetProperty(U64 glID);
	/* Returns all available Material property */
	Array<Shader::Property*>& GetProperties();
	/* Sets integer property value by name */
	void SetPropertyValue(const string& name, S32 value);
	/* Sets float property value by name */
	void SetPropertyValue(const string& name, float value);
	/* Sets Color property value by name */
	void SetPropertyValue(const string& name, const Color& value);
	/* Sets 3d vector property value by name */
	void SetPropertyValue(const string& name, Vector3D& value);
	/* Sets 4d vector property value by name */
	void SetPropertyValue(const string& name, Vector4D& value);
	/* Sets matrix property value by name */
	void SetPropertyValue(const string& name, Matrix& value);
	/* Sets texture property value by name */
	void SetPropertyValue(const string& name, Texture* value);
	/* Sets integer property value by graphics ID */
	void SetPropertyValue(U64 glID, S32 value);
	/* Sets float property value by graphics ID */
	void SetPropertyValue(U64 glID, float value);
	/* Sets Color property value by graphics ID */
	void SetPropertyValue(U64 glID, const Color& value);
	/* Sets 3d vector property value by graphics ID */
	void SetPropertyValue(U64 glID, Vector3D& value);
	/* Sets 4d vector property value by graphics ID */
	void SetPropertyValue(U64 glID, Vector4D& value);
	/* Sets matrix property value by graphics ID */
	void SetPropertyValue(U64 glID, Matrix& value);
	/* Sets texture property value by graphics ID */
	void SetPropertyValue(U64 glID, Texture* value);

	/* Returns true if this Material supports transparency */
	bool IsTransparent();
	/* Enables transparency property for current Material */
	void EnableTransparent(bool yes);

	/* Clone this material with all properties and Shader pointer (won't be copeid) */
	Material* Clone() const;

engineonly:
	U32 active_texture_slot = 0;

private:
	string filename						= "noname";
	Shader* shader						= NULL;
	bool transparent = false;
	Array<Shader::Property*> properties = Array<Shader::Property*>();

	Material(const Material& obj);
	void SetName(const string& filename);
};

}