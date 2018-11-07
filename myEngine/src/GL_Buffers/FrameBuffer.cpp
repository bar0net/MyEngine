#include "FrameBuffer.h"

#include "GL/glew.h"

#include "Globals.h"
#include "Utils/Render_Utils.h"


MyEngine::FrameBuffer::FrameBuffer()
{
	GLCall(glGenFramebuffers(1, &bufferID));
	Bind();
}

MyEngine::FrameBuffer::~FrameBuffer()
{
	UnBind();
	if (bufferID != 0) GLCall(glDeleteBuffers(1, &bufferID));
}

void MyEngine::FrameBuffer::Bind() const
{
	if (Globals::active_framebuffer == bufferID) return;

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, bufferID));
	Globals::active_framebuffer = bufferID;
}

void MyEngine::FrameBuffer::UnBind() const
{
	if (Globals::active_framebuffer != bufferID) return;

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	Globals::active_framebuffer = 0;
}

void MyEngine::FrameBuffer::SetTexture(unsigned int textureID, unsigned int slot, unsigned int type) const
{
	Bind();
	GLCall(glFramebufferTexture(GL_FRAMEBUFFER, type + slot, textureID, slot));
}

void MyEngine::FrameBuffer::SetRenderBuffer(unsigned int renderID, unsigned int type) const
{
	Bind();
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, type, GL_RENDERBUFFER, renderID));
}
