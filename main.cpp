#include "display.h"

Scene scene;
int main(int argc, char* argv[])
{
	const char* animation_model_path = "collada/sphere/model.dae";
	const char* vertext_shader_path = "shaders/animated_model.vert";
	const char* fragment_shader_path = "shaders/animated_model.frag";

	//todo: should be an array / allow multimodels to be loaded

	scene.init(animation_model_path, vertext_shader_path, fragment_shader_path);
	while(scene.show())
	{
		scene.update();
		scene.render();
		scene.handleEvents();
	}
	return 0;
}