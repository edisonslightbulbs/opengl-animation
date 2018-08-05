#pragma once

#include "Vector2D.h"

#include <iostream>
#include <vector>
#include <SDL\SDL.h>

enum mouse_buttons
{
	LEFT_PRESSED 	= 0,
	MIDDLE_PRESSED 	= 1,
	RIGHT_PRESSED 	= 2,
	LEFT_RELEASED 	= 3,
	MIDDLE_RELEASED = 4,
	RIGHT_RELEASED 	= 5
};

class Controls
{
public:
	static Controls* Instance()
	{
		static Controls inst;
		return &inst;
	}

	void updateEvent();
	void clean();

	bool getMouseButtonState(int buttonNumber)
	{
		return m_mouseButtonState[buttonNumber];
	}

	Vector2D getMousePosition()
	{
		return m_mousePosition;
	}

	bool keyPressed(SDL_Scancode key);
	bool keyReleased(SDL_Scancode key);
	void resetMouse();


private:
	Controls();

	std::vector<bool> m_mouseButtonState;
	Vector2D m_mousePosition;

	// handle mouse events
	void onMouseMove(SDL_Event& event);
	void onMouseButtonDown(SDL_Event& event);
	void onMouseButtonUp(SDL_Event& event);

	// keyboard
	const Uint8* h_keyStates;

};

