#ifndef EMMITTER_H
#define EMMITTER_H

#include "particle.h"
#include "mathFunc.h"

void newParticle(ParticleList *pb, float gravity, float speed, int levels);
void gravity_motion(Particle *p, float delta_time);
void update_particles(ParticleList *pb, float gravity, float delta_time, float speed_fac);
void styleParticle(ParticleList *pb, float gravity, float speed_fac, int num_levels);

#endif