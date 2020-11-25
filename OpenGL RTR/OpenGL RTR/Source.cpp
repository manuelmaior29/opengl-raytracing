#include <iostream>
#include "Game.h"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 1200
#define VIEWPORT_WIDTH 1600
#define VIEWPORT_HEIGHT 1200

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
			3
			);

	while (!game.windowShouldClose())
	{
		game.render();
		game.processInput();
		glfwPollEvents();
	}


	return 0;
}
