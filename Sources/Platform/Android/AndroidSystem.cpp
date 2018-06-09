/*	Copyright � 2015 Lukyanau Maksim

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
#include "AndroidSystem.h"
#include "File.h"

#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

using namespace cross;

AndroidSystem::AndroidSystem(JNIEnv* env, jobject crossActivity, AAssetManager* assManager, String dataPath){
    LOGI("AndroidSystem::AndroidSystem");
	this->data_path = dataPath;
	this->asset_manager = assManager;
	this->cross_activity = crossActivity;
	env->GetJavaVM(&jvm);
}

AndroidSystem::~AndroidSystem(){
    LOGI("AndroidSystem::~AndroidSystem");
}

void AndroidSystem::Log(const char* str){
    LOGI("%s", str);
}

String AndroidSystem::AssetsPath(){
	return "This platform do not specify application assets folder.All assets needs to be load through asset manager";
}

String AndroidSystem::DataPath(){
	return data_path;
}

File* AndroidSystem::LoadAssetFile(const String& filename) {
    AAsset* asset = AAssetManager_open(asset_manager, filename, AASSET_MODE_STREAMING);
    CROSS_RETURN(asset, NULL, "Can not open file #", filename);
    File* file = new File();
    file->size = AAsset_getLength(asset);
    file->data = new Byte[file->size];
    int read = AAsset_read(asset, file->data, file->size);
    CROSS_ASSERT(file->size == read, "File # not read properly", file->name);
    AAsset_close(asset);
    return file;
}

bool AndroidSystem::IsDirectoryExists(const String& dirname) {
	DIR* dir = opendir(dirname);
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

U64 AndroidSystem::GetTime(){
	struct timeval ptv;
	gettimeofday(&ptv, NULL);
	return (ptv.tv_usec + ptv.tv_sec * 1000000LL);
}

float AndroidSystem::GetScreenDPI() {
	JNIEnv* env = GetJNIEnv();
	jclass clazz = env->GetObjectClass(cross_activity);
	jmethodID methodID = env->GetMethodID(clazz, "GetScreenDPI", "()I");
	float dpi = (float)env->CallIntMethod(cross_activity, methodID);
    env->DeleteLocalRef(clazz);
    return dpi;
}

void AndroidSystem::PromtToExit(){
	JNIEnv* env = GetJNIEnv();
	jclass clazz = env->GetObjectClass(cross_activity);
	jmethodID methodID = env->GetMethodID(clazz, "PromtToExit", "()V");
	env->CallVoidMethod(cross_activity, methodID);
    env->DeleteLocalRef(clazz);
}

void AndroidSystem::RequestOrientation(Orientation orientation) {
    JNIEnv* env = GetJNIEnv();
    jclass clazz = env->GetObjectClass(cross_activity);
    jmethodID methodID = env->GetMethodID(clazz, "RequestOrientation", "(I)V");
    env->CallVoidMethod(cross_activity, methodID, orientation);
    env->DeleteLocalRef(clazz);
}

void AndroidSystem::Messagebox(const String& title, const String& message) {
    JNIEnv* env = GetJNIEnv();
    jclass clazz = env->GetObjectClass(cross_activity);
    jmethodID methodID = env->GetMethodID(clazz, "MessageBox", "(Ljava/lang/String;Ljava/lang/String;)V");
    env->CallVoidMethod(cross_activity, methodID, env->NewStringUTF(title), env->NewStringUTF(message));
    env->DeleteLocalRef(clazz);
}

bool AndroidSystem::IsMobile() {
    return true;
}

void AndroidSystem::Exit() {
    JNIEnv* env = GetJNIEnv();
    jclass clazz = env->GetObjectClass(cross_activity);
    jmethodID methodID = env->GetMethodID(clazz, "Exit", "()V");
    env->CallVoidMethod(cross_activity, methodID);
    env->DeleteLocalRef(clazz);
    jvm->DetachCurrentThread();
}

void AndroidSystem::Sleep(float milis) {
    usleep(milis*1000);
}

void AndroidSystem::CallActivityVoidMethod(const String &methodName) {
    JNIEnv* env = GetJNIEnv();
    jclass clazz = env->GetObjectClass(cross_activity);
    jmethodID methodID = env->GetMethodID(clazz, methodName, "()V");
    env->CallVoidMethod(cross_activity, methodID);
}

void AndroidSystem::CallActivityVoidMethod(const String& methodName, const String& parameter){
    JNIEnv* env = GetJNIEnv();
    jclass clazz = env->GetObjectClass(cross_activity);
    jmethodID methodID = env->GetMethodID(clazz, methodName, "(Ljava/lang/String;)V");
    env->CallVoidMethod(cross_activity, methodID, env->NewStringUTF(parameter));
}

void AndroidSystem::DetachFromJVM() {
    jvm->DetachCurrentThread();
}

JNIEnv* AndroidSystem::GetJNIEnv(){
    JNIEnv* env;
    int getEnvStat = jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
    switch(getEnvStat){
    case JNI_OK:
    	return env;
    case JNI_EDETACHED:
		LogIt("Attempt to attach thread to JVM");
        CROSS_RETURN(jvm->AttachCurrentThread(&env, NULL) == 0, NULL, "Failed to attach thread")
		return env;
    case JNI_EVERSION:
        CROSS_RETURN(false, NULL, "Java environment: version not supported");
    default:
        CROSS_RETURN(false, NULL, "Unknown JNI state");
    }
}


