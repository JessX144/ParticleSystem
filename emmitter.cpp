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

const int MaxParticles = 10000;

#define DEG_TO_RAD 0.017453293

void newParticle(ParticleList *pb, float gravity, float speed_fac, int num_levels, float r) {

  vec3 emmitter_shape = circularPos(r);
  vec4 water_colour = vec4(1.0, 1.0, 1.0, 0.5);

  float level_fac = 1 + num_levels * 0.2;

	Particle p = { emmitter_shape, water_colour, myRandomSpeed(speed_fac * level_fac, num_levels), gravity, 0.08, 0};
	
	if (pb->num_elements > pb->max_size) {
    pb->List[0] = pb->List[pb->num_elements];
    pb->num_elements--;
	}

	pb->num_elements++;
	pb->List[pb->num_elements] = p;

}

void newParticleScatter(ParticleList *pb, Particle p, float gravity, float speed_fac) {

  vec4 water_colour = vec4(1.0, 1.0, 1.0, 0.5);
  for (int i = 0; i < 5; i++) {
    Particle new_p = { vec3(p.position.x, 0, p.position.z), water_colour, multVec(myRandomSpeed(speed_fac, 1), 0.1), gravity, p.size *= 0.5, 1 };
    
    if (pb->num_elements > pb->max_size) {
      pb->List[0] = pb->List[pb->num_elements];
      pb->num_elements--;
    }

    pb->num_elements++;
    pb->List[pb->num_elements] = new_p;
  }

}

// How particle effected by gravity 
void gravity_motion(Particle *p, float delta_time) {
  // milliseconds 
  delta_time /= 1000;
  p->velocity.y += p->acceleration * delta_time;
  p->position.x += p->velocity.x* delta_time;
  p->position.z += p->velocity.z* delta_time;
  p->position.y += p->velocity.y* delta_time;
  
}

void update_particles(ParticleList *pb, float gravity, float delta_time, float speed_fac) {

	for (int i = 0; i < pb->num_elements; i++) {

		// if fallen lower on y axis, remove that element 
		if (pb->List[i].position.y < 0) {

      // if the particle hasnt bounced before 
      if (pb->List[i].num_b == 0) {
        newParticleScatter(pb, pb->List[i], gravity, speed_fac);
      }

      pb->List[i] = pb->List[pb->num_elements];
      pb->num_elements--;
		}

		// recalculate position due to gravity 
    gravity_motion(&pb->List[i], delta_time);

	}
}
