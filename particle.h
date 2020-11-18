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
	vec3 direction;
	float velocity;
	float size;
};

// Vector to store particles 
//typedef vector<Particle> ParticleBuffer;
//typedef Particle* ParticleBuffer;

struct ParticleList {
	Particle* List;
	int num_elements;
	int max_size;
};

//typedef Particle* ParticleList;

#endif