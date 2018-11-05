#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include <math.h>

#include "camera.h"
#include "vector_tools.h"

static camera *LOCAL_MyCamera;
static int old_x, old_y;

using namespace std;

int np = 0;
float px[10000];
float py[10000];
float pz[10000];
GLdouble mModel[16];

void reshape(int width, int height) 
{
	glViewport(0, 0, width, height);
	SetGLAspectRatioCamera( LOCAL_MyCamera );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)height / (GLfloat)width, 1.0, 128.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	//gluLookAt(5.5, 8.0, 4.5, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
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

void MouseMotion(int x, int y)
{
	old_y = y;
	old_x = x;
}

void Examinar(int x, int y)
{
	float rot_x, rot_y;
	rot_y = (float)(old_y - y);
	rot_x = (float)(x - old_x);
	Rotar_Latitud(LOCAL_MyCamera, rot_y * DEGREE_TO_RAD);
	Rotar_Longitud(LOCAL_MyCamera, rot_x * DEGREE_TO_RAD);
	old_y = y;
	old_x = x;
	glutPostRedisplay();
}

void Zoom(int x, int y)
{
	float zoom;
	zoom = (float) ((y - old_y) * DEGREE_TO_RAD);
	old_y = y;
	switch(LOCAL_MyCamera->camMovimiento)
	{
	case CAM_EXAMINAR:
		if (LOCAL_MyCamera->camAperture + zoom > (5 * DEGREE_TO_RAD) &&
		LOCAL_MyCamera->camAperture + zoom < 175 * DEGREE_TO_RAD)
		LOCAL_MyCamera->camAperture=LOCAL_MyCamera->camAperture + zoom;
		break;
	}
	glutPostRedisplay();
}

void Andar(int x, int y)
{
	float rotacion_x, avance_y; 
	avance_y = (float)(y - old_y)/10;
	rotacion_x = (float)(old_x - x) * DEGREE_TO_RAD/5;
	YawCamera(LOCAL_MyCamera, rotacion_x);
	AvanceFreeCamera(LOCAL_MyCamera, avance_y);
	old_y = y;
	old_x = x;
	glutPostRedisplay();
}

void Tripode(int x, int y)
{
	float rotacion_x, rotacion_y;
	rotacion_x = (float)(old_x - x) * DEGREE_TO_RAD / 5;
	rotacion_y = (float)(old_y - y) * DEGREE_TO_RAD / 5;
	YawCamera( LOCAL_MyCamera, rotacion_x );
	TripodeCamera( LOCAL_MyCamera, rotacion_y );
	old_y = y;
	old_x = x;
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	old_x = x;
	old_y = y;
	switch(button)
	{
	case GLUT_LEFT_BUTTON:
		switch(LOCAL_MyCamera->camMovimiento)
		{
		case CAM_EXAMINAR:
			if (state == GLUT_DOWN) glutMotionFunc(Zoom);
			if (state == GLUT_UP)
			{
				glutPassiveMotionFunc(Examinar);
				glutMotionFunc(NULL);
			}
			break;
		case CAM_PASEAR:
			if (state == GLUT_DOWN) glutMotionFunc(Andar);
			if (state == GLUT_UP) glutMotionFunc(NULL);
			break;
		case CAM_TRIPODE:
			if (state == GLUT_DOWN) glutMotionFunc(Tripode);
			if (state == GLUT_UP) glutMotionFunc(NULL);
			break;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN);
		break;
	default:
		break;
	}
	glutPostRedisplay();
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

void SpecialKey(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_F1:
		glutPassiveMotionFunc(MouseMotion);
		LOCAL_MyCamera->camMovimiento = CAM_STOP;
		break;
	case GLUT_KEY_F2:
		glutPassiveMotionFunc(Examinar);
		LOCAL_MyCamera->camMovimiento = CAM_EXAMINAR;
		break;
	case GLUT_KEY_F3:
		glutPassiveMotionFunc(MouseMotion);
		LOCAL_MyCamera->camMovimiento = CAM_PASEAR;
		LOCAL_MyCamera->camAtY = 0;
		LOCAL_MyCamera->camViewY = 0;
		SetDependentParametersCamera( LOCAL_MyCamera );
		break;
	case GLUT_KEY_F4:
		if(LOCAL_MyCamera->camProjection == CAM_CONIC)
		{
			LOCAL_MyCamera->x1=-10;
			LOCAL_MyCamera->x2=10;
			LOCAL_MyCamera->y1=-10;
			LOCAL_MyCamera->y2=10;
			LOCAL_MyCamera->z1=-15;
			LOCAL_MyCamera->z2=15;
			LOCAL_MyCamera->camProjection = CAM_PARALLEL;
		} 
		else LOCAL_MyCamera->camProjection = CAM_CONIC;
		break;
	case GLUT_KEY_F5:
		glutPassiveMotionFunc(MouseMotion);
		LOCAL_MyCamera->camMovimiento = CAM_TRIPODE;
		break;
	case GLUT_KEY_HOME: //Reset Camera
		LOCAL_MyCamera->camAtX =0;
		LOCAL_MyCamera->camAtY =0;
		LOCAL_MyCamera->camAtZ =0;
		LOCAL_MyCamera->camViewX = 5.5;
		LOCAL_MyCamera->camViewY = 8.0;
		LOCAL_MyCamera->camViewZ = 4.5;
		SetDependentParametersCamera( LOCAL_MyCamera );
		break;
	default:
		printf("key %d %c X %d Y %d\n", key, key, x, y );
	}
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
	SetGLCamera(LOCAL_MyCamera);
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
	LOCAL_MyCamera = CreatePositionCamera(5.5f, 8.0f, 4.5f);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tortuga");
	glutDisplayFunc(displayTortuga);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutSpecialFunc(SpecialKey);
	glutPassiveMotionFunc(MouseMotion);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}

/*
- Para dibujar la tortuga, se utilizaron múltiples vértices usando la función GL_LINE_LOOP
- Solo se calcularon 21 vértices para la mitad de la tortuga. Para la otra mitad, se usó simetría respecto al eje X.
- Para las traslaciones, se usó la función glTranslatef(x, y, z).
- Para las rotaciones, se usó la función glRotatef(degree, x, y, z).
- Se pueden utilzar las teclas 'w', 'a', 's' y 'd' para avanzar, ir a la izquierda, retroceder e ir a la derecha, respectivamente.
- Se pueden usar las teclas del 1 al 6 para rotar a la tortuga respecto a los diferentes ejes.
- F1: Cámara inmóvil
- F2: Cámara Modo Examinar (presionar click izquierdo y mover mouse para zoom)
- F3: Cámara Modo Caminar (presionar click izquierdo y mover mouse para moverse)
- F4: Cambiar entre cámara Perspectiva y Ortogonal
- F5: Cámara Modo Trípode (mantener click izquierdo)
- Inicio (Home): Resetear la cámara

Bugs: si en modo Examinar, la cámara es ortogonal hacia el eje Y (mira directamente hacia abajo o directamente hacia arriba), se pierde la cámara.
Solución momentánea: presionar Inicio
*/