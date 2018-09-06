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

void write_pixel(int x, int y)
{
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

void displayLine()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1,1,1);
	glViewport(0, 0, 400, 300);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-200.0, 200.0, -150.0, 150.0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	int x1 = 0;
	int y1 = 0;
	int x2 = 100;
	int y2 = 50;
	int dx = x2 - x1;
	int dy = y2 - y1;
	int d = 2 * dy - dx;
	int incE = 2 * dy;
	int incNE = 2 * (dy - dx);
	
	int x = x1;
	int y = y1;
	write_pixel(x,y);
	while(x < x2)
	{
		if (d <= 0)
		{
			d += incE;
			x += 1;
		}
		else
		{
			d += incNE;
			x += 1;
			y += 1;
		}
		write_pixel(x,y);
	}
	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 300);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Display Line");
	init();
	glutDisplayFunc(displayLine);
	glutMainLoop();
	return 0;
}