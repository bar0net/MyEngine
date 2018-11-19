#include "UniformBuffer.h"

#include "Globals.h"
#include "GL/glew.h"
#include "Utils/Render_Utils.h"
#include "_Vendor/MathGeoLib/Math/float4x4.h"

namespace MyEngine {
	UniformBuffer::UniformBuffer()
	{
		// Uniform Buffer
		GLCall(glGenBuffers(1, &ubo));
		Bind();

		GLCall(glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(float4x4), NULL, GL_STREAM_DRAW));
		GLCall(glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, 2 * 16 * sizeof(float)));
	}


	UniformBuffer::~UniformBuffer()
	{
		GLCall(glDeleteBuffers(1, &ubo));
	}


	void UniformBuffer::Bind() const
	{
		if (Globals::active_ubo == ubo) return;

		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, ubo));
		Globals::active_ubo = ubo;
	}


	void UniformBuffer::UnBind() const
	{
		if (Globals::active_ubo != ubo) return;

		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
		Globals::active_ubo = 0U;
	}


	void UniformBuffer::SetUniform4x4(unsigned int offset, math::float4x4* data) const
	{
		assert(data);
		GLCall(glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, 2 * sizeof(float4x4)));
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, ubo));
		GLCall(glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float4x4), &data->Transposed()[0][0]));
	}
}