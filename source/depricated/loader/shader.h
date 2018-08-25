#pragma once

#include <iostream>
#include <GL/glew.h>

class ForShader
{
public:
	ForShader();
	~ForShader();

	static const GLchar* readShader(const char* filename);

	static GLuint makeProgram(const char* vertex, const char* fragment);
};

