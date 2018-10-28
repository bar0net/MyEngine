#ifndef _MYENGINE_UNIFORMBUFFER_H
#define _MYENGINE_UNIFORMBUFFER_H

#include "../_Vendor/MathGeoLib/Math/float4x4.h"

class UniformBuffer
{
public:
	UniformBuffer();
	virtual ~UniformBuffer();

	void Bind();
	void UnBind();

	void SetUniform4x4(unsigned int offset, math::float4x4* data);

	unsigned int ubo = 0;
};

#endif //!_MYENGINE_UNIFORMBUFFER_H