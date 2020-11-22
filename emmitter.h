#ifndef EMMITTER_H
#define EMMITTER_H

#include "particle.h"
#include "mathFunc.h"

vec3 particle_direction();
float particle_speed();
void newParticle(ParticleList *pb);
vec3 gravity_motion(Particle &p);
//void newParticles(int counter, ParticleBuffer pb);
void update_particles(ParticleList *pb);

#endif