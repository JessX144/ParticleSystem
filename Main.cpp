#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include <GL\glew.h>
#include <GL\freeglut.h>
#include "particle.h" 
#include "emmitter.h"
#include "mathFunc.h"
#include <iostream>

GLuint axisList;

int AXIS_SIZE = 200;
int axisEnabled = 1;

using namespace std;
using namespace glm;	

int counter = 0;

ParticleBuffer pb;

void redisplay(int in) {
	glutPostRedisplay();
}

void display()
{

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 100.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);

	if (axisEnabled) glCallList(axisList);

	update_particles(pb);

	//cout << pb.size();

	for (int i = 0; i < pb.size(); i++)
	{
		//cout << "new";
		glPushMatrix();
		glTranslatef(pb[i].position.x, pb[i].position.y, pb[i].position.z);
		glutSolidSphere(pb[i].size, 20, 20);
		glPopMatrix();
	}

	glutSwapBuffers();

	// refreshes frame every 5ms after recalculate positions
	glutTimerFunc(5, redisplay, 0);
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 27) exit(0);
	glutPostRedisplay();
}

void reshape(int width, int height)
{
	glClearColor(0.9, 0.9, 0.9, 0.0);
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 10000.0);
	glMatrixMode(GL_MODELVIEW);
}

void makeAxes() {
	axisList = glGenLists(1);
	glNewList(axisList, GL_COMPILE);
	glLineWidth(2.0);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);     
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(AXIS_SIZE, 0.0, 0.0);
	glColor3f(0.0, 1.0, 0.0);       
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, AXIS_SIZE, 0.0);
	glColor3f(0.0, 0.0, 1.0);      
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, AXIS_SIZE);
	glEnd();
	glEndList();
}

void initGraphics(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutCreateWindow("COMP37111 Particles");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	makeAxes();
}

void newParticles() {
	counter++;
	// adds new particle every 4000th time function ran 
	// larger value = fewer particles
	if (counter % 4000 == 0) {
		newParticle(pb);
	}
	/*cout << pb.size();*/
}

int main(int argc, char *argv[])
{
	double f;
	srand(time(NULL));
	initGraphics(argc, argv);
	glEnable(GL_POINT_SMOOTH);
	glutIdleFunc(newParticles);
	glutMainLoop();
	return 0;
}