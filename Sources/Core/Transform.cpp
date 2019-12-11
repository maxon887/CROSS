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
#include "Transform.h"
#include "Entity.h"

#include "Libs/TinyXML2/tinyxml2.h"

using namespace cross;
using namespace tinyxml2;

Transform::Transform() : Component("Transform")
{ }

Transform::Transform(const Vector3D& position) : Component("Transform") {
	SetPosition(position);
}

Component* Transform::Clone() const {
	Transform* result = new Transform();
	result->position = this->position;
	result->scale = this->scale;
	result->rotation = this->rotation;
	result->recalc_model = true;
	return result;
}

Vector3D Transform::GetPosition() const {
	return position;
}

Vector3D Transform::GetWorldPosition() {
	Matrix world = GetWorldMatrix();
	Vector3D pos;
	pos.x = world.m[0][3];
	pos.y = world.m[1][3];
	pos.z = world.m[2][3];
	return pos;
}

void Transform::SetPosition(const Vector3D& pos) {
	position = pos;
	recalc_model = true;
}

void Transform::SetPosition(const Matrix& pos) {
	position.value.x = pos.m[0][3];
	position.value.y = pos.m[1][3];
	position.value.z = pos.m[2][3];
	recalc_model = true;
}

Vector3D Transform::GetScale() const {
	return scale;
}

void Transform::SetScale(float factor) {
	scale = Vector3D(factor);
	recalc_model = true;
}

void Transform::SetScale(const Vector2D& scaleVec) {
	scale = scaleVec;
	recalc_model = true;
}

void Transform::SetScale(const Vector3D& scaleVec) {
	scale = scaleVec;
	recalc_model = true;
}

void Transform::SetScale(const Matrix& scaleMat) {
	scale.value.x = scaleMat.m[0][0];
	scale.value.y = scaleMat.m[1][1];
	scale.value.z = scaleMat.m[2][2];
	recalc_model = true;
}

Quaternion Transform::GetRotate() const {
	return rotation;
}

void Transform::SetRotate(const Vector3D& axis, float angle) {
	Quaternion quat(axis, angle);
	SetRotate(quat);
}

void Transform::SetRotate(const Quaternion& quat) {
	rotation = quat.GetMatrix();
	recalc_model = true;
}

void Transform::SetRotate(const Matrix& rot) {
	rotation = rot;
	recalc_model = true;
}

void Transform::LookAt(const Vector3D& object){
	Vector3D forward = object - GetPosition();
	forward.Normaize();
	Vector3D right = Vector3D::Cross(Vector3D::Up, forward).GetNormalized();
	Vector3D up =  Vector3D::Cross(forward, right).GetNormalized();

	Matrix rot = Matrix::Identity;

	rot.m[0][0] = right.x;
	rot.m[1][0] = right.y;
	rot.m[2][0] = right.z;

	rot.m[0][1] = up.x;
	rot.m[1][1] = up.y;
	rot.m[2][1] = up.z;

	rot.m[0][2] = forward.x;
	rot.m[1][2] = forward.y;
	rot.m[2][2] = forward.z;

	rotation = rot;
	
	recalc_model = true;
}

Vector3D Transform::GetDirection() const {
	return GetForward();
}

Vector3D Transform::GetWorldDirection() {
	Matrix world = GetWorldMatrix();
	return world.GetRotation() * Vector3D::Forward;
}

Vector3D Transform::GetForward() const {
	return rotation.value * Vector3D::Forward;
}

Vector3D Transform::GetRight() const {
	return rotation.value * Vector3D::Right;
}

Vector3D Transform::GetUp() const {
	return rotation.value * Vector3D::Up;
}

void Transform::SetDirection(const Vector3D& direction) {
	Vector3D lookAt = this->GetPosition() + direction;
	LookAt(lookAt);
}

Matrix& Transform::GetModelMatrix() {
	if(recalc_model) {
		Matrix translate = Matrix::Identity;
		translate.SetTranslation(position);
		Matrix scaleMat = Matrix::Identity;
		scaleMat.SetScale(scale);
		model = translate * rotation.value.GetMatrix() * scaleMat;
		recalc_model = false;
	}
	return model;
}

Matrix Transform::GetWorldMatrix() {
	Entity* parent = GetEntity()->GetParent();
	if(parent) {
		return parent->GetComponent<Transform>()->GetWorldMatrix() * GetTransform()->GetModelMatrix();
	} else {
		return GetTransform()->GetModelMatrix();
	}
}