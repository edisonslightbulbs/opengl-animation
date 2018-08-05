#include "display.h"
#include "controls.h"
#include "GL\glew.h"
#include "GL\freeglut.h"
#include "IL\il.h"
#include "IL\ilu.h"
#include "IL\ilut.h"

#include <iostream>
#include <assert.h>

using namespace std;

Scene::Scene()
{
	screen_width = 1024;
	screen_height = 768;
	running = true;
	cout << " \n -------------------------------- Scene Loaded Successfully ! ----------------------------- \n"<< endl;
}

Scene::~Scene()
{

}

void Scene::init()
{
	// init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) >= 0)
	{
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1); // 1 | 0  >> hardware acceleration

		SDL_DisplayMode dm;
		SDL_GetDesktopDisplayMode(0, &dm);  // native display
		screen_width = dm.w;
		screen_height = dm.h;

		window = SDL_CreateWindow("Real-time Rendering : Skeleton Animation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | 0);

		if(window != 0){
			glcontext = SDL_GL_CreateContext(window);
		}
	}
	else{
		cout << " \n -------------------------------- Failed to initialize SDL -------------------------------- \n"<< endl;
		return;
	}

	SDL_SetWindowFullscreen(window, 0);

	// init GLEW
	GLenum err = glewInit();
	if(GLEW_OK != err){
		cout << " \n -------------------------------- Failed to initialize GLEW: " << glewGetErrorString(err) << "-------------------------------- \n"<< endl;
	}

	cout << " \n ---------------------------------- Initialized GLEW: " << glewGetString(GLEW_VERSION) << "---------------------------------- \n"<< endl;

	// init DevIL
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_UPPER_LEFT);

	glViewport(0, 0, screen_width, screen_height);
	glEnable(GL_DEPTH_TEST);

	int buffer;
	SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &buffer);

	// init animation
	animation.init();
}

void Scene::handleEvents()
{
	Controls::Instance()->updateEvent();
}

void Scene::update()
{
	animation.update();
}

void Scene::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// update key-frame animation
	animation.render();

	glFlush();
	SDL_GL_SwapWindow(window);
}

void Scene::clean()
{
	running = false;
	SDL_DestroyWindow(window);
	SDL_Quit();
}