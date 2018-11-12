#ifndef _MODULE_TEXTURE_H
#define _MODULE_TEXTURE_H

#include "Module.h"

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "GL_Buffers/Texture2D.h"

namespace MyEngine
{
	class FrameBuffer;
	class RenderBuffer;
}

struct Mesh;

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	virtual ~ModuleTexture();

	bool Init() override;
	bool CleanUp() override;

	unsigned int LoadTexture(const char* filename);
	void AssignTexture(unsigned int textureID, Mesh* mesh);
	void DeleteTexture(unsigned int id);

	void BindTexture(unsigned int textureID);

	unsigned int checkers;
	std::unordered_map<std::string, MyEngine::Texture2D*> file2texture;
	std::unordered_map<unsigned int, std::unordered_set<Mesh*>> texture2mesh;
	//std::unordered_map<unsigned int, MyEngine::Texture2D*> id2buffer;
};

#endif // !_MODULE_TEXTURE_H
