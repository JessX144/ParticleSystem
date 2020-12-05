#ifndef EMMITTER_H
#define EMMITTER_H

#include "particle.h"
#include "mathFunc.h"

void newParticle(ParticleList *pb, float gravity, float speed, int levels, float emmitter_radius, float radius);
void update_motion(Particle *p, float delta_time);
void update_particles(ParticleList *pb, float gravity, float delta_time, float speed_fac, float coeff_of_rest, bool c_p);

#endif