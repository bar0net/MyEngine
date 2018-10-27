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
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
	}

	void VertexBuffer::UnBind() const
	{
		GLCall(glDisableVertexAttribArray(0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void VertexBuffer::Draw() const
	{ 
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
	}
}