#ifndef _MYENGINE_RENDERBUFFER_H
#define _MYENGINE_RENDERBUFFER_H

#include "GLBuffer.h"

namespace MyEngine
{
	class RenderBuffer : public GLBuffer
	{
	public:
		RenderBuffer();
		virtual ~RenderBuffer();
		unsigned int ID() { return renderID; }

		void Bind() const;
		void UnBind() const;

		void SetStorage(unsigned int width, unsigned int height, unsigned int component);

	private:
		unsigned int renderID;
	};
}

#endif // !_MYENGINE_RENDERBUFFER_H

