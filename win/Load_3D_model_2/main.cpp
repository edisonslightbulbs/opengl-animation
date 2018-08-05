#include "display.h"

Scene scene;
int main(int argc, char* argv[])
{
	scene.init();
	while(scene.show())
	{
		scene.update();
		scene.render();
		scene.handleEvents();
	}
	return 0;
}