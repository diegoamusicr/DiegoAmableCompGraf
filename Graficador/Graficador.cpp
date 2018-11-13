#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <tuple>
#include <algorithm>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_freeglut.h"
#include "imgui/imgui_impl_opengl2.h"

#define WIDTH 800
#define HEIGHT 800

using namespace std;

GLdouble mModel[16];

static bool dibujo_manual = 0;
static ImVec4 color_borde = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
static ImVec4 color_relleno = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
static int vertices_poligono = 0;
static int desplazamiento = 10;
static float rotacion = 0.05;
const double rad_2_deg = 360/(2*M_PI);
vector<tuple<GLint, GLint>> points;

//--------------------------------------------------------------

class Edges
{
public:
    GLint x1;
    GLint y1;
    GLint x2;
    GLint y2;
    GLint minY;
    GLint maxY;
    GLint minX;
    GLint maxX;
    GLfloat xVal;
    GLfloat slope;
    Edges(tuple<GLint, GLint> vertexOne, tuple<GLint, GLint> vertexTwo);
};

Edges::Edges(tuple<GLint, GLint> vertexOne, tuple<GLint, GLint> vertexTwo)
{
    this->x1 = get<0>(vertexOne);
    this->y1 = get<1>(vertexOne);
    this->x2 = get<0>(vertexTwo);
    this->y2 = get<1>(vertexTwo);

    this->minY = min(get<1>(vertexOne), get<1>(vertexTwo));
    this->maxY = max(get<1>(vertexOne), get<1>(vertexTwo));
    this->minX = min(get<0>(vertexOne), get<0>(vertexTwo));
    this->maxX = max(get<0>(vertexOne), get<0>(vertexTwo));
    
    if (min(get<1>(vertexOne), get<1>(vertexTwo)) == get<1>(vertexOne)) {
        this->xVal = get<0>(vertexOne);
    } else {
        this->xVal = get<0>(vertexTwo);
    }
    this->slope = static_cast<GLfloat>(static_cast<GLfloat>(get<1>(vertexOne) - get<1>(vertexTwo)))
    			/ static_cast<GLfloat>((get<0>(vertexOne) - get<0>(vertexTwo)));
}

vector<Edges> allEdges; // Contains every Edge.
vector<Edges> activeEdges; // Contains Edges currently intersecting the scanline.


struct less_than_key
{
    inline bool operator() (const Edges& struct1, const Edges& struct2)
    {
        if (struct1.minY != struct2.minY)
            return (struct1.minY < struct2.minY);
        return (struct1.minX < struct2.minX);
    }
};

struct by_x_val_key
{
    inline bool operator() (const Edges& struct1, const Edges& struct2)
    {
        return (struct1.xVal < struct2.xVal);
    }
};

struct RGBType
{
    GLfloat r;
    GLfloat g;
    GLfloat b;
};

// Globals
int X_DOWN;
GLint scanline;
bool TOGGLE_STATE;
bool MOUSE_STATE;
bool DRAWING; // Enables or disables user input.
bool EDGE_PARITY;
 // Contains all the vertexes.

RGBType *pixels; // Contains every pixel on the screen.

void drawPixels(GLfloat x1, GLfloat x2)
{
    int i1 = roundf(x1);
    int i2 = roundf(x2);
    int count = 0;
    
    for (int i = ((WIDTH * (HEIGHT - (scanline + HEIGHT/2))) + i1 - WIDTH*40.5); i < ((WIDTH * (HEIGHT - (scanline + HEIGHT/2))) + i2 - WIDTH*40.5); i++, count++)
    {
        pixels[i].r = color_relleno.x;
        pixels[i].b = color_relleno.y;
        pixels[i].g = color_relleno.z;
        glutPostRedisplay();
        printf("change pixel %d, %d\n", (i1 + count), scanline);
    }
    printf("Pixels drawn.\n");
}

void removeActiveEdgesByScanline()
{
    for (vector<Edges>::iterator it = activeEdges.begin(); it < activeEdges.end(); )
    {
        if (it->maxY == scanline) activeEdges.erase(it);
        else it++;
    }
}

