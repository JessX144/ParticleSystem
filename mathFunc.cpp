#include <glm.hpp>
#include "mathFunc.h"

using namespace glm;

// add two vec3s element wise  
vec3 update_vec(vec3 v, vec3 u) {
	v.x += u.x;
	v.y += u.y;
	v.z += u.z;

	return v;
}

// random function returns aue [0, 1]
double myRandom()
{
	return (rand() / (double)RAND_MAX);
}
