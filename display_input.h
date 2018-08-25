#pragma once

#include <iostream>
#include <vector>
#include <math.h>


#ifdef __linux__
 #include <SDL2/SDL.h>
#endif

#ifdef _WIN32 || _WIN64
 #include <SDL\SDL.h>
#endif


class MouseOnScreenLocation
{
public:
	MouseOnScreenLocation() {};
	MouseOnScreenLocation(float x, float y) : m_x(x), m_y(y) {}

	float getX() { return m_x; }
	float getY() { return m_y; }

	void setX(float x) { m_x = x; }
	void setY(float y) { m_y = y; }

	float length() { return sqrt(m_x * m_x + m_y * m_y); }

	MouseOnScreenLocation operator+ (const MouseOnScreenLocation& v2) const
	{
		return MouseOnScreenLocation(m_x + v2.m_x, m_y + v2.m_y);
	}

	friend MouseOnScreenLocation& operator+= (MouseOnScreenLocation& v1, const MouseOnScreenLocation& v2)
	{
		v1.m_x += v2.m_x;
		v1.m_y += v2.m_y;
		return v1;
	}

	MouseOnScreenLocation operator* (float scalar)
	{
		return MouseOnScreenLocation(m_x * scalar, m_y * scalar);
	}

	MouseOnScreenLocation& operator *=(float scalar)
	{
		m_x *= scalar;
		m_y *= scalar;
		return *this;
	}

	MouseOnScreenLocation operator- (const MouseOnScreenLocation& v2) const
	{
		return MouseOnScreenLocation(m_x - v2.m_x, m_y - v2.m_y);
	}

	friend MouseOnScreenLocation& operator-= (MouseOnScreenLocation& v1, const MouseOnScreenLocation& v2)
	{
		v1.m_x -= v2.m_x;
		v1.m_y -= v2.m_y;
		return v1;
	}

	MouseOnScreenLocation operator/ (float scalar)
	{
		return MouseOnScreenLocation(m_x / scalar, m_y / scalar);
	}

	MouseOnScreenLocation& operator /=(float scalar)
	{
		m_x /= scalar;
		m_y /= scalar;
		return *this;
	}

	void normalize()
	{
		float l = length();
		if (l > 0){
			(*this) *= 1 / l;
		}
	}

private:
	float m_x;
	float m_y;
};


enum mouse_buttons
{
	LEFT_PRESSED 	= 0,
	MIDDLE_PRESSED 	= 1,
	RIGHT_PRESSED 	= 2,
	LEFT_RELEASED 	= 3,
	MIDDLE_RELEASED = 4,
	RIGHT_RELEASED 	= 5
};

class Control
{
public:
	static Control* Instance()
	{
		static Control inst;
		return &inst;
	}

	void updateEvent();
	void clean();

	bool getMouseButtonState(int buttonNumber)
	{
		return m_mouseButtonState[buttonNumber];
	}

	MouseOnScreenLocation getMousePosition()
	{
		return m_mousePosition;
	}

	bool keyPressed(SDL_Scancode key);
	bool keyReleased(SDL_Scancode key);
	void resetMouse();


private:
	Control();

	std::vector<bool> m_mouseButtonState;
	MouseOnScreenLocation m_mousePosition;

	// handle mouse events
	void onMouseMove(SDL_Event& event);
	void onMouseButtonDown(SDL_Event& event);
	void onMouseButtonUp(SDL_Event& event);

	// keyboard
	const Uint8* h_keyStates;
};
