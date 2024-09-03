#include "Platform/MacOS/MacSystem.h"
#include "Internals/Audio.h"
#include "Internals/GraphicsGL.h"
#include "Game.h"
#include "Input.h"
#include "Screen.h"
#include "Config.h"

#include <iostream>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <GLFW/glfw3.h>
#pragma clang diagnostic pop

using namespace std;
using namespace cross;

Vector2D mouse_pos;
float frame_to_window_ratio;

void GLFWErrorCallback(int error, const char* description) {
    cout << "GLFW Error occured - " << error << "\n\t" << description << endl;
}

void GLFWResizeCallback(GLFWwindow* win, int width, int height) {
    cross::os->SetWindowSize(width, height);
}

void GLFWMouseMoveCallback(GLFWwindow* win, double xPos, double yPos) {
    if(input) {
        xPos *= frame_to_window_ratio;
        yPos *= frame_to_window_ratio;
        mouse_pos.x = (float)xPos;
        mouse_pos.y = (float)yPos;
        input->TargetActionMove.Emit((float)xPos, (float)yPos, 0);
    }
}

void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if(input) {
        switch (action) {
        case GLFW_PRESS:
            input->TargetActionDown.Emit(mouse_pos.x, mouse_pos.y, button);
            break;
        case GLFW_RELEASE:
            input->TargetActionUp.Emit(mouse_pos.x, mouse_pos.y, button);
            break;
        default:
            break;
        }
    }
}

void GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	input->Scroll.Emit(yoffset);
}

void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Key mappedKey = Key::ESCAPE;
	switch(key) {
	case GLFW_KEY_ESCAPE:
		mappedKey = Key::ESCAPE;
		break;
	case GLFW_KEY_ENTER:
		mappedKey = Key::ENTER;
		break;
	case GLFW_KEY_TAB:
		mappedKey = Key::TAB;
		break;
	case GLFW_KEY_BACKSPACE:
		mappedKey = Key::BACK;
		break;
	case GLFW_KEY_INSERT:
		mappedKey = Key::INSERT;
		break;
	case GLFW_KEY_DELETE:
		mappedKey = Key::DEL;
		break;
	case GLFW_KEY_HOME:
		mappedKey = Key::HOME;
		break;
	default:
		return;
	}
	switch(action) {
	case GLFW_PRESS:
		input->KeyPressed.Emit(mappedKey);
		break;
	case GLFW_RELEASE:
		input->KeyReleased.Emit(mappedKey);
		break;
	}
}

void GLFWCharCallback(GLFWwindow* window, unsigned int codepoint) {
	input->CharEnter.Emit(codepoint);
}

GLFWmonitor* GetMonitorForWindow(GLFWwindow* window) {
    int monitorsCount = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&monitorsCount);
    for (int i = 0; i < monitorsCount; i++) {
        GLFWmonitor* monitor = monitors[i];
        int XPos = 0;
        int YPos = 0;
        glfwGetMonitorPos(monitor, &XPos, &YPos);
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        Rect screenRect(XPos, YPos, mode->width, mode->height);
        int windowXPos = 0;
        int windowYPos = 0;
        glfwGetWindowPos(window, &windowXPos, &windowYPos);
        if (PointInRect(Vector2D(windowXPos, windowYPos), screenRect)) {
            return monitor;
        }
    }
    return nullptr;
}

int main(int c,char **args) {
    cross::os = new MacSystem(args[0]);

    if(!glfwInit()) {
        cout<<"Failed to initialize GLFW"<<endl;
    }

    glfwSetErrorCallback(GLFWErrorCallback);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Cross++", NULL, NULL);
    if(!window){
        cout<<"Failed to create GLFW window"<<endl;
    }
    glfwSetFramebufferSizeCallback(window, GLFWResizeCallback);
    glfwSetCursorPosCallback(window, GLFWMouseMoveCallback);
    glfwSetMouseButtonCallback(window, GLFWMouseButtonCallback);
	glfwSetScrollCallback(window, GLFWScrollCallback);
	glfwSetKeyCallback(window, GLFWKeyCallback);
	glfwSetCharCallback(window, GLFWCharCallback);
    
    glfwMakeContextCurrent(window);

    audio = new Audio();
    gfxGL = new GraphicsGL();
    game = CrossMain();

	int windowWidth = config->GetInt("WindowWidth", 800);
	int windowHeight = config->GetInt("WindowHeight", 600);
	glfwSetWindowSize(window, windowWidth, windowHeight);
	int posX = config->GetInt("WindowPosX", 200);
	int posY = config->GetInt("WindowPosY", 100);
	glfwSetWindowPos(window, posX, posY);
    int frameWidth = 0;
    int frameHeight = 0;
	glfwGetFramebufferSize(window, &frameWidth, &frameHeight);
	cross::os->SetWindowSize(frameWidth, frameHeight);
    frame_to_window_ratio = frameWidth / (float)windowWidth;

    int widthMM, heightMM;
    GLFWmonitor* monitor = GetMonitorForWindow(window);
    if(!monitor) {
        monitor = glfwGetPrimaryMonitor();
    }
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwGetMonitorPhysicalSize(monitor, &widthMM, &heightMM);
    float dpi = mode->width * frame_to_window_ratio / (widthMM / 25.4);
    MacSystem* macSystem = (MacSystem*)cross::os;
    macSystem->SetScreenDPI(dpi);

    game->Start();
    
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        game->EngineUpdate();
        glfwSwapBuffers(window);
    }

    glfwGetWindowSize(window, &windowWidth, &windowHeight);
	config->SetInt("WindowWidth", windowWidth);
	config->SetInt("WindowHeight", windowHeight);
	glfwGetWindowPos(window, &posX, &posY);
	config->SetInt("WindowPosX", posX);
	config->SetInt("WindowPosY", posY);

	game->GetCurrentScreen()->Stop();
	game->Stop();

    glfwTerminate();

    delete game;
    delete gfxGL;
    delete audio;
    delete cross::os;
}
