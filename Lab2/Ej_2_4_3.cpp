#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include <math.h>

#define WIDTH 400
#define HEIGHT 300

using namespace std;

void init()
{
	glClearColor(0,0,0,0);
}

void move_to(int dcx, int dcy, int &posx, int &posy)
{
	posx = dcx;
	posy = dcy;
}

void draw_to(int dcx, int dcy, int &posx, int &posy)
{
	glBegin(GL_LINES);
	glVertex2i(posx, posy);
	glVertex2i(dcx, dcy);
	glEnd();

	posx = dcx;
	posy = dcy;
}

void pen_up(bool &pen)
{
	pen = 0;
}

void pen_down(bool &pen)
{
	pen = 1;
}

void locate(int dcx, int dcy, int &posx, int &posy, bool pen)
{
	if (pen) draw_to(dcx, dcy, posx, posy);
	else move_to(dcx, dcy, posx, posy);
}

void displayPen()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1,1,1);
	glViewport(0, 0, WIDTH, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WIDTH, 0, HEIGHT, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	bool pen = 0;
	int posx = 0;
	int posy = 0;

	//Empezar a dibujar
	pen_down(pen);
	locate(100, 100, posx, posy, pen);
	pen_up(pen);
	locate(100, 0, posx, posy, pen);
	pen_down(pen);
	locate(200, 100, posx, posy, pen);
	pen_up(pen);
	locate(200, 0, posx, posy, pen);
	pen_down(pen);
	locate(300, 100, posx, posy, pen);
	pen_up(pen);
	locate(300, 0, posx, posy, pen);
	pen_down(pen);
	locate(400, 100, posx, posy, pen);
	//Terminar de dibujar

	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Pen");
	init();
	glutDisplayFunc(displayPen);
	glutMainLoop();
	return 0;
}