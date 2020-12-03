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
#define PI 3.14159;
#define DEG_TO_RAD 0.017453293

void newParticle(ParticleList *pb, float gravity, float speed_fac, int num_levels, float r) {

  vec3 emmitter_shape = circularPos(r);
  vec4 water_colour = vec4(1.0, 1.0, 1.0, 0.5);
  // in meters 
  float radius = 0.08;
  // given water sphere of radius 0.08, mass = volume of sphere, water has density 1000kg/cubic m
  float mass = pow(radius, 3) * 4 / 3 * 1000 * PI;

  float level_fac = 1 + num_levels * 0.2;
	Particle p = { emmitter_shape, water_colour, myRandomSpeed(speed_fac * level_fac, num_levels), gravity, radius, mass, 0};
	
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
    // in meters 
    float radius = p.size * 0.5;
    // given water sphere of radius 0.08, mass = volume of sphere, water has density 1000kg/cubic meter 
    float mass = pow(radius, 3) * 4 / 3 * 1000 * PI;
    
    Particle new_p = { vec3(p.position.x, 0, p.position.z), water_colour, multVec(myRandomSpeed(speed_fac, 1), 0.1), gravity, radius, mass, 1 };
    
    if (pb->num_elements > pb->max_size) {
      pb->List[0] = pb->List[pb->num_elements];
      pb->num_elements--;
    }

    pb->num_elements++;
    pb->List[pb->num_elements] = new_p;
  }

}

// How particle effected by gravity 
// 2 forces: gravity and air resistance 
void gravity_motion(Particle *p, float delta_time) {
  // milliseconds 
  delta_time /= 1000;

  // p size is represented in meters 
  float area = pow(p->size, 2) * PI ;
  // force due to drag - density of air taken to be 1 
  // drag coefficient varies based on parameters, sphere in air is typically 0.45
  float drag = 0.5 * pow(p->velocity.y, 2) * 0.45 * area;

  p->velocity.y += p->acceleration * delta_time;
  // force = change in mass*change_in_velocty/time
  p->velocity.y -= (drag * delta_time) / (p->mass);

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
      // instead of removing the element in its position and shifting, swap element with last 
      // and discount last element from being considered
      pb->List[i] = pb->List[pb->num_elements];
      pb->num_elements--;
		}

		// recalculate position due to gravity 
    gravity_motion(&pb->List[i], delta_time);

	}
}
