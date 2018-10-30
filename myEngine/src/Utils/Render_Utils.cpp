#include "Render_Utils.h"

#include <string>

#include "GL/glew.h"
#include "LogSystem.h"

namespace MyEngine {
	void RenderUtils::CreateViewport(unsigned int width, unsigned int height, Color clear_color, float depth)
	{
		glewInit();

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_TEXTURE_2D);

		glClearDepth(1.0f);
		//glClearColor(0.3f, 0.3f, 0.3f, 1.f);
		ChangeClearColor(0.3f, 0.3f, 0.3f, 1.0f);

		glViewport(0, 0, width, height);
	}

	void RenderUtils::ClearViewport()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RenderUtils::GLClearErrors()
	{
		while (glGetError() != GL_NO_ERROR);
	}

	bool RenderUtils::GLLogCall(const char * function, const char * file, int line)
	{
		GLenum error = glGetError();
		if (error == GL_NO_ERROR) return true;

		std::string s("[Error OpenGL] <");
		s.append(file); s.append(" - "); s.append(function); s.append(": "); s += std::to_string(line); s.append("> ");
		
		switch (error)
		{
		case GL_INVALID_ENUM:
			s.append("Invalid Enum.");
			break;

		case GL_INVALID_VALUE:
			s.append("Invalid Value.");
			break;

		case GL_INVALID_OPERATION:
			s.append("Invalid Operation.");
			break;

		case GL_STACK_OVERFLOW:
			s.append("Stack Overflow.");
			break;

		case GL_STACK_UNDERFLOW:
			s.append("Stack Underflow.");
			break;

		case GL_OUT_OF_MEMORY:
			s.append("Out of memory.");
			break;

		case GL_INVALID_FRAMEBUFFER_OPERATION:
			s.append("Invalid frame buffer operation.");
			break;

			case GL_CONTEXT_LOST:
			s.append("Context lost.");
			break;

		case GL_TABLE_TOO_LARGE:
			s.append("Table too large.");
			break;

		default:
			s += std::to_string((int)error);
			break;
		}

		LOG(s);
		return false;
	}

	void RenderUtils::ModifyViewportSize(unsigned int width, unsigned int height)
	{
		glViewport(0, 0, width, height);
	}

	void RenderUtils::ChangeClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}
}