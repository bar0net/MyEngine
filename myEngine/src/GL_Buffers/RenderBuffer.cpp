#include "RenderBuffer.h"

#include "GL/glew.h"
#include "Utils/Render_Utils.h"

MyEngine::RenderBuffer::RenderBuffer()
{
	GLCall(glGenRenderbuffers(1, &renderID));
	Bind();
}

MyEngine::RenderBuffer::~RenderBuffer()
{
	UnBind();
	if (renderID != 0) GLCall(glDeleteBuffers(1, &renderID));
}

void MyEngine::RenderBuffer::Bind() const
{
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, renderID));
}

void MyEngine::RenderBuffer::UnBind() const
{
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void MyEngine::RenderBuffer::SetStorage(unsigned int width, unsigned int height, unsigned int component)
{
	Bind();
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, component, width, height));
}
