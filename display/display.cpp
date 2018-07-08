#include <cmath>
#include <string>
#include <vector>
#include <chrono>
#include <cstring>
#include <iostream>

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h> 

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "../loader/loader.h"
#include "display.h"

Loader loader;	

using namespace std::chrono;
typedef high_resolution_clock Clock;
std::chrono::duration<double> last_frame;

uint64_t elapsed_ms_{0};
int32_t window_width_ = 1280;
int32_t window_height_ = 720;


Display::Display()
{

}

Display::~Display()
{

}


void glut_update()
{
	
}

void glut_motion(int32_t _x, int32_t _y)
{
	
}

void glut_keyboard(uint8_t _key, int32_t _x, int32_t _y)
{
	
}


void glut_mouse(int32_t _button, int32_t _state, int32_t _x, int32_t _y)
{
	
}


void glut_timer(int32_t _e)
{
  	glutPostRedisplay();
  	glutTimerFunc(16, glut_timer, 1);
  	elapsed_ms_ += 16;
}

void glut_resize(int32_t _width, int32_t _height) {
  	window_width_ = _width;
  	window_height_ = _height;
}

void glut_display()
{
  	loader.update(window_width_, window_height_);

  	glViewport(0, 0, (GLsizei)window_width_, (GLsizei)window_height_);
  	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  	loader.render();
  	glutSwapBuffers();
}

void Display::init(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
  	glutInitWindowSize(window_width_, window_height_);
	glutInitWindowPosition(64, 64);
	glutCreateWindow(argv[0]);
	glutSetWindowTitle("Skeleton-Animation");
	glEnable(GL_DEPTH_TEST);
  	glDisable(GL_CULL_FACE);
  	glewExperimental = GL_TRUE;
  	glewInit();

	ilInit(); 
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	ilEnable(IL_ORIGIN_SET); 
	ilOriginFunc(IL_ORIGIN_UPPER_LEFT); 

	loader.init();

 	glutDisplayFunc(glut_display);
	glutReshapeFunc(glut_resize);
	glutKeyboardFunc(glut_keyboard);
	glutMotionFunc(glut_motion);
	glutMouseFunc(glut_mouse);
	glutIdleFunc(glut_update);

  	glutShowWindow();
  	glutTimerFunc(16, glut_timer, 1);
  	glutMainLoop();	
}