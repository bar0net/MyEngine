#ifndef _MYENGINE_GLOBALS_H

#define MYENGINE_VERSION "0.1.0"

#define MYENGINE_PI 3.14159265358979323846f
#define DEG2RAD 0.0174532925F
#define RAD2DEG 57.2957795F

#define RELEASE(x) if(x!=nullptr) { delete x; } x = nullptr

namespace MyEngine
{
	namespace Globals
	{
		static unsigned int active_shader = 0U;
		static unsigned int active_vao = 0U;
		static unsigned int active_ibo = 0U;
		static unsigned int active_vbo = 0U;
		static unsigned int active_ubo = 0U;
		static unsigned int active_texture = 0U;

		static unsigned int active_framebuffer = 0U;
		static unsigned int active_renderbuffer = 0U;
	}
}

#endif // !_MYENGINE_GLOBALS_H

