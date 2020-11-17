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

#define DEG_TO_RAD 0.017453293
#define ZOOM_SPEED  10.0

GLuint axisList;

int AXIS_SIZE = 200;
int axisEnabled = 1;
int MAX_PARTICLES = 10000;

GLfloat  eyex, eyey, eyez;   

using namespace std;
using namespace glm;	

int counter = 0;

ParticleBuffer pb;

void redisplay(int in) {
	glutPostRedisplay();
}

void newParticles() {
	counter++;
	newParticle(pb);
	newParticle(pb);
	newParticle(pb);
	newParticle(pb);
	glutPostRedisplay();
}

void special(int key, int xx, int yy) {

	float deg = 0;
	float original_x = eyex;
	float original_z = eyez;
	float original_y = eyey;
	float fraction = 0.1f;

	switch (key) {
		case GLUT_KEY_LEFT:
			deg += 1;
			eyex = cos(deg*DEG_TO_RAD) * original_x + sin(deg*DEG_TO_RAD) * original_z;
			eyez = -sin(deg*DEG_TO_RAD) * original_x + cos(deg*DEG_TO_RAD) * original_z;
			break;
		case GLUT_KEY_RIGHT:
			deg += 1;
			eyex = cos(deg*DEG_TO_RAD) * original_x - sin(deg*DEG_TO_RAD) * original_z;
			eyez = sin(deg*DEG_TO_RAD) * original_x + cos(deg*DEG_TO_RAD) * original_z;
			break;
		case GLUT_KEY_UP:
			cout << "\n";
			cout << eyez;
			eyez += cos(0*DEG_TO_RAD)*ZOOM_SPEED;
			break;
		case GLUT_KEY_DOWN:
			eyez -= cos(0*DEG_TO_RAD)*ZOOM_SPEED;
			break;
	}

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyex, eyey, eyez,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);

	if (axisEnabled) glCallList(axisList);

	update_particles(pb);

	for (int i = 0; i < pb.size(); i++)
	{
		glPushMatrix();
		glTranslatef(pb[i].position.x, pb[i].position.y, pb[i].position.z);
		glutSolidSphere(pb[i].size * 3, 20, 20);
		glPopMatrix();
	}

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 27) exit(0);
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
	eyex = 0.0;
	eyey = 1.7;
	eyez = 100.0;

	glutInit(&argc, argv);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutCreateWindow("COMP37111 Particles");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutReshapeFunc(reshape);
	makeAxes();
}

int main(int argc, char *argv[])
{
	double f;
	srand(time(NULL));
	initGraphics(argc, argv);
	glEnable(GL_POINT_SMOOTH);
	glutIdleFunc(newParticles);
	// calls display repeatedly, calling idle inbetween 
	glutMainLoop();
	return 0;
}