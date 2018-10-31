#include "VertexBuffer.h"

#include "GL/glew.h"
#include "Render_Utils.h"

namespace MyEngine
{
	VertexBuffer::VertexBuffer(const std::vector<float>* vertex_buffer_data)
	{
		GLCall(glGenBuffers(1, &vbo));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
		GLCall(glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data->size() * sizeof(float), vertex_buffer_data->data(), GL_STATIC_DRAW));
	}

	VertexBuffer::~VertexBuffer()
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		if (vbo != 0) glDeleteBuffers(1, &vbo);
	}

	void VertexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	}

	void VertexBuffer::UnBind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}