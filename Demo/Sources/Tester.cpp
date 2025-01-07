#include "Tester.h"
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
#include "Cross.h"

using namespace cross;

int Debuggable(String str)
{
	int i = 0;
	i += i * 2;
	return i;
}

int NotDebuggable()
{
	int i = 0;
	i++;
	i++;
	return i;
}

void RunTest()
{
	String test = "Blah";
	test.Uppercase();
	int val2 = NotDebuggable();
	int val = Debuggable(test + "Money");
	val++;
}
