#pragma once

#include "debug.h"

namespace pixeldancer
{
	class Pipeline
	{
	public:
		Pipeline(std::vector<std::string> shaders) 
		{
			int i;
			const int VECTOR_SIZE = shaders.size();
			const int PIPE_SIZE = VECTOR_SIZE - 1;

			std::string temp;

			std::ifstream file;
			file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			for (i = 0; i < VECTOR_SIZE; i++)
			{
				try
				{
					file.open(shaders[i]);

					std::stringstream stream;

					stream << file.rdbuf();

					temp = stream.str();

					const GLchar* const tmp = temp.c_str();

					codes.push_back(tmp);

					file.close();
				}
				catch (std::ifstream::failure e)
				{
					std::cout << "ERROR Can't Read " << shaders[i] << std::endl;
				}
			}

			for (i = 0; i < VECTOR_SIZE; i++)
			{
				if (i == 0)
				{
					programs.push_back(glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &codes[i]));
					checkProgram(programs[i], shaders[i]);
				}

				programs.push_back(glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &codes[i]));
				checkProgram(programs[i], shaders[i]);
			}

			glCreateProgramPipelines(PIPE_SIZE, &pipelines[0]);

			for (i = 0; i < PIPE_SIZE; i++)
			{
				glUseProgramStages(pipelines[i], GL_VERTEX_SHADER_BIT, programs[0]);
				glUseProgramStages(pipelines[i], GL_FRAGMENT_SHADER_BIT, programs[i + 1]);
			}
		}

		void bind(const int index = 0)
		{
			glUseProgram(0);

			glBindProgramPipeline(pipelines[index]);
		}

		void setInt(const std::string& name, const int index, int value) const
		{
			glProgramUniform1i(programs[index], glGetUniformLocation(programs[index], name.c_str()), (int)value);
		}

		void setFloat(const std::string& name, const int index, float value) const
		{
			glProgramUniform1f(programs[index], glGetUniformLocation(programs[index], name.c_str()), value);
		}

		void setVec2(const std::string& name, const int index, glm::vec2 vec2) const
		{
			glProgramUniform2fv(programs[index], glGetUniformLocation(programs[index], name.c_str()), 1, glm::value_ptr(vec2));
		}

		void setVec3(const std::string& name, const int index, glm::vec3 vec3) const
		{
			glProgramUniform3fv(programs[index], glGetUniformLocation(programs[index], name.c_str()), 1, glm::value_ptr(vec3));
		}

		void setVec4(const std::string& name, const int index, glm::vec4 vec4) const
		{
			glProgramUniform4fv(programs[index], glGetUniformLocation(programs[index], name.c_str()), 1, glm::value_ptr(vec4));
		}

		void setMat4(const std::string& name, const int index, glm::mat4 mat)
		{
			glProgramUniformMatrix4fv(programs[index], glGetUniformLocation(programs[index], name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
		}

	private:
		void checkProgram(unsigned int object_num, std::string name)
		{
			int success;
			char infoLog[512];
			glGetProgramiv(object_num, GL_LINK_STATUS, &success);

			if (!success)
			{
				SHOWRED("Program Can't Loaded Successfully");
				std::cout << name << std::endl;
				glGetProgramInfoLog(object_num, 512, NULL, infoLog);
				std::cout << "::ERROR::" << name << "\n" << infoLog << std::endl;
			}
		}

	private:
		std::vector<const GLchar*> codes;
		std::vector<GLuint> programs;
		std::vector<GLuint> pipelines;
	};
}