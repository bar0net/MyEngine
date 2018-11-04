#ifndef _MODULE_MODEL_LOADER_H
#define _MODULE_MODEL_LOADER_H

#include "Module.h"

#include <vector>
#include <unordered_map>
#include <string>

#include "../Utils/VertexBufferLayout.h"

class aiMesh;

namespace MyEngine
{
	class VertexBuffer;
}


struct Mesh
{
	bool valid = true;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	MyEngine::VertexBufferLayout layout;

	unsigned int num_triangles = 0;
	std::string texture_path;
};


class ModuleModelLoader : public Module
{
public:
	ModuleModelLoader();
	virtual ~ModuleModelLoader();

	static Mesh Load(const char* filename);

private:
	static void ParseMesh(const aiMesh* const mesh, std::vector<float>* const vertices, std::vector<unsigned int>* const indices, std::unordered_map<std::string, unsigned int>* vertex2index);

};

#endif // !_MODULE_MODEL_LOADER_H