#ifndef PARTICLE_H
#define PARTICLE_H

#include "particle.h" 
#include <glm.hpp>
#include <vector>

using namespace std;
using namespace glm;

struct Particle {
	vec3 position;
	vec4 colour;
	vec3 velocity;
  float acceleration;
	float size;
  int num_b;
};

struct ParticleList {
	Particle* List;
	int num_elements;
	int max_size;
};


#endif