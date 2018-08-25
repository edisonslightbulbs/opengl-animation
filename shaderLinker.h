#pragma once

#include <iostream>


#ifdef __linux__
 #include <GL/glew.h>
#endif


#ifdef _WIN32 || _WIN64
 #include "GL\glew.h"
#endif


class ShaderLinker
{
public:
	ShaderLinker();
	~ShaderLinker();

	static const GLchar* readShader(const char* filename);
	static GLuint makeProgram(const char* vertex, const char* fragment);
};


