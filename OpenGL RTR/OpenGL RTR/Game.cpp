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
		this->initObjects();
	
		this->shader = Shader(vShaderPath, fShaderPath);
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
		return 1;
	}

	void Game::initObjects()
	{
		float vertices[] = {
			// positions          // colors           // texture coords
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f, // top right
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 1.0f,   0.0f, 2.0f  // top left 
		};

		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};

		std::cout << sizeof(vertices);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Position attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

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
		unsigned char* data2 = stbi_load("Textures/awesomeface.png", &width, &height, &nrChannels, 0);


		if (data2)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "ERROR::FAILED_TO_LOAD_TEXTURE" << std::endl;
		}

		stbi_image_free(data2);

		this->shader.use();
		this->shader.setInt("texture1", 0);

	}

	// 
	void Game::processInput()
	{
		if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(this->window, true);
	}

	void Game::render()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Compute offset
		float offset = sin(glfwGetTime() * 2.0);

		this->shader.setFloat("offset", offset);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		this->shader.use();
		glBindVertexArray(this->VAO);
//		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Swap front buffer with back buffer
		glfwSwapBuffers(window);
	}
}

