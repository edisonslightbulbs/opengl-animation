
#include <iostream>

#include "display/display_.h"

#define WIDTH 1280
#define HEIGHT 720

int32_t main(int32_t argc, char* argv[]) {

  Display render;
  render.init();


  	//while our display is open
	while(!render.IsClosed())
	{

		render.update();
		render.render();
	}

  return 0;
}
