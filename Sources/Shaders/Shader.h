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
#include "Internals/GraphicsGL.h"

namespace cross{

/*	This class needed to link gpu shader input data to engine cpu core. 
	Needed to create any kind of material */
class Shader {
public:
	class Property {
	public:
		enum Type {
			SAMPLER,
			MAT4,
			VEC4,
			VEC3,
			VEC2,
			FLOAT,
			INT,
			COLOR,
			CUBEMAP,
			UNKNOWN,
		};

		Property(const string& name, const string& glName);
		Property(const string& name, const string& glName, Type type);
		Property(const Property& obj);

		void SetValue(S32 v);
		void SetValue(float v);
		void SetValue(const Color& v);
		void SetValue(const Vector2D& v);
		void SetValue(const Vector3D& v);
		void SetValue(const Vector4D& v);
		void SetValue(const Matrix& v);
		void SetValue(Texture* texture);
		void SetValue(Cubemap* cubemap);

		Property* Clone() const;

		GLuint GetID() const;
		Type GetType() const;
		const string& GetName() const;
		const string& GetGLName() const;

	engine:
		GLint glId = -1;
		Type type = UNKNOWN;

		union Value {
			S32 s32;
			float f;
			Color color;
			Vector2D vec2;
			Vector3D vec3;
			Vector4D vec4;
			Matrix mat;
			Texture* texture;
			Cubemap* cubemap;

			Value() { memset(this, 0, sizeof(Value)); }

		} value;

	private:
		string name		= string();
		string glName	= string();
	};

	//general attributes
	GLint aPosition = -1;
	GLint aTexCoords = -1;
	GLint aColor = -1;
	GLint aNormal = -1;
	GLint aTangent = -1;
	GLint aBitangent = -1;
	//general uniforms
	GLint uMVP = -1;
	GLint uModelMatrix = -1;
	GLint uNormalMatrix = -1;
	GLint uCameraPosition = -1;
	GLint uAmbientLight = -1;
	GLint uColor = -1;

	Shader() = default;
	Shader(const string& vertexFile, const string& fragmentFile);
	virtual ~Shader();

	virtual void Use();
	virtual void Load(const string& file);
	virtual void Save(const string& file);
	virtual void Compile();
	virtual void OnDraw() { }

	bool IsCompiled() const;
	string& GetFilename();
	string& GetVertexFilename();
	void SetVertexFilename(const string& filename);
	string& GetFragmentFilename();
	void SetFragmentFilename(const string& filename);

	void AddVersion(const string& version);
	void AddMacro(const string& makro, bool system = false);
	void AddMacro(const string& makro, int value, bool system = false);
	Array<string>& GetMacrosies();
	void ClearMacrosies();

	void AddProperty(const string& name, const string& glName);
	void AddProperty(const string& glName, float defValue);
	void AddProperty(const string& name, const string& glName, float defValue);
	void AddProperty(const string& name, const string& glName, const Color& color);
	void AddProperty(const string& name, const string& glName, const Vector3D& vec);
	void AddProperty(Property* prop);
	Property* GetProperty(const string& name);
	Array<Property*>& GetProperties();
	void ClearProperties();
	bool HaveProperty(const string& name) const;

protected:
	class LightUniforms{
	public:
		GLint position			= -1;
		GLint direction			= -1;
		GLint color				= -1;

		GLint intensity			= -1;
	
		GLint cut_off			= -1;
		GLint outer_cut_off		= -1;
	};

	GLuint program				= 0;
	Array<string> macrosies		= Array<string>();
	Array<string> user_macro	= Array<string>();
	bool compiled				= false;
	//custom uniforms
	Array<Property*> properties	= Array<Property*>();

	GLuint GetProgram() const;

private:
	GLuint vertex_shader		= 0;
	GLuint fragment_shader		= 0;
	string vertex_filename		= "";
	string fragment_filename	= "";
	File* vertex_file			= NULL;
	File* fragment_file			= NULL;
	U32 makro_len				= 0;
	string filename				= "noname";

	GLuint CompileShader(GLuint type, File* file);
	void CompileProgram();
};

}
