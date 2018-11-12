#ifndef _MYENGINE_TEXTURE_H
#define _MYENGINE_TEXTURE_H

#include "GLBuffer.h"

namespace MyEngine
{
	class Texture2D : public GLBuffer
	{
	public:
		Texture2D(unsigned int width, unsigned int height, unsigned int input_format = 0x1908, unsigned int store_format = 0x1908, char* data = nullptr);
		virtual ~Texture2D();
		unsigned int ID() { return textureID; }

		void Bind() const;
		void UnBind() const;

		void SetParameter(unsigned int parameter, unsigned int value) const;

	private:
		unsigned int textureID = 0;
	};
}

#endif // !_MYENGINE_TEXTURE_H