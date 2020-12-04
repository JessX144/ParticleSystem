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
#include <string>

#define DEG_TO_RAD 0.017453293
#define ZOOM_SPEED  10.0

GLuint axisList;

float old_time = 0;
float delta_time = 0;
int counter = 0;
int AXIS_SIZE = 200;
int axisEnabled = 1;
GLfloat  eyex, eyey, eyez;
int height = 800;
int width = 600;
int fps = 0;
int framesPerSec = 0.0;
static float lastTime = 0.0f;
int space_c = 0;

// MODIFIABLE VARIABLES
int emit_rate = 1;
int num_levels = 1;
const int MaxParticles = 1000;
float gravity = -9.8;
float speed_fac = 1;
float emmitter_r = 0;
int num_p = 1;
float coeff_of_rest = 0.2;

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

void newParticles() {
  // time since elapsed 
  float sec = glutGet(GLUT_ELAPSED_TIME);
  // time since last newParticles has been called 
  delta_time = sec - old_time;
  old_time = sec;

  counter++;

  // larger emit rate, fewer particles emmitted 
  // allows us to get slower rate than base rate of idle func
  if (counter%emit_rate == 0) {
    for (int j = 0; j < num_p; j++) {
      for (int i = 1; i <= num_levels; i++) {
        newParticle(&pb, gravity, speed_fac, i, emmitter_r);
      }
    }
  }

  update_particles(&pb, gravity, delta_time, speed_fac, coeff_of_rest);
  glutPostRedisplay();
}

// called every display/frame 
void getFrameRate()
{
  float currentTime = GetTickCount() * 0.001f;
  ++fps;
  if (currentTime - lastTime >= 1.0f)
  {
    framesPerSec = fps / (currentTime - lastTime);
    lastTime = currentTime;
    fps = 0;
  }
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
		case GLUT_KEY_DOWN:
      if (eyez > 10) {
        eyez -= cos(0 * DEG_TO_RAD)*ZOOM_SPEED;
      }
			break;
		case GLUT_KEY_UP:
      if (eyez <= 40) {
        eyez += cos(0 * DEG_TO_RAD)*ZOOM_SPEED;
      }
			break;
	}

}

void show_text() {
  char* c;
  GLint matrixMode;

  int lineHeight = glutBitmapHeight(GLUT_BITMAP_HELVETICA_12) / 24;
  char g[30], e[30], m[30], s[30], f[30], r[30], n[30];
  // default values are 1 
  float particle_scale_fac = (float)num_p / (float)emit_rate;
  sprintf_s(g, "gravity: %f", gravity);
  sprintf_s(m, "max particles: %d", MaxParticles);
  sprintf_s(s, "speed factor: %f", speed_fac);
  sprintf_s(r, "emmitter radius: %f", emmitter_r);
  sprintf_s(n, "num particles scale: %f", particle_scale_fac);
  sprintf_s(f, "fps: %d", framesPerSec);

  char * str[6] = { g, m, s, r, n, f };

  glGetIntegerv(GL_MATRIX_MODE, &matrixMode);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0.0, 1.0, 0.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glPushAttrib(GL_COLOR_BUFFER_BIT);   
  glColor3f(1, 1, 1);
  glRasterPos3f(0.05, 0.1, 0.0);
  for (int i = 0; i < 6; i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 10);
    for (c = str[i]; *c; c++) {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, (int)*c);
    }
  }

  glPopAttrib();
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(matrixMode);
}

void show_text_old()
{
  int lineHeight = glutBitmapHeight(GLUT_BITMAP_HELVETICA_18)/20;
  char g[30], e[30], m[30], s[30], f[30], r[30], n[30];
  char *c;

  // default values are 1 
  float particle_scale_fac = (float)num_p / (float)emit_rate;

  sprintf_s(g, "gravity: %f", gravity);
  sprintf_s(m, "max particles: %d", MaxParticles);
  sprintf_s(s, "speed factor: %f", speed_fac);
  sprintf_s(r, "emmitter radius: %f", emmitter_r);
  sprintf_s(n, "num particles scale: %f", particle_scale_fac);
  sprintf_s(f, "fps: %d", framesPerSec);

  char * str[6] = { g, m, s, r, n, f };

  glColor3f(1.0, 1.0, 1.0);
  glRasterPos3f(2, 2, 2);

  for (int i = 0; i < 6; i++) {
    glRasterPos3f(2, 2 + i * lineHeight, 2);
    for (c = str[i]; *c != '\0'; c++)
    {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
  }

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyex, eyey, eyez,
		0.0, 4.0, 0.0,
		0.0, 1.0, 0.0);

  getFrameRate();
  show_text();

	if (axisEnabled) glCallList(axisList);

	for (int i = 0; i < pb.num_elements; i++)
	{
		glPushMatrix();
		glTranslatef(pb.List[i].position.x, pb.List[i].position.y, pb.List[i].position.z);
    glColor4f(pb.List[i].colour[0], pb.List[i].colour[1], pb.List[i].colour[2], pb.List[i].colour[3]);
    // Quads seem to have no difference on the speed to render 
    // Makes sphere of radius size
    glutSolidSphere(pb.List[i].size, 20, 20);
		glPopMatrix();
	}
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
    case 27:
      exit(0);
      break;
    case '1':
      if (gravity < -1)
        gravity += 1;
      break;
    case '2':
      gravity -= 1;
      break;
    case '3':
      if (emit_rate > 1)
        emit_rate -= 1;
      else
        num_p += 1;
      cout << "emit rate: ";
      cout << emit_rate;
      cout << "num p: ";
      cout << num_p;
      break;
    case '4':
      if (num_p > 1)
        num_p -= 1;
      else
        emit_rate += 1;
      cout << "emit rate: ";
      cout << emit_rate;
      cout << "num p: ";
      cout << num_p;
      break;
    case '5':
      if (speed_fac > 0.1)
        speed_fac -= 0.1;
      break;
    case '6':
      speed_fac += 0.1;
      break;
    case '7':
      if (num_levels > 1)
        num_levels -= 1;
      break;
    case '8': 
      if (num_levels <= 2)
        num_levels += 1;
      break;
    case ' ':
      space_c++;
      if (space_c == 1)
        glutIdleFunc(newParticles);
      else {
        free(pb.List);
        createArray();
      }
      break;
    case '0':
      emmitter_r += 0.1;
      break;
    case '9':
      if (emmitter_r > 0.1)
        emmitter_r -= 0.1;
      break;
    case 'd':
      if (coeff_of_rest >= 0.1)
        coeff_of_rest -= 0.1;
      break;
    case 's':
      //if (coeff_of_rest <= 0.9)
      coeff_of_rest += 0.1;
      break;
  } 
}

void reshape(int width, int height)
{
	glClearColor(0.1, 0.1, 0.1, 0.0);
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
	eyey = 0.0;
	eyez = 10.0;

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
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutMainLoop();
	free(pb.List);
	return 0;
}