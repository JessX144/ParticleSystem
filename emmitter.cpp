#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

#include <GL\glew.h>
#include <GL\freeglut.h>
#include "mathFunc.h"
#include "particle.h" 
#include "emmitter.h"

using namespace std;
using namespace glm;

const int MaxParticles = 1000;
float gravity = 9.8;

// calculates random particle initial direction 
vec3 particle_direction() {
	// range [-0.2, 0.2]
	float x = -0.2 + 0.4 * myRandom();
	float y = 2 + 1 * myRandom();
	float z = -0.2 + 0.4 * myRandom();

	vec3 dir = vec3(x, y, z);

	return dir;
}

// returns random particle speed 
float particle_speed() {
	// range betweek [0, 1]
	return myRandom();
}

void newParticle(ParticleList *pb) {

	vec3 origin = vec3(0.0, 0.0, 0.0);
	vec4 water_colour = vec4(0.0, 0.0, 1.0, 0.8);
	Particle p = { origin, water_colour, particle_direction(), particle_speed(), 0.2 };
	
	if (pb->num_elements > pb->max_size) {
		// delete element at index 0
		// memmove(pb.List, pb.List + 1, (pb.num_elements- 2) * sizeof(Particle));
    pb->List[0] = pb->List[pb->num_elements];
		pb->num_elements--;
	}

	pb->num_elements++;
	pb->List[pb->num_elements] = p;

}


// How particle effected by gravity 
vec3 gravity_motion(Particle &p) {
	p.direction.y -= gravity * 0.004;

	return p.direction;
}

void update_particles(ParticleList *pb) {

	for (int i = 0; i < pb->num_elements; i++) {
		// if fallen lower on y axis, remove that element 
		if (pb->List[i].direction.y < -2) {
      cout << "too low";
      swap(pb->List[i], pb->List[pb->num_elements]);
			pb->num_elements--;
		}
		// recalculate direction due to gravity 
		pb->List[i].direction = gravity_motion(pb->List[i]);
		// recalculate position due to gravity 
		pb->List[i].position = update_vec(pb->List[i].position, pb->List[i].direction);
	}
}
