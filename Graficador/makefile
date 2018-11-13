CC=g++
CFLAGS=-lglut -lGLU -lGL

default: Graficador

Graficador: Graficador.cpp imgui/imgui.cpp imgui/imgui_impl_freeglut.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp
	$(CC) -std=c++11 -o Graficador Graficador.cpp imgui/imgui.cpp imgui/imgui_impl_freeglut.cpp imgui/imgui_impl_opengl2.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp $(CFLAGS) 

clean:
	$(RM) Graficador *.o *~	