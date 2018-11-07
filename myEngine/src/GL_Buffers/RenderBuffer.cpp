#include "RenderBuffer.h"

#include "Globals.h"
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
	if (Globals::active_renderbuffer == renderID) return;

	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, renderID));
	Globals::active_renderbuffer = renderID;
}

void MyEngine::RenderBuffer::UnBind() const
{
	if (Globals::active_renderbuffer != renderID) return;

	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	Globals::active_renderbuffer = 0;
}

void MyEngine::RenderBuffer::SetStorage(unsigned int width, unsigned int height, unsigned int component)
{
	Bind();
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, component, width, height));
}
