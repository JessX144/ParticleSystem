#ifndef MATHFUNC_H
#define MATHFUNC_H

#include <glm.hpp>

using namespace glm;

double myRandom();

vec3 myRandomSpeed(float speed_fac, int level);

vec3 multVec(vec3 vector, float f);

vec3 circularPos(float r);

float vec_size(vec3 v);

float vec_dist(vec3 v1, vec3 v2);

bool is_prob(float p);

#endif