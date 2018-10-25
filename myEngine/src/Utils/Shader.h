#ifndef _MYENGINE_SHADER_H
#define _MYENGINE_SHADER_H

#include "LogSystem.h"

namespace MyEngine
{
	class Shader 
	{
	public:
		Shader(const char* vShader_file, const char* fShader_file);
		virtual ~Shader();

		void Bind() const;
		void UnBind() const;

		unsigned int program = 0;

	private:
		char* ReadFile(const char* filename);
		unsigned int CompileShader(const char* filename, int type);

		bool valid = true;
	};

}

extern MyEngine::LogSystem* logger;
#endif // !_MYENGINE_SHADER_H

