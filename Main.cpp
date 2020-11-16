#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include <GL\glew.h>
#include <GL\freeglut.h>
#include "particle.h" 
#include <vector>

GLuint axisList;

int AXIS_SIZE = 200;
int axisEnabled = 1;

using namespace std;
using namespace glm;	

const int MaxParticles = 100000;

// Vector to store particles 
vector<Particle> particles;

double myRandom()
{
	return (rand() / (double)RAND_MAX);
}

// calculates random particle initial direction 
vec3 particle_direction() {
	// range [-0.2, 0.2]
	float x = -0.2 + 0.4 * myRandom();
	float y = 2 + 1 * myRandom();
	float z = -0.2 + 0.4 * myRandom();

	vec3 dir = vec3(x, y, z);

	return dir;
}

float particle_speed() {
	// range betweek [0, 1]
	return myRandom();
}

void newParticle() {
	vec3 origin = vec3(0.0, 0.0, 0.0);
	vec4 water_colour = vec4(0.0, 0.0, 1.0, 0.8);
	Particle p = { origin, water_colour, particle_direction(), particle_speed(), 0.2 };
	particles.push_back(p);
}

void display()
{
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 2.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
	glClearColor(0.5, 0.5, 0.5, 0.5);
	glClear(GL_COLOR_BUFFER_BIT);

	if (axisEnabled) glCallList(axisList);
	newParticle();
	glPointSize(10);
	glBegin(GL_POINTS);
	
	for (int i = 0; i < 1; i++)
	{

		glColor3f(particles[i].colour[0], particles[i].colour[1], particles[i].colour[2]);
		glVertex3f(particles[i].position[0], particles[i].position[1], particles[i].position[2]);
	
	}

	glEnd();
	glutSwapBuffers();
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

int main(int argc, char *argv[])
{
	double f;
	srand(time(NULL));
	initGraphics(argc, argv);
	glEnable(GL_POINT_SMOOTH);
	glutMainLoop();
	return 0;
}