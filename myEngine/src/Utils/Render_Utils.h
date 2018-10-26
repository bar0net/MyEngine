#ifndef _MYENGINE_GRAPHICS_UTILS_H
#define _MYENGINE_GRAPHICS_UTILS_H

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) MyEngine::RenderUtils::GLClearErrors();\
				  x;\
				  ASSERT(MyEngine::RenderUtils::GLLogCall(#x, __FILE__, __LINE__))

namespace MyEngine {
	class LogSystem;

	struct Color
	{
		float r = 1.0f;
		float g = 1.0f;
		float b = 1.0f;
		float a = 1.0f;
	};

	class RenderUtils
	{
	public:
		static void CreateViewport(unsigned int width, unsigned int height, Color clear_color, float depth);
		static void ClearViewport();
		static void GLClearErrors();
		static bool GLLogCall(const char* function, const char* file, int line);
	};
}

#endif // !_MYENGINE_GRAPHICS_UTILS_H

