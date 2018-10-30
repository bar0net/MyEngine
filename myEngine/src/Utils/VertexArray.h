#ifndef _VERTEX_ARRAY_H
#define _VERTEX_ARRAY_H

#include <queue>
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexBuffer;
class VertexBufferLayout;

namespace MyEngine
{
	class VertexArray
	{
	public:
		VertexArray();
		virtual ~VertexArray();

		void Bind() const;
		void UnBind() const;

		void AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);

		unsigned int vao = 0;
	
	private:
		std::queue<unsigned int> assignedAttribs;
	};
}
#endif // !_VERTEX_ARRAY_H

