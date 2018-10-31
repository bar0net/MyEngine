#include "ModuleTexture.h"

#include "GL/glew.h"

#include "IL/il.h"
#include "IL/ilut.h"

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
	GLuint textureID;
	ILboolean success;
	ILenum error;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);
	success = ilLoadImage(filename);

	if (success)
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		int channels = ilGetInteger(IL_IMAGE_CHANNELS);
		if (channels == 3)
		{
			success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
		}
		else if (channels == 4)
		{
			success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		}

		if (!success)
		{
			error = ilGetError();
			LOGERROR("Image conversion failed - IL reports error: %s - %s", error, iluErrorString(error));
			exit(-1);
		}

		// Generate a new texture
		glGenTextures(1, &textureID);
		if (textures.find(textureID) != textures.end()) 
		{
			LOGERROR("Trying to create texture %i but it is already registered.", textureID);
			__debugbreak();
		}

		// Bind the texture to a name
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Set texture clamping method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		// Texture Interpolation
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT),
			ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0,
			ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
	}
	else
	{
		error = ilGetError();
		LOGERROR("Image load failed - IL reports error: %s - %s", error, iluErrorString(error));
		exit(-1);
	}

	ilDeleteImages(1, &imageID);

	LOGINFO("Texture creation successful.");

	textures.insert(textureID);

	return textureID;
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


