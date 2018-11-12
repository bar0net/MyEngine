#ifndef _MODULE_MODEL_LOADER_H
#define _MODULE_MODEL_LOADER_H

#include "Module.h"

#include <vector>
#include <unordered_map>
#include <string>

#include "GL_Buffers/VertexBufferLayout.h"
#include "_Vendor/MathGeoLib/Math/float4x4.h"
#include "assimp/cimport.h"


class aiMesh;
class aiNode;
class aiScene;

namespace MyEngine
{
	class VertexBuffer;
}


struct Model
{
	bool valid = true;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	MyEngine::VertexBufferLayout layout;

	unsigned int num_triangles = 0;
	unsigned int textureID = 0;
	unsigned int poylygon = 3;

	float mins[3] = {  INFINITY,  INFINITY,  INFINITY };
	float maxs[3] = { -INFINITY, -INFINITY, -INFINITY };
};


class ModuleModelLoader : public Module
{
public:
	ModuleModelLoader();
	virtual ~ModuleModelLoader();

	static bool Load(const char * filename, std::vector<Model>& models, math::float4x4* transform);

private:
	static void ParseNode(const aiNode* const node, aiMatrix4x4 transform, const aiScene* const scene, std::vector<Model>& models, 
						const std::vector<unsigned int>* const materials, std::unordered_map<std::string, unsigned int>* vertex2index);
	static void ParseMesh(const aiMesh* const mesh, const aiMatrix4x4* transform, Model* model, std::unordered_map<std::string, unsigned int>* vertex2index);

};

#endif // !_MODULE_MODEL_LOADER_H