#pragma once
#include <stdlib.h>

class Frame {
public:
	//0, 0, 0 for all values
	Frame() : color(glm::vec3()), albedo(glm::vec3()), normal(glm::vec3()) {}
	glm::vec3 color;
	glm::vec3 albedo;
	glm::vec3 normal;
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