#pragma once

#include "model.h"
#include "camera.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#ifdef __linux__
 #include <GL/glew.h>
#endif


#if defined(_WIN32) || defined(_WIN64)
 #include "GL\glew.h"
#endif


class Animation
{
public:
	Animation();
	~Animation();

	GLfloat speed = 0.02f;

	// camera
	GLfloat delta_time = 0.0f;
	GLfloat last_frame = 0.0f;
	GLfloat last_x;
	GLfloat last_y;

	MouseOnScreenLocation mouse_position;
	GLboolean mouse_movement;
	Camera camera;
	GLuint shade_model;

	Model model_0;
	Model model_1;
	Model model_2;
	Model model_3;

	glm::mat4 model_matrix_0;
	glm::mat4 model_matrix_1;
	glm::mat4 model_matrix_2;
	glm::mat4 model_matrix_3;		// M.
	glm::mat4 view_matrix;    		// V.
	glm::mat4 projection_matrix; 	// P.
	glm::mat4 MVP; 					// MVP

	void init(const char* model, const char* v_shader, const char* f_shader);
	void update();
	void render();

	static GLuint loadDDS(const char* image_path, int* w = nullptr, int* h = nullptr);
	static GLuint loadImageToTexture(const char* image_path);
};