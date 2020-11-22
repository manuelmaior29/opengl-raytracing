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

		// TODO: Game state initialization method
		this->firstMouse = true;

		this->initGlfw();
		this->initWindow();
		this->initGlad();
		this->initViewport();
		this->initGLState();
		this->shader.loadShader(vShaderPath, fShaderPath);
		
		this->initLights();
		this->initObjects();
		this->initCamera();
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

		if (this->firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			this->firstMouse = false;
		}

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

	void Game::processTimePassing()
	{
		std::cout << "Time passed\n";
		/*
		this->pointLights[0].position.y = 20 * sin(glfwGetTime());
		this->shader.use();
		this->shader.setVec("pointLight.position", this->pointLights[0].position);
		*/
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

	void Game::initLights()
	{
		// Initialization of the directional light
		this->directionalLight = 
		{
			glm::normalize(glm::vec3(0.0f, -1.0f, -1.0f)),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f)
		};

		this->shader.use();

		this->shader.setVec("directionalLight.direction", this->directionalLight.direction);
		this->shader.setVec("directionalLight.color", this->directionalLight.color);
		this->shader.setVec("directionalLight.ambient", this->directionalLight.ambient);
		this->shader.setVec("directionalLight.diffuse", this->directionalLight.diffuse);
		this->shader.setVec("directionalLight.specular", this->directionalLight.specular);
		
		// Initialization of point lights
		this->pointLights.push_back
		({
			glm::vec3(glm::vec3(0.0f, 4.0f, 0.0f)),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			1.0f,
			0.027,
			0.0028
		});	

		this->shader.use();
		this->shader.setVec("pointLight.position", this->pointLights[0].position);
		this->shader.setVec("pointLight.color", this->pointLights[0].color);
		this->shader.setVec("pointLight.ambient", this->pointLights[0].ambient);
		this->shader.setVec("pointLight.diffuse", this->pointLights[0].diffuse);
		this->shader.setVec("pointLight.specular", this->pointLights[0].specular);
		this->shader.setFloat("pointLight.constant", this->pointLights[0].constant);
		this->shader.setFloat("pointLight.linear", this->pointLights[0].linear);
		this->shader.setFloat("pointLight.quadratic", this->pointLights[0].quadratic);

		//TODO: Iterate through all point light sources

		/*
		this->shader.setVec("lightPos", glm::vec3(1, 1, 1));
		this->shader.setVec("lightColor", glm::vec3(1, 1, 1));
		*/
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
		models.at(1).Load3DModel("Models/Room/Room.obj", "Models/Room");
	}

	void Game::initCamera()
	{
		// TODO: Support for custom initial state (optional)
		glm::vec3 cameraPos = glm::vec3(5.2f, 2.5f, -4.1f);
		this->camera.loadCamera(cameraPos, glm::vec3(10.0f, 0.0f, 1.0f), 4.5f);

		this->lastFrameTime = 0.0f;
		this->deltaTime = 0.0f;
	}

	void Game::processInput()
	{
		std::cout << "Position: " << this->camera.getCameraPosition().x << " " << this->camera.getCameraPosition().y << " " << this->camera.getCameraPosition().z;
		std::cout << "Direction: " << this->camera.getCameraDirection().x << " " << this->camera.getCameraDirection().y << " " << this->camera.getCameraDirection().z;
		this->processMouseInput();
		this->processKeyboardInput();
		this->processTimePassing();
	}

	void Game::render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->shader.use();

		// Model matrix
		glm::mat4 model = glm::mat4(1.0f);
		// TODO: Different model matrix for each object
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		this->shader.setMatrix("model", model);

		// Normal matrix
		glm::mat3 normalModel = glm::mat3(glm::inverseTranspose(model));
		this->shader.setMatrix("normalModel", normalModel);

		// View matrix
		glm::mat4 view = glm::mat4(1.0f);
		view = this->camera.getViewMatrix();
		this->shader.setMatrix("view", view);

		// Projection matrix
		glfwGetFramebufferSize(this->window, &this->viewportWidth, &this->viewportHeight);

		glm::mat4 projection = glm::perspective(glm::radians(85.0f), (float) this->viewportWidth / this->viewportHeight, 0.1f, 1000.0f);
		this->shader.setMatrix("projection", projection);

		// Update camera pos
		this->shader.setVec("viewPos", this->camera.getCameraPosition());

		int modelsSize = this->models.size();
		for (int i = 0; i < modelsSize; i++)
		{
			models.at(i).Draw(this->shader);
		}

		// Swap front buffer with back buffer
		glfwSwapBuffers(window);
	}
}

