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

namespace cross {

class String {
public:
	static String Format(const String& fromat);
	template<class First, class... Args>
	static String Format(const String& format, First value, Args... args);

	String();
	String(const char* cstr);
	String(const char* begin, const char* end);
	String(const String& str);
	String(String&& str);
	String(S32 number);
	String(U32 number);
	String(float number);
	String(const Color& color);
	template<class Value>
	String(Value value, const char* format, U32 bufferSize);
	~String();

	/* Return length of the string. Number of placed characters spots */
	U32 Length() const;
	/* Return count of characters that can be placed without string memory allocation */
	U32 Capacity() const;
	/* Cleans all characters in this string without memory reallocation */
	void Clean();
	/* Returns true if string is empty */
	bool IsEmpty() const;
	/* Calculates has from current String */
	U64 Hash() const;
	/* Returns position of a first occurrence of char c in this String or -1 if wasn't found */
	S32 Find(char c) const;
	/* Returns position of a first occurrence of subStr in this String or -1 if wasn't found */
	S32 Find(const char* subStr) const;
	/* Returns position of a last occurrence of char in this String or -1 if wasn't found */
	S32 FindLast(char c) const;
	/* Returns first occurrence of any character in provided sequence */
	S32 FindFirstOf(const char* sequence) const;
	/* Returns first occurrence of any character in provided sequence started from specific position */
	S32 FindFirstOf(const char* sequence, U32 startPos) const;
	/* Returns first occurrence of not of any character in provided sequence */
	S32 FindNonFirstOf(const char* sequence) const;
	/* Returns first occurrence of not of any character in provided sequence started from specific position */
	S32 FindNonFirstOf(const char* sequence, U32 startPos) const;
	/* Converts this string to uppercase */
	void Uppercase();
	/* Converts this string to lowercase */
	void Lowercase();
	/* Removes first occurrence of subStr in current String */
	bool Remove(const char* subStr);
	/* Removes first occurrence of character in current String */
	bool Remove(char c);
	/* Removes all characters before first argument and after last from current String */
	void Cut(U32 first, U32 last);
	/* Inserts provided String in the midle of current string at postion */
	void Insert(U32 position, const String& str);
	/* Returns cut String from first character to last by arguments */
	String SubString(U32 first, U32 last) const;
	/* Returns data pointer on this String */
	char* ToCStr() const;
	/* Converts this String to integer type and returns it */
	S32 ToInt() const;
	/* Converts this String to integer type. Assumed that number stored in provided base */
	S32 ToInt(U32 base) const;
	/* Converts this String to float type and returns it */
	float ToFloat() const;
	/* Converts this String to Color object and returns it */
	Color ToColor() const;

	String& operator = (const char* other);
	String& operator = (const String& other);
	String& operator = (String&& other);

	bool operator == (const char* other) const;
	bool operator != (const char* other) const;
	bool operator == (const String& other) const;
	bool operator != (const String& other) const;

	void operator += (const char* other);
	void operator += (const String& other);

	operator const char* () const;

	friend String operator + (const String& left, const char* right);
	friend String operator + (const char* left, const String& right);
	friend String operator + (const String& left, const String& right);

	friend bool operator < (const String& left, const String& right);

private:
	char* data = nullptr;
	U32 length = 0;
	U32 capacity = 0;

	String(const char* cstr, U32 length, U32 capacity);
};

template<class First, class... Args>
String String::Format(const String& format, First value, Args... args) {
	S32 spot = format.Find('#');
	CROSS_ASSERT(spot != -1, "Formatter error. More values provied than expected");
	String result = format;
	result.Remove('#');
	result.Insert(spot, String(value));
	return Format(result, args...);
}

template<class Value>
String::String(Value value, const char* format, U32 bufferSize) {
	data = (char*)CROSS_ALLOC(bufferSize + 1);
	length = sprintf(data, format, value);
	capacity = bufferSize;

	CROSS_ASSERT(length > 0, "Convertion from integer to string failed");
	CROSS_ASSERT(length < bufferSize, "More data written in buffer than was allocated");
}

}