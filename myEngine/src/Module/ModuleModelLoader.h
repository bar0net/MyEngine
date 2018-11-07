#ifndef _MODULE_MODEL_LOADER_H
#define _MODULE_MODEL_LOADER_H

#include "Module.h"

#include <vector>
#include <unordered_map>
#include <string>

#include "GL_Buffers/VertexBufferLayout.h"

class aiMesh;

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
};


class ModuleModelLoader : public Module
{
public:
	ModuleModelLoader();
	virtual ~ModuleModelLoader();

	static bool Load(const char * filename, std::vector<Model>& models);

private:
	static void ParseMesh(const aiMesh* const mesh, unsigned int num_positions, std::vector<float>* const vertices, std::vector<unsigned int>* const indices, std::unordered_map<std::string, unsigned int>* vertex2index);

};

#endif // !_MODULE_MODEL_LOADER_H