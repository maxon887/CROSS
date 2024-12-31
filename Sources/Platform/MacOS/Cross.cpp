
//#include <iostream>


#include "/Users/vladas/CLionProjects/Cross/Sources/Internals/MemoryManager.h"


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <GLFW/glfw3.h>
#pragma clang diagnostic pop


int main(int c,char **args) {

    if(!glfwInit()) {
//        cout<<"Failed to initialize GLFW"<<endl;
    }

    glfwTerminate();


	cross::MemoryManager::Instance()->Dump();
}
