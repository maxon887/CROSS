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
#include "All.h"

#include <stdlib.h>

static float sign(const cross::Vector2D& p1, const cross::Vector2D& p2, const cross::Vector2D& p3){
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

int cross::Random(int max){
	return rand() % max;
}

int cross::Random(int min, int max){
	return (min) + rand() % (max - min);
}

float cross::Random(float max){
	return rand() / (RAND_MAX / max);
}

float cross::Random(float min, float max){
	return (min)+rand() / (RAND_MAX / (max - min));
}

float cross::Lerp(float v1, float v2, float t){
	return v1 * (1 - t) + v2 * t;
}

cross::Vector3D cross::Lerp(const Vector3D& v1, const Vector3D& v2, float t){
	return v1 * (1 - t) + v2 * t;
}

cross::Quaternion cross::Lerp(const Quaternion& v1, const Quaternion& v2, float t){
	return v1 * (1 - t) + v2 * t;
}

bool cross::PointInRect(const Vector2D& p, const  Rect& rect){
	return  p.x > rect.x &&
			p.x < rect.x + rect.width &&
			p.y > rect.y &&
			p.y < rect.y + rect.height;
}

bool cross::PointInTriangle(const Vector2D& pt, const  Vector2D& v1, const  Vector2D& v2, const  Vector2D& v3){
	bool b1, b2, b3;
	b1 = sign(pt, v1, v2) < 0.0f;
	b2 = sign(pt, v2, v3) < 0.0f;
	b3 = sign(pt, v3, v1) < 0.0f;
	return ((b1 == b2) && (b2 == b3));
}

bool cross::PointInCircle(const Vector2D& p, const  Vector2D& center, float radius){
	float distance = Distance(p, center);
	if(distance < radius)
		return true;
	else return false;
}

float cross::Distance(const Vector2D &v1, const Vector2D &v2){
	return sqrtf((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y));
}

float cross::DistanceSq(const Vector2D &v1, const Vector2D &v2){
	return (v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y);
}

float cross::Distance(const Vector3D &v1, const Vector3D &v2){
	return sqrtf((v1.x - v2.x)*(v1.x - v2.x) +
		(v1.y - v2.y)*(v1.y - v2.y) +
		(v1.z - v2.z)*(v1.z - v2.z));
}

float cross::DistanceSq(const Vector3D &v1, const Vector3D &v2){
	return  (v1.x - v2.x)*(v1.x - v2.x) +
		(v1.y - v2.y)*(v1.y - v2.y) +
		(v1.z - v2.z)*(v1.z - v2.z);
}