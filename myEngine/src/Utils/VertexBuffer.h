#ifndef _MYENGINE_VERTEXBUFFER_H
#define _MYENGINE_VERTEXBUFFER_H

#include <vector>

namespace MyEngine 
{
	class VertexBuffer
	{
	public:
		VertexBuffer(const std::vector<float>* vertex_buffer_data);
		virtual ~VertexBuffer();

		void Bind() const;
		void UnBind() const;

		unsigned int vbo = 0;
	};
}

#endif // !_MYENGINE_VERTEXBUFFER_H

