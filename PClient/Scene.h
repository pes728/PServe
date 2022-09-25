#pragma once
#include "Render.h"

RenderSettings fromFile(const char* filename, FrameBuffer* fb, float* beauty, hittable* world) {
	RenderSettings r;
	char buf[512];
	std::ifstream file;
	file.open(filename);
	
	file.getline(buf, 512);


	
	
	r.width;
}