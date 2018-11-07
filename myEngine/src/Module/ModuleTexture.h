#ifndef _MODULE_TEXTURE_H
#define _MODULE_TEXTURE_H

#include "Module.h"

#include <unordered_set>
#include <unordered_map>

namespace MyEngine
{
	class FrameBuffer;
	class Texture2D;
	class RenderBuffer;
}

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	virtual ~ModuleTexture();

	bool Init();
	bool CleanUp();

	unsigned int LoadTexture(const char* filename);
	void UnLoadTexture(unsigned int id);
	void BindTexture(unsigned int textureID);

	unsigned int checkers;
	std::unordered_map<const char*, unsigned int> file2texture;

};

#endif // !_MODULE_TEXTURE_H
