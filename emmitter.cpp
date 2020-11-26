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

void newParticle(ParticleList *pb) {

	vec3 origin = vec3(0.0, 0.0, 0.0);
	vec4 water_colour = vec4(0.0, 0.0, 1.0, 0.8);
	Particle p = { origin, water_colour, myRandomSpeed(), -9.8, 0.02, 0};
	
	if (pb->num_elements > pb->max_size) {
    pb->List[0] = pb->List[pb->num_elements];
    pb->num_elements--;
	}

	pb->num_elements++;
	pb->List[pb->num_elements] = p;

}

void newParticleScatter(ParticleList *pb, Particle p) {

  vec4 water_colour = vec4(0.0, 0.0, 1.0, 0.8);
  for (int i = 0; i < 5; i++) {
    Particle new_p = { vec3(p.position.x, 0, p.position.z), water_colour, multVec(myRandomSpeed(), 0.1), -9.8, p.size *= 0.5, 1 };
    
    if (pb->num_elements > pb->max_size) {
      pb->List[0] = pb->List[pb->num_elements];
      pb->num_elements--;
    }

    pb->num_elements++;
    pb->List[pb->num_elements] = new_p;
  }

}

// How particle effected by gravity 
void gravity_motion(Particle *p) {

  p->velocity.y += p->acceleration * 0.01;
  p->position.x += p->velocity.x* 0.01;
  p->position.z += p->velocity.z* 0.01;
  p->position.y += p->velocity.y* 0.01;
  
}

void update_particles(ParticleList *pb) {

	for (int i = 0; i < pb->num_elements; i++) {
    if (pb->List[i].num_b == 1 && pb->List[i].position.y <= 0) {
      pb->List[i] = pb->List[pb->num_elements];
      pb->num_elements--;
    }
		// if fallen lower on y axis, remove that element 
		if (pb->List[i].position.y < 0) {

      // if the particle hasnt bounced before 
      if (pb->List[i].num_b == 0) {
        newParticleScatter(pb, pb->List[i]);
        pb->List[i] = pb->List[pb->num_elements];
        pb->num_elements--;
      }
      else {
        pb->List[i] = pb->List[pb->num_elements];
        pb->num_elements--;
      }

      pb->List[i] = pb->List[pb->num_elements];
      pb->num_elements--;
		}

		// recalculate position due to gravity 
    gravity_motion(&pb->List[i]);

	}
}
