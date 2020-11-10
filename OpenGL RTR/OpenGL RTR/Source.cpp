#include <iostream>
#include "Game.h"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 1200
#define VIEWPORT_WIDTH 1600
#define VIEWPORT_HEIGHT 1200

#define STD_VSHADER_PATH "./Shaders/standard_vs.vert"
#define STD_FSHADER_PATH "./Shaders/standard_fs.frag"

#define APP_NAME "OpenGL"

using namespace Graphics;

int main()
{
	Game game(APP_NAME, 
			WINDOW_WIDTH,
			WINDOW_HEIGHT, 
			VIEWPORT_WIDTH,
			VIEWPORT_HEIGHT,
			3, 
			3,
			STD_VSHADER_PATH,
			STD_FSHADER_PATH
			);

	while (!game.windowShouldClose())
	{
		game.processInput();
		game.render();
		glfwPollEvents();
	}

	

	return 0;
}
