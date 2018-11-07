#include "IndexBuffer.h"

#include "GL/glew.h"

#include "Utils/Render_Utils.h"
#include "Globals.h"

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

	void IndexBuffer::Bind() const
	{
		if (Globals::active_ibo == ibo) return;

		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
		Globals::active_ibo = ibo;
	}

	void IndexBuffer::UnBind() const
	{
		if (Globals::active_ibo == 0) return;

		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		Globals::active_ibo = 0;
	}

	void IndexBuffer::Draw() const
	{
		GLCall( glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT, NULL) );
	}


	void IndexBuffer::DrawLines(float line_width) const
	{
		GLCall(glLineWidth(line_width));
		GLCall(glDrawElements(GL_LINES, elements, GL_UNSIGNED_INT, NULL));
	}

	void IndexBuffer::DrawPoints(float point_size) const
	{
		GLCall(glPointSize(point_size));
		GLCall(glDrawElements(GL_POINTS, elements, GL_UNSIGNED_INT, NULL));
	}

	void IndexBuffer::DrawWires() const
	{
		GLCall(glDrawElements(GL_LINE_LOOP, elements, GL_UNSIGNED_INT, NULL));
	}

}