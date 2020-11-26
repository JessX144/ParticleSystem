#include <glm.hpp>
#include "mathFunc.h"

using namespace glm;

// random function returns aue [0, 1]
double myRandom()
{
	return (rand() / (double)RAND_MAX);
}

vec3 myRandomSpeed()
{

  float x = -0.4 + (myRandom() * 0.8);
  float y = 10 + (myRandom() * 0.4);
  float z = -0.4 + (myRandom() * 0.8);
  
  vec3 v = vec3(x, y, z);
  return v;
}

vec3 multVec(vec3 vector, float f) {
  vector.x *= f;
  vector.y *= f;
  vector.z *= f;

  return vector;
}