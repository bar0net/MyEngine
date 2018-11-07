#ifndef _MYENGINE_UNIFORMBUFFER_H
#define _MYENGINE_UNIFORMBUFFER_H

#include "GLBuffer.h"

namespace math { class float4x4; }

namespace MyEngine
{
	class UniformBuffer : public GLBuffer
	{
	public:
		UniformBuffer();
		virtual ~UniformBuffer();

		void Bind() const;
		void UnBind() const;

		void SetUniform4x4(unsigned int offset, math::float4x4* data) const;

		unsigned int ubo = 0;
	};
}
#endif //!_MYENGINE_UNIFORMBUFFER_H