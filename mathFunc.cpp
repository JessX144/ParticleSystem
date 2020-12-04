#include <glm.hpp>
#include "mathFunc.h"
#include <math.h>

#define PI 3.14159;

using namespace glm;

// random function returns aue [0, 1]
double myRandom()
{
	return (rand() / (double)RAND_MAX);
}

vec3 myRandomSpeed(float speed_fac, int level)
{
  // y is between 10 and 10.4
  float x = (-0.6 + (myRandom() * 1.2)) * speed_fac / level;
  float y = (10 + (myRandom() * 0.4)) * speed_fac;
  float z = (-0.6 + (myRandom() * 1.2)) * speed_fac / level;
  
  vec3 v = vec3(x, y, z);
  return v;
}

vec3 multVec(vec3 vector, float f) {
  vector.x *= f;
  vector.y *= f;
  vector.z *= f;

  return vector;
}

vec3 circularPos(float r) {
  float theta = myRandom() * 2 * PI;
  float x = r * cos(theta);
  float z = r * sin(theta);
  return vec3(x, 0, z);
}

float vec_size(vec3 v) {
  float x = pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2);
  return sqrt(x);
}