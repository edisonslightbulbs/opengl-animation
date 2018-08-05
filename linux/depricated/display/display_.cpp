#include "display_.h"

#include <cmath>
#include <string>
#include <vector>
#include <chrono>
#include <cstring>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>


#include "../loader/loader.h"
#include <SDL2/SDL.h>


Display::Display()
{
	std::cout << "konstructor Display()" << std::endl;

	screen_width = 1024;
	screen_height = 768;
}


Display::~Display()
{

}

void Display::init()
{

	if(SDL_Init(SDL_INIT_EVERYTHING) >= 0){

		// SET ATTRIBUTE ONLY after initialize
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1); // on antialiasing sdl
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); //subsamples for each pixel
	
		window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_OPENGL);	
		glcontext = SDL_GL_CreateContext(window);
	}

	else
	{
		std::cout << "window create error " << std::endl;
		return; // sdl could not initialize
	}
	SDL_SetWindowFullscreen(window, 0); 

	glewExperimental=GL_TRUE;

		//GLenum status = glewInit();

		//if(status != GLEW_OK){ std::cerr << "Glew failed to initialize" << std::endl; }

	GLenum err = glewInit(); if (GLEW_OK != err) { std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl; }
	std::cerr << "Initialized GLEW: " <<  glewGetString(GLEW_VERSION) << std::endl;
	isClosed = false;


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

	loader.init();

}

bool Display::IsClosed()
{
	return isClosed;
}



void Display::update()
{
	loader.update(screen_width, screen_height );
}

void Display::render()
{
	
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	loader.render();


	//glFlush(); 

	SDL_GL_SwapWindow(window);

		//the operating systems passes events ("SDL_Event") to validate link communication with SLD, as such, these events require handling
	SDL_Event e;

	while(SDL_PollEvent(&e))
	{	
		// checking if we have closed the window - via the OS
		if (e.type == SDL_QUIT)
		{
			isClosed = true;
		}
	}
	 
}

