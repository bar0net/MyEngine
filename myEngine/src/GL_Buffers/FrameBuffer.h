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

		void SetTexture(unsigned int textureID, unsigned int slot = 0U, unsigned int type = 0x8CE0U) const;
		void SetRenderBuffer(unsigned int renderID, unsigned int type = 0x8D00U) const;

	public:
		unsigned int bufferID = 0U;
	};
}

#endif // !_MYENGINE_FRAMEBUFFER_H

