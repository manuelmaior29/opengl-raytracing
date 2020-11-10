#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

using namespace std;

namespace Graphics
{
	class Shader
	{
	private:
		unsigned int ID;

	public:

		Shader();
		Shader(const char* vertexPath, const char* fragmentPath);

		void loadShader(const char* vertexPath, const char* fragmentPath);

		unsigned int get();
		void use();
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setMatrix(const std::string& name, glm::mat4 value) const;
	};
}
#endif