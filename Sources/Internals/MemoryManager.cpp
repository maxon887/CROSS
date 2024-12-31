
#include "MemoryManager.h"


#include <cstring>
#include <iostream>
#include <cassert>


using namespace cross;
using namespace std;


#undef new

struct MemoryObject {
    void* address;
    U64 size;
};

static U64 alloc_objects;

void* operator new(size_t size) {
	void* result = MemoryManager::Instance()->Alloc(size, __FILE__, __LINE__);
	return result;
}

void operator delete(void* p) noexcept {
//	MemoryManager::Instance()->Free(p);
}

MemoryManager MemoryManager::instance;

MemoryManager* MemoryManager::Instance() {
	return &instance;
}

MemoryManager::MemoryManager() {
}


void* MemoryManager::Alloc(U64 size, const char* filename, U64 line) {

    MemoryObject memObj {};

    auto address = malloc(size);

    memObj.address = address;
    memObj.size = size;

//    alloc_objects.push_back(memObj);

    alloc_objects += size;


    return malloc(size);
}


void MemoryManager::Free(void* address) {
//		if(address == nullptr) {
//			return;
//		}
//		for(U64 i = 0; i < object_count; i++) {
//			if(alloc_objects[i].address == address) {
////				free(address);
////				if(i != object_count - 1) {
////					memcpy(alloc_objects + i, alloc_objects + object_count - 1, sizeof(MemoryObject));
////				}
//                alloc_objects.erase(alloc_objects.begin() + i);
//
//				object_count--;
//				return;
//			}
//		}

}

U64 MemoryManager::Dump() {
	U64 totalBytes = 0;


    std::cout << alloc_objects << std::endl;

    if(totalBytes != 0) {
		assert(false);
	} else {
        std::cout << "ZAEBIZ" << std::endl;
	}
	return totalBytes;
}

