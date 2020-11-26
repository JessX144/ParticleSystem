#ifndef EMMITTER_H
#define EMMITTER_H

#include "particle.h"
#include "mathFunc.h"

void newParticle(ParticleList *pb);
void gravity_motion(Particle *p);
void update_particles(ParticleList *pb);

#endif