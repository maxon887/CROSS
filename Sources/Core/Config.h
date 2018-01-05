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
#include "Texture.h"
#include "System.h"

namespace cross {

/*	Configuration manager. This class provide interface 
	for saving and loading data in non-volatile device memory. */
class Config {
public:
	Config();
	~Config();

	/* Save custom user property as string */
	void SetString(const string& key, const string& value);
	/* Save custom user property as integer value */
	void SetInt(const string& key, S32 value);
	/* Save custom user property as float value */
	void SetFloat(const string& key, float value);
	/* Save custom user property as boolean value */
	void SetBool(const string& key, bool value);
	/* Gets custom user property as string */
	string GetString(const string& key, const string& def) const;
	/* Gets custom user property as integer */
	S32 GetInt(const string& key, S32 def) const;
	/* Gets custom user property as float */
	float GetFloat(const string& key, float def) const;
	/* Gets custom user property as bool */
	bool GetBool(const string& key, bool def) const;

	/* Gets application orientation */
	System::Orientation GetOrientation() const;
	/* Sets application orientation */
	void SetOrientation(System::Orientation orientation);
	/* Returns true if engine suppose to use compressed textures */
	bool UseCompressedTextures() const;
	/* Returns texture filter that should be applied by default */
	Texture::Filter GetTextureFilter() const;
	/* Returns true if offsecreen render enabled */
	bool IsOffscreenRender() const;

	/* Engine specific*/
	void SaveGameConfig();
	void SaveUserConfig();
	void LoadGameConfig();
	void LoadUserConfig();

protected:
	System::Orientation orientation		= System::AUTO;
	bool use_compressed_textures		= false;
	Texture::Filter texture_filter		= Texture::Filter::NEAREST;
	bool offscreen_render				= false;

	Dictionary<string, string> user_prefs = Dictionary<string, string>();

	const string& GetString(const string& key) const;

private:
	const string empty = "";
};
	
}
