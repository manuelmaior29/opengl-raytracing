//
//  Model3D.hpp
//  Lab5
//
//  Created by CGIS on 28/10/2016.
//  Copyright © 2016 CGIS. All rights reserved.
//

#ifndef Model3D_hpp
#define Model3D_hpp

#include <iostream>
#include <string>
#include <vector>

#include "Mesh.hpp"

#include "stb_image.h"

//extern assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Graphics {

    class Model3D
    {

    public:
		Model3D();

		Model3D(std::string fileName);

		Model3D(std::string fileName, std::string basePath);

		~Model3D();

		void Load3DModel(std::string fileName);

		void Load3DModel(std::string fileName, std::string basePath);

		void Draw(Graphics::Shader shaderProgram);

    private:
		// Component meshes - group of objects
        std::vector<Graphics::Mesh> meshes;
		// Associated textures
        std::vector<Graphics::Texture> loadedTextures;

		std::string basePath;
		bool bHasNormalMap;

		// Does the parsing of the model file(s) and fills in the data structure
		void LoadModel(std::string fileName);

		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);

		std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
			std::string typeName);
    };

	unsigned int TextureFromFile(const char *path, const std::string &directory, std::string typeName, bool gamma = false);
}

#endif /* Model3D_hpp */
