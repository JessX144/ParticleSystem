#ifndef MATHFUNC_H
#define MATHFUNC_H

#include <glm.hpp>

using namespace glm;

double myRandom();

vec3 myRandomSpeed(float speed_fac, int level);

vec3 multVec(vec3 vector, float f);

#endif