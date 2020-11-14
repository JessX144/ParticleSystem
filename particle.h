#pragma once
#include <glm.hpp>

struct Particle {
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec4 colour;
	glm::vec3 direction;
	float size;
	float rotate;
	float weight;
	float age;
	float life; 

	Particle()
		: position(0.0, 0.0, 0.0)
		, velocity(0.0f)
		, direction(0.0f)
		// colour is initialised as cyan 
		, colour(0.0, 1.0, 1.0, 1.0)
		, life(0.0f) 
		, age(0.0f)
		, weight(0.0f)
		, rotate(0.0f)
		, size(0.0f)
	{}
};
