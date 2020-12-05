#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

#include <GL\glew.h>
#include <GL\freeglut.h>
#include "mathFunc.h"
#include "particle.h" 
#include "emmitter.h"

#include <set>

#include <unordered_set>
#include <unordered_map>

using namespace std;
using namespace glm;

const int MaxParticles = 10000;
#define PI 3.14159;
#define DEG_TO_RAD 0.017453293

struct Cube {
  vec3 corner;
  // set of particles, as you cant add the same particle more than once 
  set<Particle> cell_data;
  //Particle cell_data[30];
};

class Hash {
public:
  // Returns hash value for argument 
  size_t operator()(const vec3& v) const
  {
    return std::hash<int>()(v.x) ^ std::hash<int>()(v.y) ^ std::hash<int>()(v.z);
  }
  bool operator()(const vec3& v1, const vec3& v2)const
  {
    return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
  }
};

// corner of grid and particles within grid 
unordered_map<vec3, Cube, Hash> grid;

float smallest_dist = 0.03;

bool operator<(const Particle & p1, const Particle & p2)
{
  return p1.position.y < p2.position.y;
}

// Each update it places a particle into a grid 
void class_particles(Particle *p) {

  float x = p->position.x < 0 ? floor(p->position.x) : ceil(p->position.x);
  float y = p->position.x < 0 ? floor(p->position.x) : ceil(p->position.x);
  float z = p->position.x < 0 ? floor(p->position.x) : ceil(p->position.x);

  // key is corner of cube 
  vec3 pos = vec3(ceil(p->position.x), ceil(p->position.y), ceil(p->position.z));

  // Creates new cube if doesnt exist
  if (grid.count(pos) == 0) {
    set<Particle> celldata;
    Cube c = {pos, celldata};
    grid.insert({ pos, c });
  }

  Cube & cells = grid.at(pos);

  if (cells.cell_data.size() > 20) {
    cells.cell_data.clear();
  }

  // set does not contain duplicate 
  cells.cell_data.insert(*p);

  set<Particle>::iterator it;

  // https://aapt.scitation.org/doi/10.1119/1.4820848
  for (it = cells.cell_data.begin(); it != cells.cell_data.end(); ) {
    if (vec_dist((*it).position, pos) < smallest_dist) {
      /*p->colour = vec4(1.0, 0.0, 0.0, 1.0);*/
      if (p->size + (*it).size < 0.5)
        p->size += (*it).size;
      it = cells.cell_data.erase(it);
    }
    else {
      it++;
    }
  }
}

void newParticle(ParticleList *pb, float gravity, float speed_fac, int num_levels, float r, float radius) {

  vec3 emmitter_shape = circularPos(r);
  vec4 water_colour = vec4(1.0, 1.0, 1.0, 0.2);

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

// https://royalsocietypublishing.org/doi/pdf/10.1098/rspa.1964.0161
void newParticleScatter(ParticleList *pb, Particle p, float gravity, float speed_fac, float coeff_of_rest) {

  vec4 water_colour = vec4(1.0, 1.0, 1.0, 0.2);

  // proportional to speed and size 
  // float rel_weber_num = pow(vec_size(p.velocity), 2) * 2 * p.size;
  // y = 0.04892x − 0.2601
  
  // given water sphere of radius 0.08, mass = volume of sphere, water has density 1000kg/cubic meter 
  float mass = pow(p.size, 3) * 4 / 3 * 1000 * PI;

  Particle new_p = { vec3(p.position.x, 0, p.position.z), water_colour, multVec(myRandomSpeed(coeff_of_rest, 1), 0.1), gravity, p.size, mass, 1 };

  if (pb->num_elements > pb->max_size) {
    pb->List[0] = pb->List[pb->num_elements];
    pb->num_elements--;
  }

  pb->num_elements++;
  pb->List[pb->num_elements] = new_p;

}

// How particle effected by gravity 
// 2 forces: gravity and air resistance 
void update_motion(Particle *p, float delta_time) {
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

void update_particles(ParticleList *pb, float gravity, float delta_time, float speed_fac, float coeff_of_rest, bool class_p) {

	for (int i = 0; i < pb->num_elements; i++) {

		// if fallen lower on y axis, remove that element 
		if (pb->List[i].position.y < 0) {

      // if the particle hasnt bounced before 
      if (pb->List[i].num_b == 0) {
        newParticleScatter(pb, pb->List[i], gravity, speed_fac, coeff_of_rest);
      }
      // instead of removing the element in its position and shifting, swap element with last 
      // and discount last element from being considered
      pb->List[i] = pb->List[pb->num_elements];
      pb->num_elements--;
		}

		// recalculate position due to gravity 
    update_motion(&pb->List[i], delta_time);
    if (class_p)
      class_particles(&pb->List[i]);
	}
}
