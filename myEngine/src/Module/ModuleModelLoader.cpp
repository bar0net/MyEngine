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
#include "../Module/ModuleTexture.h"
#include "../Utils/VertexBufferLayout.h"


#define NUM_POSITIONS 3
#define NUM_UVW_COORD 2

ModuleModelLoader::ModuleModelLoader()
{
}

ModuleModelLoader::~ModuleModelLoader()
{
}

bool ModuleModelLoader::Load(const char * filename, std::vector<Model>& models)
{
	LOGINFO("Loading 3D Model: %s", filename);

	const aiScene* scene = aiImportFile(filename, 0);
	if (scene == NULL) 
	{
		const char* error = aiGetErrorString();
		LOGERROR("Error loading %s: %s", filename, error);
		return false;
	}

	models.empty();
	models.reserve(scene->mNumMeshes);

	std::vector<unsigned int> materials;
	materials.reserve(scene->mNumMaterials);

	for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
	{
		aiString path;
		scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path, (aiTextureMapping*)aiTextureMapping_UV, 0);
		
		materials.emplace_back( App->texture->LoadTexture(path.C_Str()) );
	}

	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		Model m;
		std::unordered_map<std::string, unsigned int> vertex2index;
		ParseMesh(scene->mMeshes[i], &(m.vertices), &(m.indices), &vertex2index);
		m.textureID = materials[scene->mMeshes[i]->mMaterialIndex];
		m.layout.Push<float>(NUM_POSITIONS);
		m.layout.Push<float>(NUM_UVW_COORD);
		m.num_triangles = scene->mMeshes[i]->mNumFaces;
		models.emplace_back(m);
	}



	LOGINFO("%s Loaded successfully.", filename);

	return true;
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