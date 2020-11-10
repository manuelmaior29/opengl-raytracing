#include "Game.h"

// Class non-members
// Callback function for window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

namespace Graphics
{
	// Class members
	Game::Game(string windowName, 
			   int windowWidth, 
			   int windowHeight, 
			   int viewportWidth, 
		       int viewportHeight, 
			   int glMajor, 
			   int glMinor,
			   const char* vShaderPath,
			   const char* fShaderPath)
	{
		this->windowName = windowName;
		this->windowWidth = windowWidth;
		this->windowHeight = windowHeight;
		this->viewportWidth = viewportWidth;
		this->viewportHeight = viewportHeight;
		this->glMajor = glMajor;
		this->glMinor = glMinor;

		this->initGlfw();
		this->initWindow();
		this->initGlad();
		this->initViewport();
		this->initGLState();
		this->initObjects();
	
		this->model.Load3DModel("Models/SimpleCrate/SimpleCrate.obj", "Models/SimpleCrate/");
		this->shader.loadShader(vShaderPath, fShaderPath);

	}

	Game::~Game()
	{
		glfwTerminate();
	}

	// State checker methods
	bool Game::windowShouldClose()
	{
		return glfwWindowShouldClose(this->window);
	}

	// Initialization methods
	void Game::initGlfw()
	{
		// GLFW initialization
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->glMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->glMinor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}

	int Game::initWindow()
	{
		this->window = glfwCreateWindow(this->windowWidth, this->windowHeight, this->windowName.c_str(), NULL, NULL);

		if (window == NULL)
		{
			std::cout << "Failed to create window!" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(this->window);

		return 1;
	}

	void Game::initViewport()
	{
		glViewport(0, 0, this->viewportWidth, this->viewportHeight);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	}

	int Game::initGlad()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD!" << std::endl;
			return -1;
		}

		glEnable(GL_DEPTH_TEST);

		return 1;
	}

	void Game::initGLState()
	{
		glClearColor(0.95f, 0.85f, 0.55f, 0.0f);
	}

	void Game::initObjects()
	{
		/*
		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		std::cout << sizeof(vertices);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

		// Position attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Textures

		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data2 = stbi_load("Textures/download.jpg", &width, &height, &nrChannels, 0);

		if (data2)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "ERROR::FAILED_TO_LOAD_TEXTURE" << std::endl;
		}

		stbi_image_free(data2);

		this->shader.use();
		this->shader.setInt("texture1", 0);
		*/

	}

	// 
	void Game::processInput()
	{
		if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(this->window, true);
	}

	void Game::render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->shader.use();

		// Model matrix
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(100* (float)glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
		this->shader.setMatrix("model", model);

		// View matrix
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, 10.0f));
		this->shader.setMatrix("view", view);

		// Projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.1f, 1000.0f);
		this->shader.setMatrix("projection", projection);

		this->model.Draw(this->shader);

		// Swap front buffer with back buffer
		glfwSwapBuffers(window);
	}
}

