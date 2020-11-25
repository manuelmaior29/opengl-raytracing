#include "Model3D.hpp"
//#include "debug.h"
#include <glm/glm.hpp>


namespace Graphics {

	Model3D::Model3D() { }

	Model3D::Model3D(std::string fileName) : bHasNormalMap { false }
	{
		this->basePath = fileName.substr(0, fileName.find_last_of('/'));
		LoadModel(fileName);
	}

	Model3D::Model3D(std::string fileName, std::string basePath) : bHasNormalMap{ false }
	{
		this->basePath = basePath;
		LoadModel(fileName);
	}

	void Model3D::Load3DModel(std::string fileName)
	{
		this->basePath = fileName.substr(0, fileName.find_last_of('/'));
		LoadModel(fileName);
	}

	void Model3D::Load3DModel(std::string fileName, std::string basePath)
	{
		this->basePath = basePath;
		LoadModel(fileName);
	}

	// Draw each mesh from the model
	void Model3D::Draw(Graphics::Shader shaderProgram)
	{\
		for (unsigned int i = 0; i < meshes.size(); i++)
			meshes[i].Draw(shaderProgram);
	}

	// Does the parsing of the model file and fills in the data structure
	void Model3D::LoadModel(std::string fileName)
	{
		/*
		if (VERBOSE)
			std::cout << "Loading " << fileName << "...";
		*/
		Assimp::Importer import;
		const aiScene *scene = import.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << std::endl << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return;
		}

		processNode(scene->mRootNode, scene);

		/*
		if (VERBOSE)
			std::cout << "OK" << std::endl;
		*/
	}

	void Model3D::processNode(aiNode *node, const aiScene *scene)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		//process children (if any)
		for (unsigned int i = 0; i < node->mNumChildren; i++)
			processNode(node->mChildren[i], scene);
	}

	Mesh Model3D::processMesh(aiMesh *mesh, const aiScene *scene)
	{
		// data to fill
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		// Walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			// texture coordinates
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			// tangent
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
			// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
			vertices.push_back(vertex);
		}
		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		// process materials
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN

		// 1. diffuse maps
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_DISPLACEMENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		//5. opacity maps
		std::vector<Texture> opacityMaps = loadMaterialTextures(material, aiTextureType_OPACITY, "texture_opacity");
		textures.insert(textures.end(), opacityMaps.begin(), opacityMaps.end());

		// return a mesh object created from the extracted mesh data
		return Mesh(vertices, indices, textures);
	}

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	std::vector<Texture> Model3D::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
	{
		std::vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			bool skip = false;
			for (unsigned int j = 0; j < loadedTextures.size(); j++)
			{
				if (std::strcmp(loadedTextures[j].path.data(), str.C_Str()) == 0)
				{
					textures.push_back(loadedTextures[j]);
					skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{   // if texture hasn't been loaded already, load it
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), this->basePath, typeName);
				texture.type = typeName;
				if (typeName == "texture_normal")
					bHasNormalMap = true;
				texture.path = str.C_Str();
				textures.push_back(texture);
				loadedTextures.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}
		return textures;
	}

	unsigned int TextureFromFile(const char *path, const std::string &directory, std::string typeName, bool gamma)
	{
		std::string filename = std::string(path);
		filename = directory + '/' + filename;

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum internalFormat;
			GLenum pixelFormat;

			if (nrComponents == 1)
				pixelFormat = GL_RED;
			else if (nrComponents == 3)
				pixelFormat = GL_RGB;
			else if (nrComponents == 4)
				pixelFormat = GL_RGBA;

			if (typeName == "texture_diffuse" || typeName == "texture_specular")
			{
				if (nrComponents == 1)
					internalFormat = GL_RED;
				else if (nrComponents == 3)
					internalFormat = GL_SRGB;
				else if (nrComponents == 4)
					internalFormat = GL_SRGB_ALPHA;
			}
			else
			{
				if (nrComponents == 1)
					internalFormat = GL_RED;
				else if (nrComponents == 3)
					internalFormat = GL_RGB;
				else if (nrComponents == 4)
					internalFormat = GL_RGBA;
			}

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, pixelFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			/*
			//anisotropic filtering
			GLfloat fLargest;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
			*/

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}

	Model3D::~Model3D() {
		for (size_t i = 0; i < loadedTextures.size(); i++) {
			glDeleteTextures(1, &loadedTextures.at(i).id);
		}

		for (size_t i = 0; i < meshes.size(); i++) {
			GLuint VBO = meshes.at(i).getBuffers().VBO;
			GLuint EBO = meshes.at(i).getBuffers().EBO;
			GLuint VAO = meshes.at(i).getBuffers().VAO;
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
			glDeleteVertexArrays(1, &VAO);
		}
	}
}
