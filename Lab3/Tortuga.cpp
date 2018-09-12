#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include <math.h>

using namespace std;

void reshape(int width, int height) 
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)height / (GLfloat)width, 1.0, 128.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(2.5, 3.0, 1.5, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y) 
{
	switch (key) 
	{
	case '1':
		glRotatef(1.0,1.,0.,0.);
		break;
	case '2':
		glRotatef(1.0,-1.,0.,0.);
		break;
	case '3':
		glRotatef(1.0,0.,1.,0.);
		break;
	case '4':
		glRotatef(1.0,0.,-1.,0.);
		break;
	case '5':
		glRotatef(1.0,0.,0.,1.);
		break;
	case '6':
		glRotatef(1.0,0.,0.,-1.);
		break;
	case 'w':
		glTranslatef(-0.1,0.,0.);
		break;
	case 'a':
		glTranslatef(0.,0.,0.1);
		break;
	case 's':
		glTranslatef(0.1,0.,0.);
		break;
	case 'd':
		glTranslatef(0.,0.,-0.1);
		break;
	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void drawXZAxes()
{
	glColor3f(0.0,0.0,1.0);

	glBegin(GL_LINES);

	glVertex3f(0, 0, -3);
	glVertex3f(0, 0, 3);
	glVertex3f(-3, 0, 0);
	glVertex3f(3, 0, 0);

	glEnd();
}

void displayTortuga()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//drawXZAxes();

	glColor3f(1.0,0.0,0.0);

	int npoints = 21;
	float x[] = {0.0, 0.1, 0.3,  0.5, 0.6,  0.8, 0.75,  0.7, 0.55, 0.7, 0.95, 1.0, 1.25, 1.45, 1.6, 1.85,  2.1, 2.15,  2.0,  1.8, 1.7};
	float z[] = {0.0, 0.3, 0.5, 0.45, 0.3, 0.35, 0.55, 0.55, 0.75, 0.9, 0.95, 0.7, 0.95,  1.0, 0.7, 1.10, 0.95,  0.8, 0.65, 0.55, 0.0};

	glBegin(GL_LINE_LOOP);

	for (int i=0; i < npoints; i++)
	{
		glVertex3f(x[i], 0, z[i]);
	}

	for (int i=npoints-1; i >=0; i--)
	{
		glVertex3f(x[i], 0, -z[i]);
	}

	glEnd();

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 300);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tortuga");
	glutDisplayFunc(displayTortuga);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}

/*
- Para dibujar la tortuga, se utilizaron múltiples vértices usando la función GL_LINE_LOOP
- Solo se calcularon 21 vértices para la mitad de la tortuga. Para la otra mitad, se usó simetría respecto al eje X.
- Se pueden utilzar las teclas 'w', 'a', 's' y 'd' para avanzar, ir a la izquierda, retroceder e ir a la derecha, respectivamente.
- Se pueden usar las teclas del 1 al 6 para rotar a la tortuga respecto a los diferentes ejes.
- Para las traslaciones, se usó la función glTranslatef(x, y, z).
- Para las rotaciones, se usó la función glRotatef(degree, x, y, z).
*/