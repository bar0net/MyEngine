#ifndef _MYENGINE_INDEXBUFFER_H
#define _MYENGINE_INDEXBUFFER_H

#include <vector>

namespace MyEngine {
	class IndexBuffer
	{
	public:
		IndexBuffer(std::vector<unsigned int>* indices);
		virtual ~IndexBuffer();

		void Bind();
		void UnBind();
		void Draw();

		unsigned int ibo = 0;
		unsigned int elements = 0;
	};
}

#endif // !_MYENGINE_INDEXBUFFER_H
