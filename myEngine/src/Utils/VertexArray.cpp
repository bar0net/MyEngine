#include "VertexArray.h"

#include "Render_Utils.h"
#include "GL/glew.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

MyEngine::VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &vao));
}

MyEngine::VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &vao));
}

void MyEngine::VertexArray::Bind() const
{
	GLCall(glBindVertexArray(vao));
}

void MyEngine::VertexArray::UnBind() const
{
	GLCall(glBindVertexArray(0));
}

void MyEngine::VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout)
{
	GLCall(glBindVertexArray(vao));
	vbo.Bind();
	const std::vector<VertexBufferElement>& elements = layout.Elements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const VertexBufferElement& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall( glVertexAttribPointer(i, element.count, element.type, element.normalized ? GL_TRUE : GL_FALSE, layout.Stride(), (const void*)offset) );
		offset += element.count + VertexBufferElement::SizeOfType(element.type);
	}
}
