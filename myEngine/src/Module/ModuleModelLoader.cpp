#include "ModuleModelLoader.h"

#include <vector>
#include <unordered_map>
#include <string>

#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/material.h"
#include "assimp/mesh.h"

#include "../Application.h"
#include "../Module/ModuleRenderer.h"
#include "../Utils/VertexBufferLayout.h"


#define NUM_POSITIONS 3
#define NUM_UVW_COORD 2

ModuleModelLoader::ModuleModelLoader()
{
}

ModuleModelLoader::~ModuleModelLoader()
{
}

Mesh ModuleModelLoader::Load(const char * filename)
{
	LOGINFO("Loading 3D Model: %s", filename);
	Mesh output;

	const aiScene* scene = aiImportFile(filename, 0);
	if (scene == NULL) 
	{
		const char* error = aiGetErrorString();
		LOGERROR("Error loading %s: %s", filename, error);
		output.valid = false;
		return output;
	}

	std::unordered_map<std::string, unsigned int> vertex2index;
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i) ParseMesh(scene->mMeshes[i], &(output.vertices), &(output.indices), &vertex2index);

	aiString path;
	scene->mMaterials[0]->GetTexture(aiTextureType_DIFFUSE, 0, &path, (aiTextureMapping*)aiTextureMapping_UV, 0);
	output.texture_path = std::string(path.C_Str());

	//MyEngine::Shader* shader = App->renderer->CreateShader("default", "default.vs", "default.fs");
	output.layout.Push<float>(NUM_POSITIONS);
	output.layout.Push<float>(NUM_UVW_COORD);

	LOGINFO("%s Loaded successfully.", filename);
	output.valid = true;
	return output;
}


void ModuleModelLoader::ParseMesh(const aiMesh* const mesh, std::vector<float>* const vertices, std::vector<unsigned int>* const indices, std::unordered_map<std::string, unsigned int>* vertex2index)
{
	for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
	{
		float* vertex = (float*)&mesh->mVertices[j];
		float* uvw = (float*)&mesh->mTextureCoords[0][j];
		std::string s;

		for (unsigned int k = 0; k < NUM_POSITIONS; ++k)  s += std::to_string(vertex[k]);
		for (unsigned int k = 0; k < NUM_UVW_COORD; ++k) s += std::to_string(uvw[k]);

		if (vertex2index->find(s) == vertex2index->end())
		{
			for (unsigned int k = 0; k < NUM_POSITIONS; ++k)  vertices->push_back(vertex[k]);
			for (unsigned int k = 0; k < NUM_UVW_COORD; ++k) vertices->push_back(uvw[k]);
			indices->push_back(vertex2index->size());
			(*vertex2index)[s] = vertex2index->size();
		}
		else
		{
			indices->push_back((*vertex2index)[s]);
		}
	}
}