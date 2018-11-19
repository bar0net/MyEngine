#include "VertexArray.h"

#include "Utils/Render_Utils.h"
#include "GL/glew.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Globals.h"

#include "LogSystem.h"

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
	if (Globals::active_vao == vao) return;

	GLCall(glBindVertexArray(vao));
	Globals::active_vao = vao;
}

void MyEngine::VertexArray::UnBind() const
{
	if (Globals::active_vao == 0) return;

	GLCall(glBindVertexArray(0));
	Globals::active_vao = 0U;
}

void MyEngine::VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout)
{
	this->Bind();
	vbo.Bind();
	const std::vector<VertexBufferElement>& elements = layout.Elements();
	unsigned int offset = 0U;
	for (unsigned int i = 0U; i < elements.size(); i++)
	{
		const VertexBufferElement& element = elements[i];
		if (element.count == 0U) continue;

		GLCall(glEnableVertexAttribArray(i));
		GLCall( glVertexAttribPointer(i, element.count, element.type, element.normalized ? GL_TRUE : GL_FALSE, layout.Stride(), (const void*)offset) );
		offset += element.count * VertexBufferElement::SizeOfType(element.type);
	}
}
