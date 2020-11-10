//
//  Mesh.hpp
//  Lab4
//
//  Created by CGIS on 27/10/2016.
//  Copyright © 2016 CGIS. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <glm.hpp>


#include "Shader.h"

namespace Graphics {

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
	};

	struct Texture
	{
		GLuint id;
		std::string type;
		std::string path;
	};

	struct Material
		{
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
			GLfloat shininess;
		};

	struct Buffers {
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
	};

	class Mesh
	{
	public:
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture> textures;

		Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

		void Draw(Graphics::Shader shader);

		Buffers getBuffers();

	private:
		/*  Render data  */
		Buffers buffers;

		// Initializes all the buffer objects/arrays
		void setupMesh();
	};
}
#endif /* Mesh_hpp */
