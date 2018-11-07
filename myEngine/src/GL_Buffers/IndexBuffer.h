#ifndef _MYENGINE_INDEXBUFFER_H
#define _MYENGINE_INDEXBUFFER_H

#include "GLBuffer.h"

#include <vector>

namespace MyEngine {
	class IndexBuffer : public GLBuffer
	{
	public:
		IndexBuffer(const std::vector<unsigned int>* indices);
		virtual ~IndexBuffer();

		void Bind() const;
		void UnBind() const;
		void Draw() const;
		void DrawLines(float line_width) const;
		void DrawPoints(float point_size) const;
		void DrawWires() const;

		unsigned int ibo = 0;
		unsigned int elements = 0;
	};
}

#endif // !_MYENGINE_INDEXBUFFER_H
