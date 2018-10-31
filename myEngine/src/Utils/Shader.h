#ifndef _MYENGINE_SHADER_H
#define _MYENGINE_SHADER_H

#include "LogSystem.h"

#include <vector>

#define SHADER_VIEW_OFFSET sizeof(float4x4)
#define SHADER_PROJ_OFFSET 0

namespace math
{
	class float4x4;
	class float4;
}

namespace MyEngine
{
	class Shader 
	{
	public:
		Shader(const char* vShader_file, const char* fShader_file);
		virtual ~Shader();

		void Bind() const;
		void UnBind() const;

		void SetUniform4x4(const char* name, math::float4x4* entry);
		void SetUniform4(const char* name, math::float4* entry);
		void SetUniform4(const char* name, float x, float y, float z, float w);

		void AddTexture2D(unsigned int textureID);

		unsigned int program = 0;

	private:
		char* ReadFile(const char* filename);
		unsigned int CompileShader(const char* filename, int type);

		bool valid = true;
		std::vector<unsigned int> textures;
	};

}

#endif // !_MYENGINE_SHADER_H

