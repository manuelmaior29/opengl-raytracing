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
			   int glMinor)
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
		this->initShaders();
		
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

	void Game::initShaders()
	{
		Shader* standardShader = new Shader();
		standardShader->loadShader("./Shaders/standard_vs.vert", "./Shaders/standard_fs.frag");
		this->shaders.push_back(*standardShader);

		/*
		Shader* lightsourceShader = new Shader();
		lightsourceShader->loadShader("./Shaders/lightsource_vs.vert", "./Shaders/lighsource_fs.frag");
		this->shaders.push_back(*lightsourceShader);
		*/
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

		this->shaders[STANDARD_SHADER].use();

		this->shaders[STANDARD_SHADER].setVec("directionalLight.direction", this->directionalLight.direction);
		this->shaders[STANDARD_SHADER].setVec("directionalLight.color", this->directionalLight.color);
		this->shaders[STANDARD_SHADER].setVec("directionalLight.ambient", this->directionalLight.ambient);
		this->shaders[STANDARD_SHADER].setVec("directionalLight.diffuse", this->directionalLight.diffuse);
		this->shaders[STANDARD_SHADER].setVec("directionalLight.specular", this->directionalLight.specular);
		
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

		this->shaders[STANDARD_SHADER].use();
		this->shaders[STANDARD_SHADER].setVec("pointLight.position", this->pointLights[0].position);
		this->shaders[STANDARD_SHADER].setVec("pointLight.color", this->pointLights[0].color);
		this->shaders[STANDARD_SHADER].setVec("pointLight.ambient", this->pointLights[0].ambient);
		this->shaders[STANDARD_SHADER].setVec("pointLight.diffuse", this->pointLights[0].diffuse);
		this->shaders[STANDARD_SHADER].setVec("pointLight.specular", this->pointLights[0].specular);
		this->shaders[STANDARD_SHADER].setFloat("pointLight.constant", this->pointLights[0].constant);
		this->shaders[STANDARD_SHADER].setFloat("pointLight.linear", this->pointLights[0].linear);
		this->shaders[STANDARD_SHADER].setFloat("pointLight.quadratic", this->pointLights[0].quadratic);

		//TODO: Iterate through all point light sources

		/*
		this->shader.setVec("lightPos", glm::vec3(1, 1, 1));
		this->shader.setVec("lightColor", glm::vec3(1, 1, 1));
		*/
	}

	void Game::initObjects()
	{
		try
		{
			std::ifstream objectsDataFile("objects.txt");
			std::string objectLine;
			bool headerLine = true;

			// Parsing object metadata file
			while (std::getline(objectsDataFile, objectLine))
			{
				if (headerLine)
					headerLine = false;
				else
				{
					char modelName[20];
					float pX, pY, pZ;
					float rX, rY, rZ;
					float sX, sY, sZ;
					int shaderType;

					sscanf(objectLine.c_str(), "%s %f %f %f %f %f %f %f %f %f %d",
						modelName, &pX, &pY, &pZ, &rX, &rY, &rZ, &sX, &sY, &sZ, &shaderType);
					std::cout << "Loaded " << modelName << "|" << pX << "|" << pY << "|" << pZ << "|" << rX << "|" << rY << "|" << rZ << "|" << sX << "|" << sY << "|" << sZ << "|" << shaderType << std::endl;

					// Computation of Model matrix
					glm::mat4 modelMatrix = glm::mat4(1.0f);

					modelMatrix = glm::translate(modelMatrix, glm::vec3(pX, pY, pZ));
					
					if (rX != 0)
						modelMatrix = glm::rotate(modelMatrix, glm::radians(rX), glm::vec3(1.0f, 0.0f, 0.0f));
					if (rY != 0)
						modelMatrix = glm::rotate(modelMatrix, glm::radians(rY), glm::vec3(0.0f, 1.0f, 0.0f));
					if (rZ != 0)
						modelMatrix = glm::rotate(modelMatrix, glm::radians(rZ), glm::vec3(0.0f, 0.0f, 1.0f));
					
					modelMatrix = glm::scale(modelMatrix, glm::vec3(sX, sY, sZ));

					// 3D Object creation
					Object3D object =
					{
						new Model3D(),
						glm::vec3(pX, pY, pZ),
						glm::vec3(rX, rY, rZ),
						glm::vec3(sX, sY, sZ),
						modelMatrix,
						(SHADER_TYPE)shaderType,
					};

					std::string modelNameString = std::string(modelName);
					object.model->Load3DModel("Models/" + modelNameString + "/" + modelNameString + ".obj");

					this->objects.push_back(object);
				}
			}
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::OBJECTS_DATA_FILE::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}
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
		this->processMouseInput();
		this->processKeyboardInput();
		this->processTimePassing();
	}

	void Game::render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->shaders[STANDARD_SHADER].use();

		// View matrix
		glm::mat4 view = glm::mat4(1.0f);
		view = this->camera.getViewMatrix();
		this->shaders[STANDARD_SHADER].setMatrix("view", view);

		// Projection matrix
		glfwGetFramebufferSize(this->window, &this->viewportWidth, &this->viewportHeight);

		glm::mat4 projection = glm::perspective(glm::radians(85.0f), (float) this->viewportWidth / this->viewportHeight, 0.1f, 1000.0f);
		this->shaders[STANDARD_SHADER].setMatrix("projection", projection);

		// Update camera pos
		this->shaders[STANDARD_SHADER].setVec("viewPos", this->camera.getCameraPosition());

		int objectsSize = this->objects.size();
		for (int i = 0; i < objectsSize; i++)
		{
			this->shaders[STANDARD_SHADER].use();

			// Model matrix
			this->shaders[STANDARD_SHADER].setMatrix("model", this->objects.at(i).modelMatrix);

			// Normal matrix
			glm::mat3 normalModel = glm::mat3(glm::inverseTranspose(this->objects.at(i).modelMatrix));
			this->shaders[STANDARD_SHADER].setMatrix("normalModel", normalModel);


			this->objects.at(i).model->Draw(this->shaders[STANDARD_SHADER]);
		}

		// Swap front buffer with back buffer
		glfwSwapBuffers(window);
	}
}

