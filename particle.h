#pragma once
#include <glm.hpp>

struct Particle {
	glm::vec3 position;
	glm::vec4 colour;
	glm::vec3 direction;
	float velocity;
	float size;
	//float rotate;
	//float weight;
	//float age;
	//float life; 

	//Particle()
	//	: position(0.0, 0.0, 0.0)
	//	, velocity(0.0f)
	//	, direction(0.0f)
	//	// colour is initialised as blue, slightly transparent
	//	, colour(0.0, 0.0, 1.0, 0.8)
	//	, life(0.0f) 
	//	, age(0.0f)
	//	, weight(0.0f)
	//	, rotate(0.0f)
	//	, size(0.0f)
	//{}
	/*Particle(vec3 position, vec3 velocity, vec4 colour, vec3 direction,
		float size, float rotate, float weight, float age, float life);*/
};
