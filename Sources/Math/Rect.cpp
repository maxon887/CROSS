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
#include "Rect.h"
#include "Cross.h"

using namespace cross;

Rect::Rect(){
	x = 0;
	y = 0;
	width = 0;
	height = 0;
}

Rect::Rect(const Vector2D& pos, float w, float h){
	this->x = pos.x;
	this->y = pos.y;
	this->width = w;
	this->height = h;
}

Rect::Rect(float x, float y, float w, float h){
	this->x = x;
	this->y = y;
	this->width = w;
	this->height = h;
}

bool Rect::IsInRect(const Vector2D& point){
	return point.x >= x && point.y >= y && point.x <= x + width && point.y <= y + height;
}