#ifndef _MODULE_TEXTURE_H
#define _MODULE_TEXTURE_H

#include "Module.h"

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace MyEngine
{
	class FrameBuffer;
	class Texture2D;
	class RenderBuffer;
}

struct Mesh;

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	virtual ~ModuleTexture();

	bool Init();
	bool CleanUp();

	unsigned int LoadTexture(const char* filename);
	void AssignTexture(unsigned int textureID, Mesh* mesh);
	void DeleteTexture(unsigned int id);

	void BindTexture(unsigned int textureID);

	unsigned int checkers;
	std::unordered_map<std::string, unsigned int> file2texture;
	std::unordered_map<unsigned int, std::unordered_set<Mesh*>> texture2mesh;
	//std::unordered_map<unsigned int, MyEngine::Texture2D*> id2buffer;
};

#endif // !_MODULE_TEXTURE_H
