#ifndef _VERTEX_BUFFER_LAYOUT
#define _VERTEX_BUFFER_LAYOUT


#define VBL_Byte 0x1400
#define VBL_Unsigned_Byte 0x1401
#define VBL_Int 0x1404
#define VBL_Unsigned_Int 0x1405
#define VBL_Half_Float 0x140B
#define VBL_Float 0x1406

#include <vector>

namespace MyEngine
{


	struct VertexBufferElement
	{
		unsigned int type = 0U;
		unsigned int count = 0U;
		bool normalized = false;

		static unsigned int SizeOfType(unsigned int type)
		{
			switch (type)
			{
				case VBL_Float: return sizeof(float);
				case VBL_Int: return sizeof(int);
				case VBL_Unsigned_Int: return sizeof(unsigned int);
			}
			__debugbreak();
			return 0U;
		}
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout() {}
		virtual ~VertexBufferLayout() {}

		template<typename T>
		void Push(unsigned int count) { __debugbreak(); }

		template<>
		void Push<float>(unsigned int count)
		{
			elements.push_back({ VBL_Float, count, false });
			stride += sizeof(float) * count;
		}

		template<>
		void Push<int>(unsigned int count)
		{
			elements.push_back({ VBL_Int, count, false });
			stride += sizeof(int) * count;
		}

		template<>
		void Push<unsigned int>(unsigned int count)
		{
			elements.push_back({ VBL_Unsigned_Int, count, false });
			stride += sizeof(unsigned int) * count;
		}

		inline const std::vector<VertexBufferElement> Elements() const& { return elements; }
		inline unsigned int Stride() const& { return stride; }

	private:
		std::vector<VertexBufferElement> elements;
		unsigned int stride = 0U;
	};

}
#endif // !_VERTEX_BUFFER_LAYOUT

