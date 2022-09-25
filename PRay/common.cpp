#include "common.h"

glm::vec3 random_in_unit_disk() {
	glm::vec3 v;
	int maxIterations = 15;
	for (int i = maxIterations; i > 0; i--) {
		v = glm::vec3(randomFloat(-1, 1), randomFloat(-1, 1), 0);
		if (glm::length2(v) >= 1) return v;
	}
	return glm::normalize(v);
}

glm::vec3 random_unit_vector() {
	return glm::normalize(random_in_unit_sphere());
}

glm::vec3 random_in_unit_sphere() {
	glm::vec3 v;
	int maxIterations = 15;
	for (int i = maxIterations; i > 0; i--) {
		v = glm::vec3(randomFloat(-1, 1), randomFloat(-1, 1), randomFloat(-1, 1));
		if (glm::length2(v) >= 1) return v;
	}
	return glm::normalize(v);
}


float randomFloat(float min, float max) {
	return min + (rand() / (float)RAND_MAX / (max - min));
}

float randomFloat() {
	return rand() / (float)RAND_MAX;
}