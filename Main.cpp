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
const int MaxParticles = 10000;

GLfloat  eyex, eyey, eyez;   

using namespace std;
using namespace glm;	

int counter = 0;

//ParticleBuffer pb;
ParticleList pb;

void createArray() {
	pb.List = (Particle*)malloc(MaxParticles * sizeof(Particle));
	pb.max_size = MaxParticles;
	pb.num_elements = 0;

	if (pb.List == NULL) {
		printf("Memory not allocated.\n");
		exit(0);
	}
}

void newParticles() {
	counter++;
  newParticle(&pb);
  newParticle(&pb);
  newParticle(&pb);
  if (counter % 3 == 0) {
    update_particles(&pb);
    glutPostRedisplay();
  }
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
		case GLUT_KEY_UP:;
			eyez += cos(0*DEG_TO_RAD)*ZOOM_SPEED;
			break;
		case GLUT_KEY_DOWN:
			eyez -= cos(0*DEG_TO_RAD)*ZOOM_SPEED;
			break;
	}

}

void explode(Particle p) {
  for (int i = 0; i < 4200; i++) {
    vec3 direction(2 * (1 - (rand() % 200 / 100.0)), 1 * (rand() % 300 / 100.0), 2 * ((1 - rand() % 200 / 100.0)));

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

  // poly

	for (int i = 0; i < pb.num_elements; i++)
	{
		glPushMatrix();
		glTranslatef(pb.List[i].position.x, pb.List[i].position.y, pb.List[i].position.z);
		glutWireSphere(pb.List[i].size * 2, 20, 20);
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

	createArray();

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
	// must close using proper exit, else error
	free(pb.List);
	return 0;
}