#include "ModuleTexture.h"

#include <assert.h>

#include "GL/glew.h"
#include "IL/il.h"
#include "IL/ilut.h"

#include "Globals.h"

#include "Utils/Render_Utils.h"

#include "GL_Buffers/FrameBuffer.h"
#include "GL_Buffers/Texture2D.h"
#include "GL_Buffers/RenderBuffer.h"

#include "GameObject/Components/ComponentMeshRenderer.h"

ModuleTexture::ModuleTexture()
{
}

ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::Init()
{
	ilInit();
	iluInit();
	ilutInit();

	checkers = LoadTexture("checkers.png");

	return true;
}

bool ModuleTexture::CleanUp()
{
	LOGINFO("Deleting all remaining loaded textures (%i textures)", file2texture.size());


	for (std::unordered_map<std::string, unsigned int>::iterator it = file2texture.begin(); it != file2texture.end(); ++it)
	{
		glDeleteTextures(1, &(it->second));
	}

	file2texture.clear();

	return true;
}


unsigned int ModuleTexture::LoadTexture(const char* filename)
{
	assert(filename);
	if (file2texture.find(filename) != file2texture.end())
	{
		return file2texture[filename];
	}

	LOGINFO("Loading Texture %s", filename);

	ILuint imageID;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	ILboolean success = IL_TRUE;
	ILenum error;

	if (ilLoadImage(filename))
	{

		GLuint textureID;
		GLCall(glGenTextures(1, &textureID));
		GLCall(glBindTexture(GL_TEXTURE_2D, textureID));

		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT) iluFlipImage();

		int channels = ilGetInteger(IL_IMAGE_CHANNELS);
		if (channels == 3) success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
		else if (channels == 4)	success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (!success) LOGERROR("Image conversion on Texture Load failed.");

		ILubyte* data = ilGetData();
		int width = ilGetInteger(IL_IMAGE_WIDTH);
		int height = ilGetInteger(IL_IMAGE_HEIGHT);

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), width, height, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, data));

		// Texture Interpolation
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

		file2texture[filename] = textureID;

		LOGINFO("Texture %s loaded successfully.", filename);
		return textureID;
	}

	ilDeleteImages(1, &imageID);
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	error = ilGetError();
	LOGERROR("Image load failed (%s) - IL reports error: %i - %s", filename, (int)error, iluErrorString(error));
	return 0;
}

void ModuleTexture::AssignTexture(unsigned int textureID, Mesh * mesh)
{
	if (mesh->textureID != 0) texture2mesh[textureID].erase(mesh);

	texture2mesh[textureID].insert(mesh);
	mesh->textureID = textureID;
}


void ModuleTexture::DeleteTexture(const unsigned int id)
{
	if (id == 0)
	{
		LOGWARNING("Trying to unload texture with id 0.");
		return;
	}

	if (texture2mesh.find(id) == texture2mesh.end())
	{
		LOGWARNING("Trying to unload texture with id %i, but it's not registered", id);
		return;
	}

	// Assign all meshes that display the texture to checkers
	std::unordered_set<Mesh*>* set = &texture2mesh[id];
	for (auto it = set->begin(); it != set->end(); ++it)
	{
		AssignTexture(checkers, *it);
	}
	texture2mesh.erase(id);
	

	for (std::unordered_map<std::string, unsigned int>::iterator it = file2texture.begin(); it != file2texture.end(); ++it)
	{
		if (it->second == id)
		{
			LOGINFO("Unloading texture %s (id: %i).", it->first, id);
			file2texture.erase(it->first);
			glDeleteTextures(1, &id);
			return;
		}
	}

	LOGWARNING("Trying to unload texture %i but it is not registered.", id);
}

void ModuleTexture::BindTexture(unsigned int textureID)
{
	GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
}


