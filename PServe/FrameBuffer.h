#pragma once
#include <stdlib.h>

class Frame {
public:
	//0, 0, 0 for all values
	Frame() : color(vec3()), albedo(vec3()), normal(vec3()) {}
	vec3 color;
	vec3 albedo;
	vec3 normal;
};


class FrameBuffer {
public:
	FrameBuffer(int width, int height) {
		size_t size = width * height * 3;
		albedo = new float[size];
		normal = new float[size];
		color = new float[size];
	}

	~FrameBuffer() {
		delete color;
		delete albedo;
		delete normal;
	}

	float* color;
	float* albedo;
	float* normal;
};