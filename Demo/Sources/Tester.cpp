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
#include <vector>
#include "Cross.h"

class Sexy {
public:
    Sexy(int a, int b) : ass(a), boobs(b) { }

	int boobs = 8;
	int ass = 9;
};

void StringConsumer(const cross::String& str)
{
	int i = 0;
	i++;
}

void RunTest()
{
	Sexy Olga{ 7, 8 };
	Sexy Gosha{ 6, 6 };

	std::vector<Sexy> stdArray;
	stdArray.push_back(Olga);
	stdArray.push_back(Gosha);

	cross::Array<Sexy> crossArray;
	crossArray.Add(Olga);
	crossArray.Add(Gosha);

	std::string stdString = "Some std user text here";
	cross::String crossString = "Cross could do the string to";

	StringConsumer("In place created string");
}