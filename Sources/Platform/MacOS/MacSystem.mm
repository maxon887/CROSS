#include "MacSystem.h"

#include "File.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#import <Cocoa/Cocoa.h>
#pragma clang diagnostic pop
#include <sys/time.h>
#include <sys/stat.h>
#include <dirent.h>

#pragma clang diagnostic ignored "-Wdeprecated-declarations"

using namespace cross;

MacSystem::MacSystem(const String& workingDir) {
    working_dir = File::PathFromFile(workingDir);
    if(IsDirectoryExists("Assets/")) {
        assets_path = "Assets/";
    } else if(IsDirectoryExists("../Resources/Assets/")) {
        assets_path = "../Resources/Assets/";
    } else if(IsDirectoryExists("../../../Assets/")) {
        assets_path = "../../../Assets/";
    } else if(IsDirectoryExists("../../../../Assets/")) {
        assets_path = "../../../../Assets/";
    } else if(IsDirectoryExists("../../../../../Assets/")) {
        assets_path = "../../../../../Assets/";
    } else {
        CROSS_ASSERT(false, "Can not find Assets directory");
    }
	
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
	NSString *applicationSupportDirectory = [paths firstObject];
	NSLog(@"applicationSupportDirectory: '%@'", applicationSupportDirectory);
	data_path = [applicationSupportDirectory cStringUsingEncoding:[NSString defaultCStringEncoding]];
	data_path += "/Data/";
}

void MacSystem::Log(const char *msg) {
    printf("%s\n", msg);
}

String MacSystem::AssetsPath() {
	return assets_path;
}

String MacSystem::DataPath() {
	return data_path;
}

U64 MacSystem::GetTime() {
    struct timeval ptv;
    gettimeofday(&ptv, NULL);
    return (ptv.tv_usec + ptv.tv_sec * 1000000LL);
}

float MacSystem::GetScreenDPI() {
    return dpi;
}

bool MacSystem::IsDirectoryExists(const cross::String& filepath) {
    DIR* dir = opendir(working_dir + filepath);
    dirent* dr = nullptr;
    if(dir && (dr = readdir(dir))) {
        if(dr->d_type == DT_DIR) {
            return true;
        } else {
            return false;
        }
    } else {
        CROSS_ASSERT(errno == ENOENT, "IsDirectoryExists() error code - #\nDescription - #", errno, strerror(errno));
        return false;
    }
}

void MacSystem::CreateDirectory(const String &dirname) {
	mkdir(dirname, 0775);
}

void MacSystem::Delete(const String& path) {
	remove(path);
}

Array<String> MacSystem::GetSubDirectories(const String& filepath) {
	Array<String> files;
	DIR* dir = opendir(filepath);
    if(!dir) {
        CROSS_ASSERT(errno == ENOENT, "GetSubDirectories() error code - #\nDescription - #", errno, strerror(errno));
        return files;
    }
	dirent* dr = nullptr;
	while(dir && (dr = readdir(dir))) {
		String name = dr->d_name;
		if(dr->d_type == DT_DIR && name != "." && name != "..") {
			files.Add(name);
		}
	}
	closedir(dir);
	return files;
}

Array<String> MacSystem::GetFilesInDirectory(const String& filepath) {
	Array<String> files;
	DIR* dir = opendir(filepath);
    if(!dir) {
        CROSS_ASSERT(errno == ENOENT, "GetSubDirectories() error code - #\nDescription - #", errno, strerror(errno));
        return files;
    }
    
	dirent* dr = nullptr;
	while(dir && (dr = readdir(dir))) {
		if(dr->d_type != DT_DIR) {
			files.Add(dr->d_name);
		}
	}
	closedir(dir);
	return files;
}

bool MacSystem::Alert(const String& msg) {
	NSAlert *alert = [[NSAlert alloc] init];
	[alert setMessageText:[NSString stringWithCString:"Something goes wrong"
								  encoding:[NSString defaultCStringEncoding]]];
	[alert setInformativeText:[NSString stringWithCString:msg.ToCStr()
									  encoding:[NSString defaultCStringEncoding]]];
	[alert addButtonWithTitle:[NSString stringWithCString:"Ok"
							   encoding:[NSString defaultCStringEncoding]]];
	[alert addButtonWithTitle: [NSString stringWithCString:"Abort"
							   encoding:[NSString defaultCStringEncoding]]];
	[alert addButtonWithTitle:[NSString stringWithCString:"Skip"
							   encoding:[NSString defaultCStringEncoding]]];
	NSModalResponse response = [alert runModal];
	switch(response) {
		case NSAlertFirstButtonReturn:
			return false;
		case NSAlertSecondButtonReturn:
			*((volatile unsigned int*)0) = 0xDEAD;
		case NSAlertThirdButtonReturn:
			return true;
		default:
			return false;
	}
}

void MacSystem::Messagebox(const String& title, const String& msg) {
	NSAlert *alert = [[NSAlert alloc] init];

	[alert setMessageText:[NSString stringWithCString:title.ToCStr()
								  encoding:[NSString defaultCStringEncoding]]];
	[alert setInformativeText:[NSString stringWithCString:msg.ToCStr()
									  encoding:[NSString defaultCStringEncoding]]];

	[alert runModal];
}

void MacSystem::Sleep(float milis) {
	usleep(milis * 1000.f);
}

String MacSystem::OpenFileDialog(const String& extension, bool saveDialog) {
	NSURL* fileURL = [[NSURL alloc] init];
	if(!saveDialog) {
		NSOpenPanel* openDialog = [NSOpenPanel openPanel];
		[openDialog setCanChooseFiles:YES];
		[openDialog setAllowsMultipleSelection:NO];
		[openDialog setCanChooseDirectories:NO];
		
		if(extension != "*.*") {
			String shortExtension = extension.SubString(2, 5);
			NSString* nsExtension = [NSString stringWithCString:shortExtension];
			NSArray* fileTypes = [NSArray arrayWithObjects:nsExtension,nil];
			[openDialog setAllowedFileTypes:fileTypes];
		}
		if([openDialog runModal] == NSOKButton) {
			NSArray* urls = [openDialog URLs];
			if([urls count] > 0) {
				fileURL = [urls objectAtIndex:0];
				if(fileURL.absoluteString.length > 0) {
					String result = [fileURL fileSystemRepresentation];
					result = File::FromAbsoluteToAssetPath(result);
					return result;
				}
			}
		}
	} else {
		NSSavePanel* openDialog = [NSSavePanel savePanel];
		if([openDialog runModal] == NSOKButton) {
			fileURL = [openDialog URL];
			if(fileURL.absoluteString.length > 0) {
				String result = [fileURL fileSystemRepresentation];
				return result;
			}
		}
	}
	return String();
}

void MacSystem::SetScreenDPI(float newDPI) {
    dpi = newDPI;
}

void MacSystem::SetAssetPath(const String& path) {
	assets_path = path;
}
