//
//  common.h
//  PRender
//
//  Created by Peter Sandquist on 10/1/20.
//

#ifndef common_h
#define common_h

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdlib>
#include <limits>
#include <memory>
#include <fstream>

using std::shared_ptr;
using std::make_shared;

#include "ray.h"
#include "glm/vec3.hpp"
#include "glm/geometric.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtx/string_cast.hpp"

float randomFloat();

float randomFloat(float min, float max);

glm::vec3 random_in_unit_sphere();

glm::vec3 random_unit_vector();

glm::vec3 random_in_unit_disk();



#endif
