#include "Game.h"

namespace Graphics
{
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

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
		this->lastX = viewportHeight / 2;
		this->lastY = viewportWidth / 2;

		this->initGlfw();
		this->initWindow();
		this->initGlad();
		this->initViewport();
		this->initGLState();
		this->initObjects();
		this->initCamera();
	
		this->shader.loadShader(vShaderPath, fShaderPath);

	}

	Game::~Game()
	{
		glfwTerminate();
	}

	// Private methods
	void Game::processMouseInput()
	{
		double xpos, ypos;

		glfwGetCursorPos(this->window, &xpos, &ypos);

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;

		lastX = xpos;
		lastY = ypos;

		// TODO: Customizable sensitivity (optional)
		const float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		this->camera.rotate(xoffset, yoffset);
	}

	void Game::processKeyboardInput()
	{
		float currentFrameTime = glfwGetTime();
		this->deltaTime = (currentFrameTime - this->lastFrameTime);
		this->lastFrameTime = currentFrameTime;

		if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(this->window, true);

		if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
			this->camera.move(MOVE_FORWARD, this->deltaTime);

		if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
			this->camera.move(MOVE_BACKWARD, this->deltaTime);

		if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
			this->camera.move(MOVE_LEFT, this->deltaTime);

		if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
			this->camera.move(MOVE_RIGHT, this->deltaTime);
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
		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

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
		Model3D* model = new Model3D();

		for (int i = 0; i < 50; i++)
		{
			Model3D* model = new Model3D();
			models.push_back(*model);
		}

		models.at(0).Load3DModel("Models/Sphere/Sphere.obj", "Models/Sphere");
		models.at(1).Load3DModel("Models/Plane/Plane.obj", "Models/Plane");
		
	}

	void Game::initCamera()
	{
		// TODO: Support for custom initial state (optional)
		this->camera.loadCamera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 2.5f);

		this->lastFrameTime = 0.0f;
		this->deltaTime = 0.0f;
	}

	void Game::processInput()
	{
		this->processMouseInput();
		this->processKeyboardInput();
	}

	void Game::render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->shader.use();

		// Model matrix
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		this->shader.setMatrix("model", model);

		// View matrix
		glm::mat4 view = glm::mat4(1.0f);
		view = this->camera.getViewMatrix();
		this->shader.setMatrix("view", view);

		// Projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(85.0f), 800.0f / 600.0f, 0.1f, 1000.0f);
		this->shader.setMatrix("projection", projection);

		int modelsSize = this->models.size();
		for (int i = 0; i < modelsSize; i++)
		{
			models.at(i).Draw(this->shader);
		}

		// Swap front buffer with back buffer
		glfwSwapBuffers(window);
	}
}

