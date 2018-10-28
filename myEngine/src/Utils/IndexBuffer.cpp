#include "IndexBuffer.h"

#include "GL/glew.h"
#include "Render_Utils.h"

namespace MyEngine {
	IndexBuffer::IndexBuffer(const std::vector<unsigned int>* indices)
	{
		GLCall(glGenBuffers(1, &ibo));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
		elements = indices->size();
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements * sizeof(unsigned int), indices->data(), GL_STATIC_DRAW));
	}

	IndexBuffer::~IndexBuffer()
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		if (ibo != 0) glDeleteBuffers(1, &ibo);
	}

	void IndexBuffer::Bind()
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	}

	void IndexBuffer::UnBind()
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	void IndexBuffer::Draw()
	{
		//GLCall(glDrawElements(GL_LINES, elements, GL_UNSIGNED_INT, NULL));
		GLCall( glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT, NULL) );
	}


	void IndexBuffer::DrawLines()
	{
		GLCall(glDrawElements(GL_LINES, elements, GL_UNSIGNED_INT, NULL));
	}
}