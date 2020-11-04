#include <GL\glew.h>
#include <GL\freeglut.h>

void displayMe(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POLYGON);
	glVertex2f(0.0, 0.0);                 
	glVertex2f(0.5, 0.0);                  
	glVertex2f(0.5, 0.5);                  
	glVertex2f(0.0, 0.5);                  
	glEnd();
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(300, 300);                
	glutInitWindowPosition(500, 500);             
	glutCreateWindow("Hello world");   
	glutDisplayFunc(displayMe);
	glutMainLoop();
	return 0;
}