#include "Shader.h"

#include <fstream>

#include "GL/glew.h"
#include "Render_Utils.h"

#include "../_Vendor/MathGeoLib/Math/float4x4.h"

namespace MyEngine 
{
	Shader::Shader(const char* vertex_file, const char* fragment_file)
	{
		unsigned int vShader = CompileShader(vertex_file, (int)GL_VERTEX_SHADER);
		unsigned int fShader = CompileShader(fragment_file, (int)GL_FRAGMENT_SHADER);

		if (valid) 
		{
			program = glCreateProgram();
			GLCall(glAttachShader(program, vShader));
			GLCall(glAttachShader(program, fShader));
			GLCall(glLinkProgram(program));

			int success = 0;
			glGetProgramiv(program, GL_LINK_STATUS, &success);

			if (success == GL_FALSE) 
			{
				LOGERROR("Program failed to link: %s & %s", vertex_file, fragment_file);
			}
			else LOGINFO("Program successfully linked: %s & %s", vertex_file, fragment_file);
		}

		GLCall(glDetachShader(program, vShader));
		GLCall(glDetachShader(program, fShader));

		if (vShader != 0) GLCall(glDeleteShader(vShader));
		if (fShader != 0) GLCall(glDeleteShader(fShader));

		// Set Unifrom Block Index to binding point 0
		unsigned int index = glGetUniformBlockIndex(program, "GlobalMatrices");
		GLCall(glUniformBlockBinding(program, index, 0));
	}


	Shader::~Shader()
	{
		GLCall(glDeleteProgram(program));
	}


	void Shader::Bind() const
	{
		GLCall(glUseProgram(program));
	}


	void Shader::UnBind() const
	{
		GLCall(glUseProgram(0));
	}


	char* Shader::ReadFile(const char* filename)
	{
		char* data = nullptr;
		FILE* file = nullptr;
		fopen_s(&file, filename, "rb");

		if (file)
		{
			fseek(file, 0, SEEK_END);
			int size = ftell(file);
			rewind(file);
			data = (char*)malloc(size + 1);

			fread(data, 1, size, file);
			data[size] = 0;

			fclose(file);
		}
		else LOGERROR("File not found: %s", filename);

		return data;
	}

	unsigned int Shader::CompileShader(const char* filename, int type)
	{
		int success = 0;

		char* shaderData = ReadFile(filename);
		if (shaderData == nullptr) 
		{
			LOGERROR("Could not load %s", filename);
			return 0;
		}

		unsigned shader = glCreateShader( (GLenum)type );
		GLCall(glShaderSource(shader, 1, &shaderData, NULL));
		GLCall(glCompileShader(shader));
		GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			const char* msg = infoLog.data();
			LOGERROR("Could not compile shader: %s. << %s >>", filename, msg);
			valid = false;
		}
		else LOGINFO("Shader compiled: %s", filename);
		
		delete shaderData;

		return shader;
	}

	void Shader::SetUniform4x4(const char* name, math::float4x4* entry)
	{
		this->Bind();
		GLCall(glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_TRUE, &(*entry)[0][0]));
	}
}
