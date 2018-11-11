#ifndef _MYENGINE_FRAMEBUFFER_H
#define _MYENGINE_FRAMEBUFFER_H

#include "GLBuffer.h"

namespace MyEngine
{
	class FrameBuffer : public GLBuffer
	{
	public:
		FrameBuffer();
		virtual ~FrameBuffer();
		unsigned int ID() { return bufferID; }

		void Bind() const ;
		void UnBind() const;

		void SetTexture(unsigned int textureID, unsigned int slot = 0, unsigned int type = 0x8CE0) const;
		void SetRenderBuffer(unsigned int renderID, unsigned int type = 0x8D00) const;

	public:
		unsigned int bufferID = 0;
	};
}

#endif // !_MYENGINE_FRAMEBUFFER_H

