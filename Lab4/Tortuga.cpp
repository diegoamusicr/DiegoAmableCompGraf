#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include <math.h>

using namespace std;

int np = 0;
float px[10000];
float py[10000];
float pz[10000];
GLdouble mModel[16];

void reshape(int width, int height) 
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)height / (GLfloat)width, 1.0, 128.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	gluLookAt(5.5, 8.0, 4.5, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glGetDoublev(GL_MODELVIEW_MATRIX, mModel);
	glPopMatrix();
}

void addPointToTrace()
{
	int i;
	GLdouble m[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, m);
	if (np == 0)
	{
		px[0] = 0;
		py[0] = 0;
		pz[0] = 0;
		np++;
	}
	px[np] = m[0] * px[0] + m[4] * py[0] +  m[8] * pz[0] + m[12];
	py[np] = m[1] * px[0] + m[5] * py[0] +  m[9] * pz[0] + m[13];
	pz[np] = m[2] * px[0] + m[6] * py[0] + m[10] * pz[0] + m[14];
	cout << "Punto: " << px[np] << " " << py[np] << " " << pz[np] << endl;
	np++;
}

void keyboard(unsigned char key, int x, int y) 
{
	glPushMatrix();
	glLoadIdentity();
	glMultMatrixd(mModel);
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
		addPointToTrace();
		break;
	case 'a':
		glTranslatef(0.,0.,0.1);
		addPointToTrace();
		break;
	case 's':
		glTranslatef(0.1,0.,0.);
		addPointToTrace();
		break;
	case 'd':
		glTranslatef(0.,0.,-0.1);
		addPointToTrace();
		break;
	case 'q':
		exit(0);
		break;
	}
	glGetDoublev(GL_MODELVIEW_MATRIX, mModel);
	glPopMatrix();
	glutPostRedisplay();
}

void drawAxes()
{
	glColor3f(1.0,0.0,0.0);

	glBegin(GL_LINES);
	
	glVertex3f(0, 0, 0);
	glVertex3f(10, 0, 0);

	glEnd();

	glColor3f(0.0,1.0,0.0);

	glBegin(GL_LINES);
	
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10, 0);

	glEnd();

	glColor3f(0.0,0.0,1.0);

	glBegin(GL_LINES);

	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 10);

	glEnd();
}

void drawTortuga()
{
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
}

void displayTrace()
{
	int i;
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINE_STRIP);

	for (i = 0; i < np; i++)
	{
		glVertex3f(px[i], py[i], pz[i]);
	}

	glEnd();
}

void displayTortuga()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glMultMatrixd(mModel);
	drawTortuga();
	glPopMatrix();
	displayTrace();
	drawAxes();
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
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