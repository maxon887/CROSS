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
#pragma once
#include "Cross.h"
#include "Event.h"

namespace cross {
	
/*	Provides platform specific functional. Use this class for logging, file access and other platform specific function */
class System {
public:
	enum Orientation {
		LANDSCAPE,
		PORTRAIT,
		AUTO,
		COUNT
	};
	/* Occurs when window size changed */
	Event<S32, S32> WindowResized;
	/* Occurs when window orientation changed */
	Event<System::Orientation> OrientationChanged;

	virtual ~System() { }

	/* Print message in console. See LogIt() if you need formated output */
	virtual void Log(const char* msg) = 0;
	/* Returns path to the application assets folder */
	virtual string AssetsPath() = 0;
	/* Returns path to the application data folder */
	virtual string DataPath() = 0;
	/* Return current time in microseconds */
	virtual U64 GetTime() = 0;

	/* Load file from absolute path */
	virtual File* LoadFile(const string& filename);
	/* Load file from game asset directory */
	virtual File* LoadAssetFile(const string& filename);
	/* Load file from data directory */
	virtual File* LoadDataFile(const string& filename);
	/* Save file by absolute folder */
	virtual void SaveFile(File* file);
	/* Save file to data folder */
	virtual void SaveDataFile(File* file);
	/* Checks if file exists in provided path */
	virtual bool IsDataFileExists(const string& filename);
	/* Notifies user that something happened by system message. Returns true if system should skip this message in further use */
	virtual bool Alert(const string& msg);
	/* Notifies user that something happened by system message. Usually something bad. Use it at last case */
	virtual void Alert(const char* filename, unsigned int line, const char* msg, va_list list);
	/* Force current thread to sleep */
	virtual void Sleep(float milis);
	/* Requests to set required orientation. Can not to be set if system not allow to change orientation */
	virtual void RequestOrientation(Orientation orientation) { }
	/* Returns true if current working platform is mobile one */
	virtual bool IsMobile() { return false; }
	/* Returns device screen DPI. This is approximate value. Real devices does not know their screen sizes */
	virtual float GetScreenDPI();
	/* Ask user about exit from application */
	virtual void PromtToExit() { }
	/* Obtains clipboard data as text if available */
	virtual string GetClipboard();
	/* Sets current string int clipboard buffer */
	virtual void SetClipboard(const string& data);
	/* Prints message with formated input */
	void LogIt(const char* format, ...);
	/* Prints string */
	void LogIt(const string& msg);
	/* Prints vector */
	void LogIt(const Vector3D& vector);
	/* How much screen elements must be increased or decreased due to device DPI */
	float GetScreenScale();
	/* Returns window width in pixels */
	S32 GetWindowWidth() const;
	/* Returns window height in pixels */
	S32 GetWindowHeight() const;
	/* Returns current device orientation */
	Orientation GetDeviceOrientation() const;
	/* Returns physical screen aspect ration */
	float GetAspectRatio() const;
	/* Sets physical device screen size */
	void SetWindowSize(S32 width, S32 height);

protected:
	virtual void Messagebox(const string& title, const string& msg);

private:
	S32 window_width	= -1;
	S32 window_height	= -1;
	Set<int> asserts_hashes;
};
	
}
