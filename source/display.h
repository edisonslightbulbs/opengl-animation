#pragma once

#include "animation.h"

#include <vector>


#ifdef __linux__
#include <SDL/SDL.h>
#endif


#if defined(_WIN32) || defined(_WIN64)
 #include <SDL\SDL.h>
#endif


class Scene
{
public:
	void init(const char* model, const char* v_shader, const char* f_shader);
	void render();
	void handleEvents();
	void update();
	void clean();

	int screen_width;
	int screen_height;

	bool show() { return running; }
	SDL_Window* getWindow() const { return window; }

	static Scene* Instance(){
		static Scene inst;
		return &inst;
	}

	Scene();
	~Scene();

private:
	SDL_Window* window = 0;
	SDL_GLContext glcontext = 0;
	Animation animation;
	bool running;
};

