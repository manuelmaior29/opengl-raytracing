
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
#include <gtc/matrix_inverse.hpp>
#include <gtc/type_ptr.hpp>
#include <deque>

#include "Shader.h"
#include "Model3D.hpp"
#include "Camera.hpp"

using namespace std;

namespace Graphics
{

	enum SHADER_TYPE { STANDARD_SHADER, LIGHTSOURCE_SHADER };

	// Model structs
	typedef struct Object3D_
	{
		Model3D* model;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		glm::mat4 modelMatrix;
		SHADER_TYPE usedShader;
	} Object3D;

	// Light structs
	typedef struct PointLight_
	{
		glm::vec3 position;
		glm::vec3 color;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float constant;
		float linear;
		float quadratic;
	} PointLight;

	typedef struct DirectionalLight_
	{
		glm::vec3 direction;
		glm::vec3 color;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	} DirectionalLight;

	class Game
	{
	private:

		std::vector<Object3D> objects;

		DirectionalLight directionalLight;
		std::vector<PointLight> pointLights;

		Camera camera;
		GLFWwindow* window;

		// TODO: Create init method for shaders (possibly add vector of shaders)
		std::vector<Shader> shaders;
		string windowName;
		int windowWidth;
		int windowHeight;
		int viewportWidth;
		int viewportHeight;
		int glMajor;
		int glMinor;

		// State members
		bool firstMouse;
		float lastFrameTime;
		float deltaTime;
		float lastX;
		float lastY;

		// Test purposes


		void initGlfw();
		int initWindow();
		void initViewport();
		int initGlad();
		void initGLState();
		void initShaders();
		void initLights();
		void initObjects();
		void initCamera();

		void processTimePassing();
		void processMouseInput();
		void processKeyboardInput();

	public:

		Game(string windowName,
			int windowWidth,
			int windowHeight,
			int viewportWidth,
			int viewportHeight,
			int glMajor,
			int glMinor);

		Game(const Game&) = delete;

		~Game(); 

		// Member methods
		void render();
		bool windowShouldClose();
		void processInput();
	};
}

#endif // !GAME_H

