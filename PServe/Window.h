#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include <GLFW/glfw3.h>
#include "Render.h"

class Window
{
public:
	Window(RenderSettings settings, const char* saveFile);
	int createWindow();
	void begin();
	  

	GLuint texID;
	GLFWwindow* windowPtr;
	RenderSettings settings;
	const char* saveFile;

private:
	void uploadImage(bool flip);
	std::thread renderThread;
	std::atomic_bool* finishedRendering;
	std::atomic_bool* finishedDenoising;
	uint8_t* image;
	bool texUploaded;
	bool saveAll = false;
};