void updateXValues()
{
    for (vector<Edges>::iterator it = activeEdges.begin(); it < activeEdges.end(); it++)
        it->xVal += (1/it->slope);
}

void updateActiveEdges()
{
    for (vector<Edges>::iterator it = allEdges.begin(); it < allEdges.end(); it++) {
        if (it->minY == scanline)
            activeEdges.push_back(*it);
        if (it->minY > scanline) return;
    }
}

void initScanline()
{
    if (allEdges.size() != 0)
    {
        scanline = allEdges.at(0).minY;
        cout << "Scanline = " << scanline << endl;;
        glutPostRedisplay();
    }
}

void sortAndFilterEdges()
{
    sort(allEdges.begin(), allEdges.end(), less_than_key());
    for (vector<Edges>::iterator it = allEdges.begin(); it < allEdges.end(); it++)
        if (it->slope == 0) 
        	allEdges.erase(it);
}

void sortActiveEdgesByXValues()
{
    sort(activeEdges.begin(), activeEdges.end(), by_x_val_key());
}

void fillPolygon()
{
    printf("Filling...");
    while (activeEdges.size() != 0)
    {
        for (vector<Edges>::iterator it = activeEdges.begin(); it < activeEdges.end(); it++)
        {
            printf("drawing from %f to %f\n", it->xVal, (it+1)->xVal);
            drawPixels(it->xVal, (it+1)->xVal);
            it++;
        }
        scanline++;
        removeActiveEdgesByScanline();
        updateXValues();
        updateActiveEdges();
        sortActiveEdgesByXValues();
        glutPostRedisplay();
    }
}

//--------------------------------------------------------------
void initPixels()
{
	pixels = new RGBType[WIDTH*HEIGHT];
    
    for (int i = 0; i < WIDTH*HEIGHT; i++)
    {
        pixels[i].r = 0.0f;
        pixels[i].g = 0.0f;
        pixels[i].b = 0.0f;
	}
}

void Draw_Poligono()
{
	//glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(color_borde.x, color_borde.y, color_borde.z);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-WIDTH/2, WIDTH/2, -HEIGHT/2, HEIGHT/2, -1, 1);
	glMatrixMode(GL_MODELVIEW);

	glBegin(GL_LINES);
	for (vector<Edges>::iterator it = allEdges.begin(); it < allEdges.end(); it++)
	{
		glVertex2f(it->x1, it->y1);
		glVertex2f(it->x2, it->y2);
	}
	glEnd();
}

void Paint_Poligono()
{
	glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_FLOAT, pixels);
}

void PrintAristas()
{
	for (vector<Edges>::iterator it = allEdges.begin(); it < allEdges.end(); it++)
	{
		cout << it->x1 << " " << it->y1 << "->" << it->x2 << " " << it->y2 << endl;
	}
	cout << "-----------" << endl;
}

void CalcularAristas(int vertices)
{
	allEdges.clear();

	int radius = 100;
	double angle = 2*M_PI/vertices;

	for (int i = 0; i < vertices; i++)
	{
		double current_angle = i*angle;
		double next_angle = (i+1)*angle;
		int x1 = cos(current_angle)*radius;
		int y1 = sin(current_angle)*radius;
		int x2 = cos(next_angle)*radius;
		int y2 = sin(next_angle)*radius;

		points.push_back(tuple<GLint, GLint>(x1, y1));
		Edges newEdge(tuple<GLint, GLint>(x1, y1), tuple<GLint, GLint>(x2, y2));
		allEdges.push_back(newEdge);
	}
}

