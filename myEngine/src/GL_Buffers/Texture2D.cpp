#include "Texture2D.h"

#include "GL/glew.h"
#include "Utils/Render_Utils.h"

#include "Globals.h"

namespace MyEngine
{
	Texture2D::Texture2D(unsigned int width, unsigned int height, unsigned int input_format, unsigned int store_format, char* data)
	{
		GLCall(glGenTextures(1, &textureID));
		Bind();
		if (data)
		{
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, input_format, width, height, 0, store_format, GL_UNSIGNED_BYTE, (void*)data));
		}
		else
		{
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, input_format, width, height, 0, store_format, GL_UNSIGNED_BYTE, 0));
		}

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	}

	Texture2D::~Texture2D()
	{
		UnBind();
		if(textureID != 0) GLCall(glDeleteTextures(1, &textureID));
	}

	void Texture2D::Bind() const
	{
		if (Globals::active_texture != textureID)
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
			Globals::active_texture = textureID;
		}
	}

	void Texture2D::UnBind() const
	{
		if (Globals::active_texture == textureID)
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, 0));
			Globals::active_texture = 0U;
		}
	}

	void Texture2D::SetParameter(unsigned int parameter, unsigned int value) const
	{
		GLCall(glTexParameteri(GL_TEXTURE_2D, parameter, value));
	}
}