#include "ModuleTexture.h"

#include "GL/glew.h"
#include "IL/il.h"
#include "IL/ilut.h"

#include "../Utils/Render_Utils.h"

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
	textures.empty();

	return true;
}

bool ModuleTexture::CleanUp()
{
	LOGINFO("Deleting all remaining loaded textures (%i textures)", textures.size());

	unsigned int i = 1;

	for (std::unordered_set<unsigned int>::iterator it = textures.begin(); it != textures.end(); ++it)
	{
		glDeleteTextures(1, &(*it));
	}

	textures.clear();

	return true;
}

unsigned int ModuleTexture::LoadTexture(const char* filename)
{
	ILuint imageID;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	ILboolean success;
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

		ILubyte* data = ilGetData();
		int width = ilGetInteger(IL_IMAGE_WIDTH);
		int height = ilGetInteger(IL_IMAGE_HEIGHT);

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), width, height, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, data));

		// Set texture clamping method
		//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		// Texture Interpolation
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

		ilDeleteImages(1, &imageID);
		textures.insert(textureID);
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		LOGINFO("Texture loading successful.");
		return textureID;
	}

	error = ilGetError();
	LOGERROR("Image load failed - IL reports error: %s - %s", error, iluErrorString(error));
	return 0;
}

void ModuleTexture::UnLoadTexture(const unsigned int id)
{
	if (id == 0 || textures.find(id) == textures.end())
	{
		LOGWARNING("Trying to unload texture %i but it is not registered.", id);
		return;
	}

	LOGINFO("Deleting texture %i.", id);
	glDeleteTextures(1, &id);
}


