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
#include "Component.h"

namespace cross {

/* Base class for cameras. Used for calculation projection and view matrices. */
class Camera : public Component {
public:
	void Update(float sec) override;
	Component* Clone() const override;
	bool Load(tinyxml2::XMLElement* xml, Scene* laodingScene) override;
	bool Save(tinyxml2::XMLElement* xml, tinyxml2::XMLDocument* doc) override;

	float GetViewDistance() const;
	const Matrix& GetViewMatrix() const;
	void SetProjectionMatrix(const Matrix& projection);
	const Matrix& GetProjectionMatrix() const;

protected:
	Matrix view			= Matrix::Identity;
	Matrix projection	= Matrix::Identity;
	float view_distance = 100.f;

	void RecalcView();
};

};