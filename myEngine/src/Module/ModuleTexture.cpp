#include "ModuleTexture.h"

#include <assert.h>

#include "GL/glew.h"
#include "IL/il.h"
#include "IL/ilut.h"

#include "Globals.h"

#include "Utils/Render_Utils.h"
#include "Module/ModuleRenderer.h"

#include "GL_Buffers/FrameBuffer.h"
#include "GL_Buffers/RenderBuffer.h"

#include "GameObject/Components/ComponentMeshRenderer.h"

// TODO: Migrate all texture uses to Texture2D class.

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


	for (std::unordered_map<std::string, MyEngine::Texture2D*>::iterator it = file2texture.begin(); it != file2texture.end(); ++it)
	{
		RELEASE(it->second);
	}

	file2texture.clear();

	return true;
}


unsigned int ModuleTexture::LoadTexture(const char* filename)
{
	assert(filename);
	if (file2texture.find(filename) != file2texture.end())
	{
		return file2texture[filename]->ID();
	}

	LOGINFO("Loading Texture %s", filename);

	ILuint imageID;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	ILboolean success = IL_TRUE;
	ILenum error;

	if (ilLoadImage(filename))
	{
		std::string name(filename);
		if (name.find_last_of("/") != std::string::npos)
			name = name.substr(name.find_last_of("/") + 1);
		if (name.find_last_of("\\") != std::string::npos)
			name = name.substr(name.find_last_of("\\") + 1);

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

		file2texture[name.c_str()] = new MyEngine::Texture2D(width, height, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_FORMAT), (char*)data);
		file2texture[name.c_str()]->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		file2texture[name.c_str()]->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		file2texture[name.c_str()]->UnBind();

		ilDeleteImages(1, &imageID);
		LOGINFO("Texture %s loaded successfully.", filename);
		return file2texture[name.c_str()]->ID();
	}

	ilDeleteImages(1, &imageID);

	error = ilGetError();
	LOGERROR("Image load failed (%s) - IL reports error: %i - %s", filename, (int)error, iluErrorString(error));
	return 0U;
}

void ModuleTexture::AssignTexture(unsigned int textureID, Mesh * mesh)
{
	if (mesh->textureID != 0) texture2mesh[textureID].erase(mesh);

	texture2mesh[textureID].insert(mesh);
	mesh->textureID = textureID;
}


void ModuleTexture::DeleteTexture(const unsigned int id)
{
	if (id == 0U)
	{
		LOGWARNING("Trying to unload texture with id 0.");
		return;
	}

	if (texture2mesh.find(id) == texture2mesh.end())
	{
		LOGDEBUG("Texture %i is not assigned to any mesh.", id);
	}
	else
	{
		// Assign all meshes that display the texture to checkers
		std::unordered_set<Mesh*>* set = &texture2mesh[id];
		LOGDEBUG("Texture %i is assigned to %i meshes.", id, set->size());

		for (auto it = set->begin(); it != set->end(); ++it)
		{
			AssignTexture(checkers, *it);
		}
		texture2mesh.erase(id);
	}

	for (std::unordered_map<std::string, MyEngine::Texture2D*>::iterator it = file2texture.begin(); it != file2texture.end(); ++it)
	{
		if (it->second->ID() == id)
		{
			LOGINFO("Unloading texture %s (id: %i).", it->first.c_str(), id);
			RELEASE(it->second);
			file2texture.erase(it->first);
			return;
		}
	}

	LOGWARNING("Trying to unload texture %i but it is not registered.", id);
}

void ModuleTexture::BindTexture(unsigned int textureID)
{
	if (MyEngine::Globals::active_texture != textureID)
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
		MyEngine::Globals::active_texture = textureID;
	}
}


