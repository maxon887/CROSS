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
	/* Custom user properties */
	void SetString(const string& key, const string& value);
	void SetInt(const string& key, S32 value);
	void SetFloat(const string& key, float value);
	void SetBool(const string& key, bool value);
	string GetString(const string& key, const string& def) const;
	S32 GetInt(const string& key, S32 def) const;
	float GetFloat(const string& key, float def) const;
	bool GetBool(const string& key, bool def) const;
	/* Game config properties */
	System::Orientation GetOrientation() const;
	void SetOrientation(System::Orientation orientation);
	bool UseCompressedTextures() const;
	Texture::Filter GetTextureFilter() const;
	bool IsOffscreenRender() const;

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
