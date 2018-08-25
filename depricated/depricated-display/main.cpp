
#include <iostream>

#include "display/display.h"

#define WIDTH 1280
#define HEIGHT 720

int main(int argc, char* argv[])
{

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
