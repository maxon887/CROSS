/*	Copyright © 2015 Lukyanau Maksim

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
#include "Scene.h"
#include "Input.h"

namespace cross{

class FreeCameraScene : public Scene {
public:
	FreeCameraScene() = default;
	FreeCameraScene(const string& filename);

	virtual void Start() override;
	virtual void Stop() override;
	virtual void Update(float sec) override;

	void LookAtCamera(bool enbled);
	void LookAtCamera(const Vector3D& target);
	bool IsLookAtCamera() const;

	void MoveForward(float sec);
	void MoveBackward(float sec);
	void MoveLeft(float sec);
	void MoveRight(float sec);
	void MoveUp(float sec);
	void MoveDown(float sec);
	void MoveCameraUp(float sec);
	
	void LookRight(float sec);
	void LookLeft(float sec);
	void LookDown(float sec);
	void LookUp(float sec);

protected:
	bool look_at				= true;

	float liner_speed			= 5.f;
	float angular_speed			= 45.f;
	float orbit_distance		= 3.f;

	float lerp_time				= 0.f;
	Vector3D target				= Vector3D::Zero;
	Vector3D destanation		= Vector3D::Zero;
	Quaternion orientation		= Quaternion::Identity;

	void MouseWheelRoll(float delta);
};

}