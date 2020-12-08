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
#include <unordered_map>

using namespace std;
using namespace glm;

#define PI 3.14159;
#define DEG_TO_RAD 0.017453293

// smallest distance for droplets to combine 
float smallest_dist = 0.01;

// represents a single cell/cell in 3d grid 
struct Cell {
  vec3 corner;
  // set of particles in the grid 
  set<Particle> cell_data;
};

// hash function for each cell in grid 
class Hash {
public:
  size_t operator()(const vec3& v) const
  {
    return std::hash<int>()(v.x) ^ std::hash<int>()(v.y) ^ std::hash<int>()(v.z);
  }
  bool operator()(const vec3& v1, const vec3& v2)const
  {
    return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
  }
};

// hashmap for cells forming grid in 3d space
// key is upper corner, value is cell itself 
unordered_map<vec3, Cell, Hash> grid;

// operator function for set of particles in each cell
bool operator<(const Particle & p1, const Particle & p2)
{
  return p1.position.y < p2.position.y;
}

// Every update it places the particle into a grid 
void class_particles(Particle *p) {

  // rounds coordinates up, to get the upper corner of the cell it should be in 
  float x = p->position.x < 0 ? floor(p->position.x) : ceil(p->position.x);
  float y = p->position.x < 0 ? floor(p->position.x) : ceil(p->position.x);
  float z = p->position.x < 0 ? floor(p->position.x) : ceil(p->position.x);

  // key is corner of cell
  vec3 pos = vec3(ceil(p->position.x), ceil(p->position.y), ceil(p->position.z));

  // Creates new cell if doesnt exist
  if (grid.count(pos) == 0) {
    set<Particle> celldata;
    Cell c = {pos, celldata};
    grid.insert({ pos, c });
  }

  Cell & cell = grid.at(pos);

  // when cell is too large, contains particles that have already left cell
  // clear particles list in grid
  if (cell.cell_data.size() > 10) {
    cell.cell_data.clear();
  }

  cell.cell_data.insert(*p);

  set<Particle>::iterator it;

  // https://aapt.scitation.org/doi/10.1119/1.4820848
  // https://vortex.plymouth.edu/precip/precip2aaa.html
  for (it = cell.cell_data.begin(); it != cell.cell_data.end(); ) {
    float dist = pow(vec_dist((*it).position, pos), 2);
    float sq = pow((*it).size + p->size, 2);
    // if particles are close and the probability is considered 
    // erases particle from set of particles in grid if has been combined
    if (dist < smallest_dist && is_prob(dist/ sq)) {
      // new radius once 2 droplets combine volume
      float r_sum = pow(p->size, 3) + pow((*it).size, 3);
      float new_rad = pow(r_sum, 1.0 / 3);

      // size of water droplet has upper bound, appropriately scaled for radius 
      if (new_rad < 0.15)
        p->size += (*it).size;
      it = cell.cell_data.erase(it);
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
// when particle hits ground, rebounds at velocity depending on coefficient of restitution  
void particleRebound(ParticleList *pb, Particle p, float gravity, float speed_fac, float coeff_of_rest) {

  vec4 water_colour = vec4(1.0, 1.0, 1.0, 0.2);

  // proportional to speed and size 
  // float rel_weber_num = pow(vec_size(p.velocity), 2) * 2 * p.size;
  // y = 0.04892x − 0.2601
  
  // given water sphere of radius 0.08, mass = volume of sphere, water has density 1000kg/cubic meter 
  float mass = pow(p.size, 3) * 4 / 3 * 1000 * PI;

  Particle new_p = { vec3(p.position.x, 0, p.position.z), water_colour, multVec(multVec(p.velocity, -1), coeff_of_rest), gravity, p.size, mass, 1 };

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

  p->velocity.y -= (drag * delta_time) / (p->mass);
  p->velocity.x -= (drag * delta_time) / (p->mass);
  p->velocity.z -= (drag * delta_time) / (p->mass);

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
        particleRebound(pb, pb->List[i], gravity, speed_fac, coeff_of_rest);
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
