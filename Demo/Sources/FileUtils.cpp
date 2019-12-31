#include "FileUtils.h"
#include "System.h"
#include "File.h"

Array<String> FileUtils::GetAllFilesOfType(const String& fileExtention) {
	Array<String> aggregator;
	AssembleAllFilesOfType(fileExtention, system->AssetsPath(), aggregator);
	return aggregator;
}

Array<String> FileUtils::GetAllFilesOfTypeInDirectory(const String& fileExtention, const String& directory) {
	Array<String> result;
	Array<String> allFiles = system->GetFilesInDirectory(directory);
	for(const String& file : allFiles) {
		if(File::ExtensionFromFile(file) == fileExtention) {
			result.Add(directory + file);
		}
	}
	return result;
}

void FileUtils::AssembleAllFilesOfType(const String& fileExtention, const String& directory, Array<String>& aggregator) {
	Array<String> files = GetAllFilesOfTypeInDirectory(fileExtention, directory);
	aggregator.Combine(files);
	Array<String> subDirectories = system->GetSubDirectories(directory);
	for(const String& subDir : subDirectories) {
		AssembleAllFilesOfType(fileExtention, directory + subDir + "/", aggregator);
	}
}