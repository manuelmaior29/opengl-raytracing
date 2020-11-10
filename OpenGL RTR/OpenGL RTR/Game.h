
#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include "glfw3.h"
#include "glad/glad.h"
#include <KHR/khrplatform.h>
#include "stb_image.h"
#include <filesystem>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <deque>

#include "Shader.h"
#include "Model3D.hpp"
#include "Camera.hpp"

using namespace std;

namespace Graphics
{
	class Game
	{
	private:
		static Game* game;
		
		// Singleton object

		// TODO: Extend to multiple models
		std::vector<Model3D> models;

		Camera camera;
		GLFWwindow* window;
		Shader shader;
		string windowName;
		int windowWidth;
		int windowHeight;
		int viewportWidth;
		int viewportHeight;
		int glMajor;
		int glMinor;

		float lastFrameTime;
		float deltaTime;

		float lastX;
		float lastY;

		void initGlfw();
		int initWindow();
		void initViewport();
		int initGlad();
		void initGLState();
		void initObjects();
		void initCamera();

		void processMouseInput();
		void processKeyboardInput();

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

		Game(const Game&) = delete;

		~Game(); 

		// Member methods
		void render();
		bool windowShouldClose();
		void processInput();
	};
}

#endif // !GAME_H

