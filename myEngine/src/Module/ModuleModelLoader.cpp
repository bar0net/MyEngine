#include "ModuleModelLoader.h"

#include <vector>
#include <unordered_map>
#include <string>

#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/material.h"
#include "assimp/mesh.h"

#include "Application.h"
#include "Module/ModuleRenderer.h"
#include "Module/ModuleTexture.h"
#include "GL_Buffers/VertexBufferLayout.h"


#define NUM_POSITIONS 3
#define NUM_UVW_COORD 2

ModuleModelLoader::ModuleModelLoader()
{
}

ModuleModelLoader::~ModuleModelLoader()
{
}

bool ModuleModelLoader::Load(const char * filename, std::vector<Model>& models, math::float4x4* transform)
{
	assert(filename && transform);

	LOGINFO("Loading 3D Model: %s", filename);

	std::string folder(filename);
	if (folder.find_first_of("\\") == std::string::npos) folder = "";
	else folder = folder.substr(0, folder.find_last_of("\\")+1);

	const aiScene* scene = aiImportFile(filename, aiProcess_Triangulate);

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

	transform->Set(&scene->mRootNode->mTransformation[0][0]);


	for (unsigned int i = 0U; i < scene->mNumMaterials; ++i)
	{
		aiString path;
		scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path, (aiTextureMapping*)aiTextureMapping_UV, 0);

		unsigned int textureID = App->texture->LoadTexture(  (folder + path.C_Str()).c_str() );
		materials.emplace_back( textureID );
	}

	ParseNode(scene->mRootNode, aiMatrix4x4(), scene, models, &materials);

	LOGINFO("%s Loaded successfully.", filename);
	
	aiReleaseImport(scene);
	return true;
}


void ModuleModelLoader::ParseNode(const aiNode* const node, aiMatrix4x4 transform, const aiScene* const scene, std::vector<Model>& models, 
	const std::vector<unsigned int>* const materials)
{
	assert(scene);

	if (node == nullptr) return;

	transform = transform * node->mTransformation;
	for (unsigned int i = 0U; i < node->mNumChildren; i++)
	{
		ParseNode(node->mChildren[i], transform, scene, models, materials);
	}

	for (unsigned int i = 0U; i < node->mNumMeshes; ++i)
	{
		Model m;

		switch (scene->mMeshes[i]->mPrimitiveTypes)
		{
		case (aiPrimitiveType_POINT):
			m.poylygon = 1;
			break;
		case(aiPrimitiveType_LINE):
			m.poylygon = 2;
			break;
		case(aiPrimitiveType_TRIANGLE):
			m.poylygon = 3;
			break;

		default:
			LOGWARNING("Mesh %d contains non supported polygons.");
			continue;
			break;
		}

		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ParseMesh(mesh, &transform, &m);

		m.textureID = (*materials)[mesh->mMaterialIndex];
		m.layout.Push<float>(m.poylygon);
		if (mesh->mNumUVComponents[0] > 0)
			m.layout.Push<float>(mesh->mNumUVComponents[0]);
		m.num_triangles = mesh->mNumFaces;

		models.emplace_back(m);
	}
}

void ModuleModelLoader::ParseMesh(const aiMesh* const mesh, const aiMatrix4x4* transform, Model* model)
{
	assert(mesh && transform && model);
	unsigned int num_uvw_coord = mesh->mNumUVComponents[0];

	for (unsigned int j = 0U; j < mesh->mNumVertices; ++j)
	{
		aiVector3D v = *transform * mesh->mVertices[j];

		float* vertex = (float*)&v;
		float* uvw = (float*)&mesh->mTextureCoords[0][j];
		
		for (unsigned int k = 0U; k < 3U; ++k)
		{
			model->vertices.push_back(vertex[k]);
			if (vertex[k] < model->mins[k]) model->mins[k] = vertex[k];
			if (vertex[k] > model->maxs[k]) model->maxs[k] = vertex[k];
		}

		for (unsigned int k = 0U; k < num_uvw_coord; ++k) model->vertices.push_back(uvw[k]);
	}

	for (unsigned int i = 0U; i < mesh->mNumFaces; ++i)
	{
		if (mesh->mFaces[i].mNumIndices > 3)
		{
			LOGWARNING("Mesh with more than 3 vertices per element.");
		}

		for (unsigned int j = 0U; j < mesh->mFaces[i].mNumIndices; ++j)
		{
			model->indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}
}