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
  // x and z is between -0.6 and 0.6 
  float x = (-0.6 + (myRandom() * 1.2)) * speed_fac / level;
  float y = (10 + (myRandom() * 0.4)) * speed_fac;
  float z = (-0.6 + (myRandom() * 1.2)) * speed_fac / level;
  
  vec3 v = vec3(x, y, z);
  return v;
}

// element wise vector multiplication 
vec3 multVec(vec3 vector, float f) {
  vector.x *= f;
  vector.y *= f;
  vector.z *= f;

  return vector;
}

// returns random value within circle of given radius 
vec3 circularPos(float r) {
  float theta = myRandom() * 2 * PI;
  float x = r * cos(theta);
  float z = r * sin(theta);
  return vec3(x, 0, z);
}

// returns size of 3d vector
float vec_size(vec3 v) {
  float x = pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2);
  return sqrt(x);
}

// calculates distance (2-norm) of 3d vector 
float vec_dist(vec3 v1, vec3 v2) {
  float xdiff = v1.x - v2.x;
  float ydiff = v1.y - v2.y;
  float zdiff = v1.z - v2.z;

  float x = pow(xdiff, 2) + pow(ydiff, 2) + pow(zdiff, 2);
  return sqrt(x);
}

// probability function 
bool is_prob(float p) {
  if (myRandom() <= p) {
    return true;
  }
  return false;
}