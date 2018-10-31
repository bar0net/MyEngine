#ifndef _MODULE_TEXTURE_H
#define _MODULE_TEXTURE_H

#include "Module.h"

#include <unordered_set>;

class ModuleTexture
{
public:
	ModuleTexture();
	virtual ~ModuleTexture();

	bool Init();
	bool CleanUp();

	unsigned int LoadTexture(const char* filename);
	void UnLoadTexture(unsigned int id);

private:
	std::unordered_set<unsigned int> textures;

};

#endif // !_MODULE_TEXTURE_H
