
#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include "glfw3.h"
#include "glad/glad.h"
#include <KHR/khrplatform.h>
#include "Shader.h"
#include "stb_image.h"
#include <filesystem>

using namespace std;

namespace Graphics
{
	class Game
	{
	private:
		GLFWwindow* window;
		Shader shader;
		string windowName;
		int windowWidth;
		int windowHeight;
		int viewportWidth;
		int viewportHeight;
		int glMajor;
		int glMinor;

		//temp usage
		unsigned int VBO;
		unsigned int VAO;
		unsigned int EBO;
		unsigned int texture1;
		unsigned int texture2;

		// Initialization methods
		void initGlfw();
		int initWindow();
		void initViewport();
		int initGlad();
		void initObjects();

	public:
		Game(string windowName, 
			int windowWidth, 
			int windowHeight, 
			int viewportWidth, 
			int viewportHeight, 
			int glMajor, 
			int glMinor,
			const char* vShaderPath,
			const char* fShaderPath);
		~Game();

		void render();
		bool windowShouldClose();

		void processInput();
	};
}

#endif // !GAME_H

