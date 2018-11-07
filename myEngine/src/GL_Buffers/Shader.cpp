#include "Shader.h"

#include <assert.h>
#include <vector>
#include "GL/glew.h"

#include "Globals.h"
#include "Utils/Render_Utils.h"

#include "_Vendor/MathGeoLib/Math/float4x4.h"
#include "_Vendor/MathGeoLib/Math/float4.h"

namespace MyEngine 
{
	Shader::Shader(const char* vertex_file, const char* fragment_file)
	{
		assert(vertex_file && fragment_file);
		unsigned int vShader = CompileShader(vertex_file, (int)GL_VERTEX_SHADER);
		unsigned int fShader = CompileShader(fragment_file, (int)GL_FRAGMENT_SHADER);

		if (vShader != 0 && fShader != 0) 
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
		if (Globals::active_shader == program) return;

		GLCall(glUseProgram(program));
		Globals::active_shader = program;
	}


	void Shader::UnBind() const
	{
		if (Globals::active_shader != program) return;

		GLCall(glUseProgram(0));
		Globals::active_shader = 0;
	}


	const char* Shader::ReadFile(const char* filename) const
	{
		assert(filename);
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

	unsigned int Shader::CompileShader(const char* filename, int type) const
	{
		assert(filename);
		int success = 0;

		const char* shaderData = ReadFile(filename);
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
			return 0;
		}
		else LOGINFO("Shader compiled: %s", filename);
		
		delete shaderData;

		return shader;
	}

	void Shader::SetUniform4x4(const char* name, math::float4x4& entry) const
	{
		assert(name);
		this->Bind(); 
		GLCall(glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_TRUE, &(entry)[0][0]));
	}

	void Shader::SetUniform4(const char* name, math::float4& entry) const
	{
		assert(name);
		this->Bind();
		GLCall(glUniform4f(glGetUniformLocation(program, name), entry.x, entry.y, entry.z, entry.w));
	}

	void Shader::SetUniform4(const char* name, float x, float y, float z, float w) const
	{
		this->Bind();
		GLCall(glUniform4f(glGetUniformLocation(program, name), x, y, z, w));
	}

	void Shader::SetUniform1(const char * name, int value) const
	{
		this->Bind();
		GLCall(glUniform1i(glGetUniformLocation(program, name), value));
	}

	/*void Shader::AddTexture2D(unsigned int textureID)
	{
		if (textures.find(textureID) != textures.end())
		{
			LOGWARNING("Texture %i already loaded in shader.", textureID);
			return;
		}

		this->Bind();

		GLCall(glActiveTexture(GL_TEXTURE0 + textures.size()));
		GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
		
		std::string uniformName = std::string("texture") + std::to_string(textures.size());
		GLCall(int location = glGetUniformLocation(program, uniformName.c_str()));

		if (location == -1)
			LOGWARNING("Cannot find Uniform %s in this shader.", uniformName);

		GLCall(glUniform1i(location, textures.size()));
		textures.insert(textureID);
	}*/

	void Shader::EnableTexture2D(unsigned int textureID)
	{
		Bind();

		GLCall(glActiveTexture(GL_TEXTURE1));
		GLCall(glBindTexture(GL_TEXTURE_2D, textureID));

		GLCall(int location = glGetUniformLocation(program, "texture0"));
		if (location == -1) LOGWARNING("Cannot find Uniform texture0 in this shader.");

		GLCall(glUniform1i(location, 1));
	}

	void Shader::DisableTexture2D()
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		
	}

	void Shader::RemoveTexture2D(unsigned int textureID)
	{
	}
}
