#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include <math.h>

using namespace std;

void init()
{
	glClearColor(0,0,0,0);
}

void displayPolygon()
{
	int sides = 8;
	double radius = 0.5;
	double angle = 2*M_PI/sides;

	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1,1,1);
	glLoadIdentity();

	glBegin(GL_LINES);
	for (int i = 0; i < sides; i++)
	{
		double current_angle = i*angle;
		double next_angle = (i+1)*angle;
		glVertex2f(cos(current_angle)*radius,sin(current_angle)*radius);
		glVertex2f(cos(next_angle)*radius,sin(next_angle)*radius);
		/*cout << "Curr: " << current_angle << endl;
		cout << "Next: " << next_angle << endl;*/
	}
	glEnd();

	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 300);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Hello World");
	init();
	glutDisplayFunc(displayPolygon);
	glutMainLoop();
	return 0;
}