void Dibujo_GUI()
{
    //ImGui::ShowDemoWindow();

    static int vertices = 3;

    ImGui::Begin("Herramientas");

    if (ImGui::CollapsingHeader("Dibujo"))
    {
    	ImGui::Checkbox("Dibujar vertices manualmente", &dibujo_manual);
        ImGui::Text("Numero de vertices = %d", vertices); ImGui::SameLine();
        if (ImGui::ArrowButton("##vertices_up", ImGuiDir_Up))
        	vertices++;
        ImGui::SameLine();
        if (ImGui::ArrowButton("##vertices_down", ImGuiDir_Down) && vertices > 3)
        	vertices--;
        if (ImGui::Button("Dibujar"))
        {
        	initPixels();
        	CalcularAristas(vertices);
        	PrintAristas();
        }
    }

    if (ImGui::CollapsingHeader("Coloreo"))
    {
	    ImGui::ColorEdit3("Color de borde", (float*)&color_borde);
	    ImGui::ColorEdit3("Color de relleno", (float*)&color_relleno);
	    if (ImGui::Button("Pintar"))
	    {
	    	sortAndFilterEdges();
	    	initScanline();
	    	updateActiveEdges();
	    	fillPolygon();
	    	glutPostRedisplay();
	    }
	}

    if (ImGui::CollapsingHeader("Traslacion"))
    {
    	ImGui::Text("Trasladar Poligono");
        ImGui::SliderInt("Desplazamiento", &desplazamiento, 10, 200);
        ImVec2 dummy_sz(ImGui::GetFrameHeight(), ImGui::GetFrameHeight());
        ImGui::Dummy(dummy_sz); ImGui::SameLine(0, 160);
        if (ImGui::ArrowButton("##traslacion_up", ImGuiDir_Up))
        {
        	glTranslatef(0, desplazamiento, 0);
        }
        ImGui::Dummy(dummy_sz); ImGui::SameLine(0, 135);
        if (ImGui::ArrowButton("##traslacion_left", ImGuiDir_Left))
        {
        	glTranslatef(-desplazamiento, 0, 0);
        }
        ImGui::SameLine(0, 30);
        if (ImGui::ArrowButton("##traslacion_right", ImGuiDir_Right))
        {
        	glTranslatef(desplazamiento, 0, 0);
        }
        ImGui::Dummy(dummy_sz); ImGui::SameLine(0, 160);
        if (ImGui::ArrowButton("##traslacion_down", ImGuiDir_Down))
        {
        	glTranslatef(0, -desplazamiento, 0);
        }
    }

    if (ImGui::CollapsingHeader("Rotacion"))
    {
    	ImGui::Text("Rotar Poligono");
        ImGui::SliderAngle("Angulo de rotacion", &rotacion);
        if (ImGui::Button("Horario"))
        {
        	glPushMatrix();
			glLoadIdentity();
			glMultMatrixd(mModel);
        	glRotatef(-rotacion*rad_2_deg, 0.0f, 0.0f, 1.0f);
        	glGetDoublev(GL_MODELVIEW_MATRIX, mModel);
			glPopMatrix();
        }
        ImGui::SameLine();
        if (ImGui::Button("Antihorario"))
        {
        	glPushMatrix();
			glLoadIdentity();
			glMultMatrixd(mModel);
        	glRotatef(rotacion*rad_2_deg, 0.0f, 0.0f, 1.0f);
        	glGetDoublev(GL_MODELVIEW_MATRIX, mModel);
			glPopMatrix();
        }
    }

    ImGui::End();
}

void displayGraficador()
{
	ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplFreeGLUT_NewFrame();

    Dibujo_GUI();

    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
	glMultMatrixd(mModel);
    
    if (allEdges.size() > 2)
    {
    	Paint_Poligono();
   		Draw_Poligono();
   	}
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    glPopMatrix();

    glutSwapBuffers();
    glutPostRedisplay();

    glFlush();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Graficador de Poligonos");
	initPixels();
	glutDisplayFunc(displayGraficador);

    // Setup ImGui binding
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

    ImGui_ImplFreeGLUT_Init();
    ImGui_ImplFreeGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();
    ImGui::StyleColorsDark();

	glMatrixMode(GL_MODELVIEW);
	glGetDoublev(GL_MODELVIEW_MATRIX, mModel);
    glutMainLoop();

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplFreeGLUT_Shutdown();
    ImGui::DestroyContext();
	return 0;
}