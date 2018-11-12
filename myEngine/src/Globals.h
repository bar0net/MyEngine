#ifndef _MYENGINE_GLOBALS_H

#define MYENGINE_VERSION "0.1.0"

#define RELEASE(x) if(x!=nullptr) { delete x; } x = nullptr

namespace MyEngine
{
	namespace Globals
	{
		static unsigned int active_shader = 0;
		static unsigned int active_vao = 0;
		static unsigned int active_ibo = 0;
		static unsigned int active_vbo = 0;
		static unsigned int active_ubo = 0;
		static unsigned int active_texture = 0;

		static unsigned int active_framebuffer = 0;
		static unsigned int active_renderbuffer = 0;
	}
}

#endif // !_MYENGINE_GLOBALS_H

