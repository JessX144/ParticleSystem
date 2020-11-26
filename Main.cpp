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

float emit_rate = 0.8;

int AXIS_SIZE = 200;
int axisEnabled = 1;
const int MaxParticles = 10000;

GLfloat  eyex, eyey, eyez;   

int height = 800;
int width = 600;

using namespace std;
using namespace glm;	

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

void newParticles(int i) {
  newParticle(&pb);
  update_particles(&pb);
  glutPostRedisplay();
  glutTimerFunc(emit_rate, newParticles, 0);
}

void special(int key, int xx, int yy) {

	float deg = 0;
	float original_x = eyex;
	float original_z = eyez;
	float original_y = eyey;

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
      if (eyez <= 200) {
        eyez += cos(0 * DEG_TO_RAD)*ZOOM_SPEED;
      }
			break;
		case GLUT_KEY_DOWN:
      if (eyez >= 20) {
        eyez -= cos(0 * DEG_TO_RAD)*ZOOM_SPEED;
      }
			break;
	}

}

void show_text() {
  unsigned char string[] = "test";
  int w;
  w = glutBitmapLength(GLUT_BITMAP_8_BY_13, string);
  float x = .5; /* Centre in the middle of the window */
  glRasterPos2f(x - (float)width / 2, 0.);
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

	for (int i = 0; i < pb.num_elements; i++)
	{
		glPushMatrix();
		glTranslatef(pb.List[i].position.x, pb.List[i].position.y, pb.List[i].position.z);
		glutWireSphere(pb.List[i].size, 20, 20);
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
	eyez = 20.0;

	createArray();

	glutInit(&argc, argv);
	glutInitWindowSize(height, width);
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
  glutTimerFunc(emit_rate, newParticles, 0);
	glutMainLoop();
	free(pb.List);
	return 0;
}