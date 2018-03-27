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
//#include "BaseTypes.h"
#include "Cross.h"
#include "String.h"

#include <cstring>
#include <stdlib.h>
#include <stdio.h>

using namespace cross;

String::String() : String("") { }

String::String(const char* cstr) {
	length = strlen(cstr);
	capacity = length;
	data = new char[length + 1];
	memcpy(data, cstr, length + 1);
}

String::String(const char* cstr, U32 len, U32 cap) :
	length(len),
	capacity(cap)
{
	data = new char[capacity + 1];
	memcpy(data, cstr, length + 1);
}

String::String(const char* begin, const char* end) {
	const char* iter = begin;
	while(iter != end) {
		length++;
	}
	capacity = length;
	data = new char[capacity + 1];
	memcpy(data, begin, length + 1);
}

String::String(const String& str) :
	length(str.length),
	capacity(str.capacity)
{
	data = new char[length + 1];
	memcpy(data, str.data, str.length + 1);
}

String::String(S32 number) {
	const U32 max_int_len = 12;	//max possible 32 bit int value
	data = new char[max_int_len];
	length = sprintf(data, "%d", number);
	capacity = length;

	CROSS_ASSERT(length > 0, "Convertion from integer to string failed");
	CROSS_ASSERT(length < max_int_len, "More data written in buffer than was allocated");
}

String::String(U32 number) {
	const U32 max_int_len = 11;
	data = new char[max_int_len];
	length = sprintf(data, "%d", number);
	capacity = length;

	CROSS_ASSERT(length > 0, "Convertion from integer to string failed");
	CROSS_ASSERT(length < max_int_len, "More data written in buffer than was allocated");
}

String::String(float number) {
	const U32 max_float_len = 50;
	data = new char[max_float_len];
	length = sprintf(data, "%f", number);
	capacity = length;

	CROSS_ASSERT(length > 0, "Convertion from integer to string failed");
	CROSS_ASSERT(length < max_float_len, "More data written in buffer than was allocated");
}

String::~String() {
	delete[] data;
}

U32 String::Length() const {
	return length;
}

U32 String::Capacity() const {
	return capacity;
}

const char* String::ToCStr() const {
	return data;
}

S32 String::ToInt() const {
	return atoi(data);
}

float String::ToFloat() const {
	return (float)atof(data);
}

String& String::operator = (const char* cstr) {
	U32 cLen = strlen(cstr);
	if(capacity < cLen) {
		delete[] data;
		data = new char[cLen + 1];
		capacity = cLen;
	}
	length = cLen;
	memcpy(data, cstr, length + 1);
	return *this;
}

String& String::operator = (const String& other) {
	if(capacity < other.length) {
		delete[] data;
		data = new char[other.length + 1];
		capacity = other.length;
	}
	length = other.length;
	memcpy(data, other.data, other.length + 1);
	return *this;
}

bool String::operator == (const char* other) const {
	return strcmp(data, other) == 0;
}

bool String::operator != (const char* other) const {
	return !(*this == other);
}

bool String::operator == (const String& other) const {
	return strcmp(data, other.data) == 0;
}

bool String::operator != (const String& other) const {
	return !(*this == other);
}

void String::operator += (const char* other) {
	U32 otherLen = strlen(other);
	U32 requiredLen = length + otherLen;
	if(requiredLen > capacity) {
		char* newData = new char[requiredLen + 1];
		memcpy(newData, data, length);
		delete[] data;
		data = newData;
		capacity = requiredLen;
	}
	memcpy(data + length, other, otherLen + 1);
	length = requiredLen;
}

void String::operator += (const String& other) {
	U32 requiredLen = length + other.length;
	if(requiredLen > capacity) {
		char* newData = new char[requiredLen + 1];
		memcpy(newData, data, length);
		delete[] data;
		data = newData;
		capacity = requiredLen;
	}
	memcpy(data + length, other.data, other.length + 1);
	length = requiredLen;
}

namespace cross {

String operator + (const String& left, const char* right) {
	U32 cstrLen = strlen(right);
	U32 capacity = cstrLen + left.length;
	String result(left.ToCStr(), left.length, capacity);
	result += right;
	return result;
}

String operator + (const char* left, const String& right) {
	U32 cstrLen = strlen(left);
	U32 capacity = cstrLen + right.length;
	String result(left, cstrLen, capacity);
	result += right;
	return result;
}

String operator + (const String& left, const String& right) {
	U32 capacity = left.length + right.length;
	String result(left.ToCStr(), left.length, capacity);
	result += right;
	return result;
}

}