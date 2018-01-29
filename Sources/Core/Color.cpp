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
#include "Color.h"
#include "Cross.h"

#include <cstdlib>

using namespace cross;

const Color Color::Red(1.f, 0.f, 0.f);
const Color Color::Green(0.f, 1.f, 0.f);
const Color Color::Blue(0.f, 0.f, 1.f);
const Color Color::White(1.f, 1.f, 1.f);
const Color Color::Black(0.f, 0.f, 0.f);
const Color Color::Yellow(1.f, 1.f, 0.f);
const Color Color::Purple(1.f, 0.f, 1.f);

Color Color::Random(){
	return Color(cross::Random(0.f, 1.f), cross::Random(0.f, 1.f), cross::Random(0.f, 1.f));
}

Color::Color(float all) :
	Color(all, all, all)
{ }

Color::Color(float r, float g, float b):
	R(r),
	G(g),
	B(b),
	A(1.0f)
{ }

Color::Color(int r, int g, int b):
	R(r / 255.f),
	G(g / 255.f),
	B(b / 255.f),
	A(1.0f)
{ }

Color::Color(float r, float g, float b, float a) :
	R(r),
	G(g),
	B(b),
	A(a)
{ }

Color::Color(int r, int g, int b, int a) :
	R(r / 255.f),
	G(g / 255.f),
	B(b / 255.f),
	A(a / 255.f)
{ }

Color::Color(const char* hex){
	String rStr = String(hex + 0, hex + 2);
	U64 rInt = std::strtol(rStr.c_str(), nullptr, 16);
	String gStr = String(hex + 2, hex + 4);
	U64 gInt = std::strtol(gStr.c_str(), nullptr, 16);
	String bStr = String(hex + 4, hex + 6);
	U64 bInt = std::strtol(bStr.c_str(), nullptr, 16);
	String aStr = String(hex + 6, hex + 8);
	U64 aInt = std::strtol(aStr.c_str(), nullptr, 16);
	R = rInt / 255.f;
	G = gInt / 255.f;
	B = bInt / 255.f;
	A = aInt / 255.f;
}

void Color::SetData(const char* data){
	memcpy(&R, data, sizeof(Color));
}

const float* Color::GetData() const{
	return &R;
}

bool Color::operator==(const Color &c) const{
	if(	this->R == c.R &&
		this->G == c.G &&
		this->B == c.B ){
		return true;
	}else{
		return false;
	}
}

bool Color::operator!=(const Color &c) const{
	return !((*this) == c);
}
