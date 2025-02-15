#pragma once
#include "System.h"

namespace cross {

class MacSystem : public System {
public:
    MacSystem(const String& workingDir);
    
    void Log(const char* msg) override;
    String AssetsPath() override;
    String DataPath() override;
    U64 GetTime() override;
	float GetScreenDPI() override;
	bool IsDirectoryExists(const String& filepath) override;
	void CreateDirectory(const String& dirname) override;
	void Delete(const String& path) override;
	Array<String> GetSubDirectories(const String& filepath) override;
	Array<String> GetFilesInDirectory(const String& filepath) override;
	bool Alert(const String& msg) override;
	void Messagebox(const String& title, const String& msg) override;
	void Sleep(float milis) override;
	String OpenFileDialog(const String& extension = "*.*", bool saveDialog = false) override;

    void SetScreenDPI(float dpi);
	void SetAssetPath(const String& path);
	
	void SetAppIcon();

private:
    float dpi = 96.f;

    String working_dir = "";
	String assets_path = "";
	String data_path = "";
};

}
