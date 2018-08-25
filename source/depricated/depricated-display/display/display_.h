#ifndef DISPLAY_H
#define DISPLAY_H

#include "../loader/loader.h"
#include <SDL2/SDL.h>

#include <vector>

class Display
{
public:

	void init();
	void render();
	void update();


	int screen_width;
	int screen_height;

	bool IsClosed();

	SDL_Window* getWindow() const { return window; }


	static Display* Instance()
	{
		static Display inst;
		return &inst;
	}

	Display();
	~Display();

	SDL_Window* window = 0;
	SDL_GLContext glcontext = 0;

	Loader loader;

	bool isClosed;

private:




};

#endif //DISP