#include "UniformBuffer.h"

#include "GL/glew.h"
#include "Render_Utils.h"

UniformBuffer::UniformBuffer() 
{
	// Uniform Buffer
	GLCall(glGenBuffers(1, &ubo));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, ubo));
	GLCall(glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(float4x4), NULL, GL_STREAM_DRAW));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));

	GLCall(glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, 2 * 16 * sizeof(float)));
}


UniformBuffer::~UniformBuffer() 
{
	GLCall(glDeleteBuffers(1, &ubo));
}


void UniformBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, ubo));
}


void UniformBuffer::UnBind() const
{
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}


void UniformBuffer::SetUniform4x4(unsigned int offset, math::float4x4* data) const
{
	GLCall(glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, 2 * sizeof(float4x4)));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, ubo));
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float4x4), &data->Transposed()[0][0]));
}