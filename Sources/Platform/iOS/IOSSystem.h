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
#include "System.h"

using namespace cross;

class IOSSystem : public System {
public:
    IOSSystem();
    ~IOSSystem();
    
    void Log(const char* msg) override;
    string AssetsPath() override;
    string DataPath() override;
    U64 GetTime() override;
    float GetScreenDPI() override;
    
    void RequestOrientation(Orientation orientation) override;
    bool IsMobile() override;
    void Messagebox(const string& title, const string& message) override;
};
