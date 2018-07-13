#include "loader.h"
#include "shader.h"

#include <chrono>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

using namespace std::chrono;
typedef high_resolution_clock Clock;

Loader::Loader()
{
	glm::vec3 position(2.0f, 5.0f, 18.0f);
	camera = animation_Camera(position);
	mouse_first_in = true;
}

Loader::~Loader()
{
	glDeleteProgram(shade_model);
}

void Loader::init()
{
	//shaders used for all collada(.dae) animation models
	shade_model = ForShader::makeProgram("resources/shaders/animated_model.vert", "resources/shaders/animated_model.frag");

	model_0.loadModel("resources/collada/man/model.dae");
	model_1.loadModel("resources/collada/eagle/model.dae");
	model_2.loadModel("resources/collada/astroboy/model.dae");
	
	model_0.initShaders(shade_model);
	model_1.initShaders(shade_model);
	model_2.initShaders(shade_model);
	
	cout << " \n --------------- animation shaders and animated model/s loaded successfully --------------- \n"<< endl;

	model_matrix_0 = glm::rotate(model_matrix_0, glm::radians(-85.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model_matrix_0 = glm::rotate(model_matrix_0, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model_matrix_0 = glm::translate(model_matrix_0, glm::vec3(-19.0f, 20.0f, 0.0f));
	model_matrix_0 = glm::scale(model_matrix_0, glm::vec3(1.5f, 1.5f, 1.5f));

	model_matrix_1 = glm::rotate(model_matrix_1, glm::radians(-85.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model_matrix_1 = glm::rotate(model_matrix_1, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model_matrix_1 = glm::translate(model_matrix_1, glm::vec3(3.0f, 20.0f, 0.0f));
	model_matrix_1 = glm::scale(model_matrix_1, glm::vec3(1.5f, 1.5f, 1.5f));

	model_matrix_2 = glm::rotate(model_matrix_2, glm::radians(-85.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model_matrix_2 = glm::rotate(model_matrix_2, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model_matrix_2 = glm::translate(model_matrix_2, glm::vec3(22.0f, 20.0f, 0.0f));
	model_matrix_2 = glm::scale(model_matrix_2, glm::vec3(1.5f, 1.5f, 1.5f));

}

void Loader::update(int32_t width, int32_t height)
{
	auto _current_frame = system_clock::now();
    auto __current_frame = time_point_cast<milliseconds>(_current_frame);

    auto value = __current_frame.time_since_epoch();
    GLfloat current_frame = value.count();
	delta_time = (current_frame - last_frame);
	last_frame = current_frame;

	view_matrix = camera.getViewMatrix();
	projection_matrix = glm::perspective(glm::radians(camera.fov), (float)width / (float)height, 1.0f, 2000.0f); 
}

void Loader::render()
{
	glUseProgram(shade_model);

	glUniform3f(glGetUniformLocation(shade_model, "view_pos"), camera.camera_pos.x, camera.camera_pos.y, camera.camera_pos.z);
	glUniform1f(glGetUniformLocation(shade_model, "material.shininess"), 32.0f);
	glUniform1f(glGetUniformLocation(shade_model, "material.transparency"), 1.0f);

	glUniform3f(glGetUniformLocation(shade_model, "point_light.position"), camera.camera_pos.x, camera.camera_pos.y, camera.camera_pos.z);

	glUniform3f(glGetUniformLocation(shade_model, "point_light.ambient"), 0.1f, 0.1f, 0.1f);
	glUniform3f(glGetUniformLocation(shade_model, "point_light.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shade_model, "point_light.specular"), 1.0f, 1.0f, 1.0f);

	glUniform1f(glGetUniformLocation(shade_model, "point_light.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(shade_model, "point_light.linear"), 0.007);	
	glUniform1f(glGetUniformLocation(shade_model, "point_light.quadratic"), 0.0002);


	MVP = projection_matrix * view_matrix * model_matrix_0;
	glUniformMatrix4fv(glGetUniformLocation(shade_model, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	glUniformMatrix4fv(glGetUniformLocation(shade_model, "model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix_0));
	glm::mat4 matr_normals_cube_0 = glm::mat4(glm::transpose(glm::inverse(model_matrix_0)));
	glUniformMatrix4fv(glGetUniformLocation(shade_model, "normals_matrix"), 1, GL_FALSE, glm::value_ptr(matr_normals_cube_0));
	model_0.draw(shade_model);


	MVP = projection_matrix * view_matrix * model_matrix_1;
	glUniformMatrix4fv(glGetUniformLocation(shade_model, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	glUniformMatrix4fv(glGetUniformLocation(shade_model, "M_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix_1));
	glm::mat4 matr_normals_cube_1 = glm::mat4(glm::transpose(glm::inverse(model_matrix_1)));
	glUniformMatrix4fv(glGetUniformLocation(shade_model, "normals_matrix"), 1, GL_FALSE, glm::value_ptr(matr_normals_cube_1));
	model_1.draw(shade_model);


	MVP = projection_matrix * view_matrix * model_matrix_2;
	glUniformMatrix4fv(glGetUniformLocation(shade_model, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	glUniformMatrix4fv(glGetUniformLocation(shade_model, "M_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix_2));
	glm::mat4 matr_normals_cube_2 = glm::mat4(glm::transpose(glm::inverse(model_matrix_2)));
	glUniformMatrix4fv(glGetUniformLocation(shade_model, "normals_matrix"), 1, GL_FALSE, glm::value_ptr(matr_normals_cube_2));
	model_2.draw(shade_model);


	glUseProgram(0);
	glDepthFunc(GL_ALWAYS);
	glDepthFunc(GL_LESS);
}

GLuint Loader::loadImageToTexture(const char* image_path)
{
	ILuint ImageName; 
	ilGenImages(1, &ImageName); 
	ilBindImage(ImageName); 

	if (!ilLoadImage((ILstring)image_path)){
		cout << " ------------------------- Image loading failed ------------------------------------------- \n "<< endl;
	}

	GLuint textureID;
	glGenTextures(1, &textureID); 
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE), ilGetData());
	glGenerateMipmap(GL_TEXTURE_2D);

	ilDeleteImages(1, &ImageName);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

GLuint Loader::loadDDS(const char* image_path, int *w, int *h)
{
	/**
	 *   Loader:: loadDDS
	 *   compress image and save as .DDS (e.g photoshop / nvidia utility)
	 *   pass compressed image to this method. The method returns texture.   
	 */
	unsigned char header[124];

	FILE *fp;
	fp = fopen(image_path, "rb");
	if (fp == NULL){	
		return 0;
	}

	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		std::cout << "return 0; \n";

		fclose(fp);
		return 0;
	}
	
	fread(&header, 124, 1, fp);
	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linear_size = *(unsigned int*)&(header[16]);
	unsigned int mipmap_count = *(unsigned int*)&(header[24]);
	unsigned int four_cc = *(unsigned int*)&(header[80]);  // formats

	if (w != nullptr) *w = width;
	if (h != nullptr) *h = height;

#ifdef _DEBUG
	std::cout << "image: " << image_path << " mipmap_count = " << mipmap_count << std::endl;
#endif // _DEBUG

	unsigned char *buffer;
	unsigned int buff_size = mipmap_count > 1 ? linear_size * 2 : linear_size;
	buffer = (unsigned char*)malloc(buff_size * sizeof(unsigned char));
	fread(buffer, 1, buff_size, fp);
	fclose(fp);

#define FOURCC_DXT1 0x31545844 // "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // "DXT5" in ASCII

	unsigned int components = (four_cc == FOURCC_DXT1) ? 3 : 4;;
	unsigned int format;
	if (four_cc == FOURCC_DXT1)
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
	else if (four_cc == FOURCC_DXT3)
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
	else if (four_cc == FOURCC_DXT5)
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;

#undef FOURCC_DXT1
#undef FOURCC_DXT3
#undef FOURCC_DXT5

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (mipmap_count == 1) 
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);	// highest resolution
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);	// lowest resolution
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else 
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);			//  highest resolution
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipmap_count);	// lowest resolution
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;;
	unsigned int offset = 0;

	// load mipmap. if we have not mipmap mipmap_count = 1 and be loaded 1 image (0 level)
	for (unsigned int level = 0; level < mipmap_count && (width || height); level++)
	{
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;
	}
	free(buffer);

	return textureID;
}