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
#include "Config.h"
#include "System.h"
#include "File.h"

#include "Libs/TinyXML2/tinyxml2.h"

using namespace cross;
using namespace tinyxml2;

Config::Config() {
	LoadGameConfig();
	LoadUserConfig();
}

Config::~Config() {
	SaveGameConfig();
	SaveUserConfig();
}

void Config::SetString(const String& key, const String& value) {
	user_prefs[key] = value;
}

void Config::SetInt(const String& key, S32 value) {
	SetString(key, String(value));
}

void Config::SetFloat(const String& key, float value) {
	SetString(key, String(value));
}

void Config::SetBool(const String& key, bool value) {
	SetInt(key, value);
}

String Config::GetString(const String& key, const String& def) const {
	String strValue = GetString(key);
	if(strValue.IsEmpty())
		return def;
	return strValue;
}

S32 Config::GetInt(const String& key, S32 def) const {
	String strValue = GetString(key);
	if(strValue.IsEmpty())
		return def;
	return strValue.ToInt();
}

float Config::GetFloat(const String& key, float def) const {
	String strValue = GetString(key);
	if(strValue.IsEmpty())
		return def;
	return strValue.ToFloat();
}

bool Config::GetBool(const String& key, bool def) const {
	return GetInt(key, def) != 0;
}

System::Orientation Config::GetOrientation() const {
	return orientation;
}

void Config::SetOrientation(System::Orientation ori) {
	orientation = ori;
}

bool Config::UseCompressedTextures() const {
	return use_compressed_textures;
}

Texture::Filter Config::GetTextureFilter() const {
	return texture_filter;
}

const String& Config::GetString(const String& key) const {
	auto entry = user_prefs.find(key);
	if(entry != user_prefs.end()) {
		return (*entry).second;
	} else {
		return empty;
	}
}

void Config::LoadGameConfig() {
	File* xmlFile = os->LoadAssetFile("GameConfig.xml");
	CROSS_FAIL(xmlFile, "Can not load GameConfig file");
	XMLDocument doc;
	XMLError error = doc.Parse((const char*)xmlFile->data, (Size)xmlFile->size);
	CROSS_FAIL(error == XML_SUCCESS, "Can not parse shader xml file");
	delete xmlFile;

	XMLElement* root = doc.FirstChildElement("GameConfig");
	CROSS_FAIL(root, "Failed to find GameConfig root element");
	XMLElement* element = root->FirstChildElement("Property");
	while(element) {
		String name = element->Attribute("name");
		String strValue = element->Attribute("value");

		if(name == "Orientation") {
			orientation = (System::Orientation)strValue.ToInt();
		}

		if(name == "UseCompressedTextures") {
			use_compressed_textures = strValue == "true";
		}

		if(name == "TextureFilter") {
			texture_filter = Texture::StringToFilter(strValue);
		}

		element = element->NextSiblingElement("Property");
	}
}

void Config::LoadUserConfig() {
	if(os->IsDataFileExists("UserConfig.xml")) {
		File* xmlFile = os->LoadDataFile("UserConfig.xml");
		CROSS_FAIL(xmlFile, "Can not load UserConfig xml file");

		XMLDocument doc;
		XMLError error = doc.Parse((const char*)xmlFile->data, (Size)xmlFile->size);
		delete xmlFile;
		CROSS_FAIL(error == XML_SUCCESS, "Can not parse shader xml file");

		XMLElement* root;
		XMLElement* element;

		root = doc.FirstChildElement("UserConfig");
		CROSS_FAIL(root, "Failed to fined UserConfig root element");
		element = root->FirstChildElement("Property");
		while(element) {
			String name = element->Attribute("name");
			String value = element->Attribute("value");
			user_prefs[name] = value;
			element = element->NextSiblingElement("Property");
		}
	} else {
		os->LogIt("UserConfig file don't exists if it is not a first launch consider this like a bug");
	}
}

void Config::SaveGameConfig() {
	XMLDocument doc;

	XMLElement* element = doc.NewElement("GameConfig");
	doc.LinkEndChild(element);

	XMLElement* property = doc.NewElement("Property");
	property->SetAttribute("name", "Orientation");
	property->SetAttribute("value", String(orientation));
	element->LinkEndChild(property);

	property = doc.NewElement("Property");
	property->SetAttribute("name", "UseCompressedTextures");
	property->SetAttribute("value", use_compressed_textures ? "true" : "false");
	element->LinkEndChild(property);

	property = doc.NewElement("Property");
	property->SetAttribute("name", "TextureFilter");
	property->SetAttribute("value", Texture::FilterToString(texture_filter));
	element->LinkEndChild(property);

	XMLPrinter printer;

	doc.Accept(&printer);
	File gameConfig;
	gameConfig.name = "GameConfig.xml";
	gameConfig.size = printer.CStrSize();
	gameConfig.data = (Byte*)printer.CStr();
	os->SaveDataFile(&gameConfig);
	gameConfig.data = nullptr;
}

void Config::SaveUserConfig() {
	XMLDocument doc;

	XMLElement* element = doc.NewElement("UserConfig");
	doc.LinkEndChild(element);

	XMLElement* property;
	for(auto pair : user_prefs){
		property = doc.NewElement("Property");
		property->SetAttribute("name", pair.first);
		property->SetAttribute("value", pair.second);
		element->LinkEndChild(property);
	}

	XMLPrinter printer;

	doc.Accept(&printer);
	File userConfig;
	userConfig.name = "UserConfig.xml";
	userConfig.size = printer.CStrSize();
	userConfig.data = (Byte*)printer.CStr();
	os->SaveDataFile(&userConfig);
	userConfig.data = nullptr;
}
