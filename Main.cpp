#pragma once

#include <GL\glew.h>
#include <GL\freeglut.h>
#include "particle.h" 
#include <vector>

using namespace std;	

const int MaxParticles = 100000;

// Array to store particles 
// Particle Particles[MaxParticles];

// Vector to store particles 
vector<Particle> particles;

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POLYGON);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.5, 0.0);
	glVertex2f(0.5, 0.5);
	glVertex2f(0.0, 0.5);
	glEnd();
	glFlush();
}

void newParticle() {
	Particle p;
	particles.push_back(p);
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Particle System");
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}