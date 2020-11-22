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

// calculates random particle initial direction 
vec3 particle_direction() {
	// range [-0.2, 0.2]
	float x = -0.3 + 0.6 * myRandom();
	float y = 3 + 0.3 * myRandom();
	float z = -0.3 + 0.6 * myRandom();

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
	Particle p = { origin, water_colour, particle_direction(), particle_speed(), 9.8, 0.2, 0};
	
	if (pb->num_elements > pb->max_size) {
    cout << "too many";
    pb->List[0] = pb->List[pb->num_elements];
    //swap(pb->List[0], pb->List[pb->num_elements]);
    pb->num_elements--;
	}

	pb->num_elements++;
	pb->List[pb->num_elements] = p;

}

//void newParticleScatter(ParticleList *pb, Particle p) {
//
//  vec4 water_colour = vec4(0.0, 0.0, 1.0, 0.8);
//
//  p.position.y = 0.0;
//  p.direction.y = 0.01 * (p.direction.y);
//
//  for (int i = 0; i < 5; i++) {
//    float y_dir = 0.1 * p.direction.y * -1;
//    Particle new_p = {vec3(p.position.x, -3.8, p.position.z), water_colour, vec3(p.direction.x, y_dir, p.direction.z), particle_speed(), 9.8, p.size *= 0.5, 1};
//
//    if (pb->num_elements > pb->max_size) {
//      cout << "too many";
//      pb->List[0] = pb->List[pb->num_elements];
//      pb->num_elements--;
//    }
//
//    pb->num_elements++;
//    pb->List[pb->num_elements] = new_p;
//  }
//
//}

// How particle effected by gravity 
vec3 gravity_motion(Particle &p) {
	p.direction.y -= p.acceleration * 0.007;

	return p.direction;
}

void update_particles(ParticleList *pb) {

	for (int i = 0; i < pb->num_elements; i++) {
    if (pb->List[i].num_b == 1 && pb->List[i].direction.y <= 0.1) {
      pb->List[i] = pb->List[pb->num_elements];
      pb->num_elements--;
    }
		// if fallen lower on y axis, remove that element 
		if (pb->List[i].position.y < -4) {

      // if the particle hasnt bounced before 
      if (pb->List[i].num_b == 0) {
        pb->List[i].position.y = -3.8;
        pb->List[i].direction.y = 0.1 * (-1 * pb->List[i].direction.y);
        pb->List[i].num_b += 1;
        pb->List[i].size *= 0.5;
        //newParticleScatter(pb, pb->List[i]);
      }
      else {
        pb->List[i] = pb->List[pb->num_elements];
        pb->num_elements--;
      }
		}

		// recalculate direction due to gravity 
		pb->List[i].direction = gravity_motion(pb->List[i]);
		// recalculate position due to gravity 
		pb->List[i].position = update_vec(pb->List[i].position, pb->List[i].direction);
	}
}
