#ifndef _MYENGINE_INDEXBUFFER_H
#define _MYENGINE_INDEXBUFFER_H

#include <vector>

namespace MyEngine {
	class IndexBuffer
	{
	public:
		IndexBuffer(const std::vector<unsigned int>* indices);
		virtual ~IndexBuffer();

		void Bind() const;
		void UnBind() const;
		void Draw() const;
		void DrawLines(float line_width) const;

		unsigned int ibo = 0;
		unsigned int elements = 0;
	};
}

#endif // !_MYENGINE_INDEXBUFFER_H
