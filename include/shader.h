#pragma once

#include "helper.h"

#include <fstream>
#include <sstream>

namespace pixeldancer
{
	class Shader
	{
	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// default
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Shader()
		{
			ID = 0;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// compute
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Shader(const char* computePath)
		{
			std::string computeCode;

			std::ifstream computeFile;

			computeFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			try
			{
				computeFile.open(computePath);

				std::stringstream computeStream;

				computeStream << computeFile.rdbuf();

				computeCode = computeStream.str();

				computeFile.close();
			}
			catch (std::ifstream::failure e)
			{
				std::cout << "ERROR Can't Read File" << std::endl;
			}

			const char* computeShaderCode = computeCode.c_str();

			unsigned int compute;

			compute = glCreateShader(GL_COMPUTE_SHADER);
			glShaderSource(compute, 1, &computeShaderCode, NULL);
			glCompileShader(compute);
			IfFailed(compute, computePath);

			ID = glCreateProgram();
			glAttachShader(ID, compute);
			glLinkProgram(ID);
			IfFailed(ID, "shader program");

			SHOWGREEN("Shaders Loaded Successfully");

			glDeleteShader(compute);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// vertex & fragment
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Shader(const char* vertPath, const char* fragPath)
		{
			std::string vertCode;
			std::string fragCode;

			std::ifstream vertFile;
			std::ifstream fragFile;

			vertFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			// read in file
			try
			{
				vertFile.open(vertPath);
				fragFile.open(fragPath);

				std::stringstream vertStream, fragStream;

				vertStream << vertFile.rdbuf();
				fragStream << fragFile.rdbuf();

				vertCode = vertStream.str();
				fragCode = fragStream.str();

				vertFile.close();
				fragFile.close();
			}
			catch (std::ifstream::failure e)
			{
				std::cout << "ERROR Can't Read File" << std::endl;
			}

			const char* vertShaderCode = vertCode.c_str();
			const char* fragShaderCode = fragCode.c_str();

			// compiling
			unsigned int vertex, fragment;

			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex, 1, &vertShaderCode, NULL);
			glCompileShader(vertex);
			IfFailed(vertex, vertPath);

			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &fragShaderCode, NULL);
			glCompileShader(fragment);
			IfFailed(fragment, fragPath);

			// shader program
			ID = glCreateProgram();
			glAttachShader(ID, vertex);
			glAttachShader(ID, fragment);
			glLinkProgram(ID);
			IfFailed(ID, "shader program");

			SHOWGREEN("Shaders Loaded Successfully");
			
			glDeleteShader(vertex);
			glDeleteShader(fragment);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// vertex & geometry & fragment
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Shader(const char* vertPath, const char* geoPath, const char* fragPath)
		{
			std::string vertCode;
			std::string geoCode;
			std::string fragCode;

			std::ifstream vertFile;
			std::ifstream geoFile;
			std::ifstream fragFile;

			vertFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			geoFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			// read in file
			try
			{
				vertFile.open(vertPath);
				geoFile.open(geoPath);
				fragFile.open(fragPath);

				std::stringstream vertStream, geoStream, fragStream;

				vertStream << vertFile.rdbuf();
				geoStream << geoFile.rdbuf();
				fragStream << fragFile.rdbuf();

				vertFile.close();
				geoFile.close();
				fragFile.close();

				vertCode = vertStream.str();
				geoCode = geoStream.str();
				fragCode = fragStream.str();
			}
			catch (std::ifstream::failure e)
			{
				std::cout << "ERROR Can't Read File" << std::endl;
			}

			const char* vertShaderCode = vertCode.c_str();
			const char* geoShaderCode = geoCode.c_str();
			const char* fragShaderCode = fragCode.c_str();

			// compiling
			unsigned int vertex, geometry, fragment;

			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex, 1, &vertShaderCode, NULL);
			glCompileShader(vertex);
			IfFailed(vertex, vertPath);

			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &geoShaderCode, NULL);
			glCompileShader(geometry);
			IfFailed(geometry, geoPath);

			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &fragShaderCode, NULL);
			glCompileShader(fragment);
			IfFailed(fragment, fragPath);

			// shader program
			ID = glCreateProgram();
			glAttachShader(ID, vertex);
			glAttachShader(ID, geometry);
			glAttachShader(ID, fragment);
			glLinkProgram(ID);
			IfFailed(ID, "shader program");

			SHOWGREEN("Shaders Loaded Successfully");
		
			glDeleteShader(vertex);
			glDeleteShader(geometry);
			glDeleteShader(fragment);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// vertex & tessControl & tessEvaluation & fragment
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Shader(const char* vertPath, const char* tessControlPath, const char* tessEvaluationPath, const char* fragPath)
		{
			std::string vertCode;
			std::string tessControlCode;
			std::string tessEvaluationCode;
			std::string fragCode;

			std::ifstream vertFile;
			std::ifstream tessControlFile;
			std::ifstream tessEvaluationFile;
			std::ifstream fragFile;

			vertFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			tessControlFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			tessEvaluationFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			// read in file
			try
			{
				vertFile.open(vertPath);
				tessControlFile.open(tessControlPath);
				tessEvaluationFile.open(tessEvaluationPath);
				fragFile.open(fragPath);

				std::stringstream vertStream, tessControlStream, tessEvaluationStream, fragStream;

				vertStream << vertFile.rdbuf();
				tessControlStream << tessControlFile.rdbuf();
				tessEvaluationStream << tessEvaluationFile.rdbuf();
				fragStream << fragFile.rdbuf();

				vertFile.close();
				tessControlFile.close();
				tessEvaluationFile.close();
				fragFile.close();

				vertCode = vertStream.str();
				tessControlCode = tessControlStream.str();
				tessEvaluationCode = tessEvaluationStream.str();
				fragCode = fragStream.str();
			}
			catch (std::ifstream::failure e)
			{
				std::cout << "ERROR Can't Read File" << std::endl;
			}

			const char* vertShaderCode = vertCode.c_str();
			const char* tessControlShaderCode = tessControlCode.c_str();
			const char* tessEvaluationShaderCode = tessEvaluationCode.c_str();
			const char* fragShaderCode = fragCode.c_str();

			// compiling
			unsigned int vertex, tessControl, tessEvaluation, fragment;

			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex, 1, &vertShaderCode, NULL);
			glCompileShader(vertex);
			IfFailed(vertex, vertPath);

			tessControl = glCreateShader(GL_TESS_CONTROL_SHADER);
			glShaderSource(tessControl, 1, &tessControlShaderCode, NULL);
			glCompileShader(tessControl);
			IfFailed(tessControl, tessControlPath);

			tessEvaluation = glCreateShader(GL_TESS_EVALUATION_SHADER);
			glShaderSource(tessEvaluation, 1, &tessEvaluationShaderCode, NULL);
			glCompileShader(tessEvaluation);
			IfFailed(tessEvaluation, tessEvaluationPath);

			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &fragShaderCode, NULL);
			glCompileShader(fragment);
			IfFailed(fragment, fragPath);

			// shader program
			ID = glCreateProgram();
			glAttachShader(ID, vertex);
			glAttachShader(ID, tessControl);
			glAttachShader(ID, tessEvaluation);
			glAttachShader(ID, fragment);
			glLinkProgram(ID);
			IfFailed(ID, "shader program");

			SHOWGREEN("Shaders Loaded Successfully");

			glDeleteShader(vertex);
			glDeleteShader(tessControl);
			glDeleteShader(tessEvaluation);
			glDeleteShader(fragment);	
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// vertex & tessControl & tessEvaluation & geometry & fragment
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Shader(const char* vertPath, const char* tessControlPath, const char* tessEvaluationPath, const char* geoPath, const char* fragPath)
		{
			std::string vertCode;
			std::string tessControlCode;
			std::string tessEvaluationCode;
			std::string geoCode;
			std::string fragCode;

			std::ifstream vertFile;
			std::ifstream tessControlFile;
			std::ifstream tessEvaluationFile;
			std::ifstream geoFile;
			std::ifstream fragFile;

			vertFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			tessControlFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			tessEvaluationFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			geoFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			// read in file
			try
			{
				vertFile.open(vertPath);
				tessControlFile.open(tessControlPath);
				tessEvaluationFile.open(tessEvaluationPath);
				geoFile.open(geoPath);
				fragFile.open(fragPath);

				std::stringstream vertStream, tessControlStream, tessEvaluationStream, geoStream, fragStream;

				vertStream << vertFile.rdbuf();
				tessControlStream << tessControlFile.rdbuf();
				tessEvaluationStream << tessEvaluationFile.rdbuf();
				geoStream << geoFile.rdbuf();
				fragStream << fragFile.rdbuf();

				vertFile.close();
				tessControlFile.close();
				tessEvaluationFile.close();
				geoFile.close();
				fragFile.close();

				vertCode = vertStream.str();
				tessControlCode = tessControlStream.str();
				tessEvaluationCode = tessEvaluationStream.str();
				geoCode = geoStream.str();
				fragCode = fragStream.str();
			}
			catch (std::ifstream::failure e)
			{
				std::cout << "ERROR Can't Read File" << std::endl;
			}

			const char* vertShaderCode = vertCode.c_str();
			const char* tessControlShaderCode = tessControlCode.c_str();
			const char* tessEvaluationShaderCode = tessEvaluationCode.c_str();
			const char* geoShaderCode = geoCode.c_str();
			const char* fragShaderCode = fragCode.c_str();

			// compiling
			unsigned int vertex, tessControl, tessEvaluation, geometry, fragment;

			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex, 1, &vertShaderCode, NULL);
			glCompileShader(vertex);
			IfFailed(vertex, vertPath);

			tessControl = glCreateShader(GL_TESS_CONTROL_SHADER);
			glShaderSource(tessControl, 1, &tessControlShaderCode, NULL);
			glCompileShader(tessControl);
			IfFailed(tessControl, tessControlPath);

			tessEvaluation = glCreateShader(GL_TESS_EVALUATION_SHADER);
			glShaderSource(tessEvaluation, 1, &tessEvaluationShaderCode, NULL);
			glCompileShader(tessEvaluation);
			IfFailed(tessEvaluation, tessEvaluationPath);

			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &geoShaderCode, NULL);
			glCompileShader(geometry);
			IfFailed(geometry, geoPath);

			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &fragShaderCode, NULL);
			glCompileShader(fragment);
			IfFailed(fragment, fragPath);

			// shader program
			ID = glCreateProgram();
			glAttachShader(ID, vertex);
			glAttachShader(ID, tessControl);
			glAttachShader(ID, tessEvaluation);
			glAttachShader(ID, geometry);
			glAttachShader(ID, fragment);
			glLinkProgram(ID);
			IfFailed(ID, "shader program");

			SHOWGREEN("Shaders Loaded Successfully");
		
			glDeleteShader(vertex);
			glDeleteShader(tessControl);
			glDeleteShader(tessEvaluation);
			glDeleteShader(geometry);
			glDeleteShader(fragment);
		}

		~Shader()
		{
			if (ID != 0)
				glDeleteProgram(ID);
		}

	public:

		unsigned int ID;

	public:

		Shader& operator=(Shader& shader) { 
			ID = shader.ID;

			return *this;
		}

		void load()
		{
			glUseProgram(ID);
		}

		void setBool(const std::string& name, bool value) const
		{
			glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
		}

		void setInt(const std::string& name, int value) const
		{
			glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
		}

		void setFloat(const std::string& name, float value) const
		{
			glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
		}

		void setMat4(const std::string& name, glm::mat4 matrix) const
		{
			/////////////////////////////////////////////////////////////////////////////
			///// glUniformMatrix4fv();
			// 1. how many matrices we want to send
			// 2. want to transpose
			// 3. actual data
			/////////////////////////////////////////////////////////////////////////////

			glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
		}

		void setVec2(const std::string& name, glm::vec2 vector2) const
		{
			glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vector2));
		}

		void setVec3(const std::string& name, glm::vec3 vector3) const
		{
			glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vector3));
		}

		void setVec4(const std::string& name, glm::vec4 vector4) const
		{
			glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vector4));
		}

	private:
		void IfFailed(unsigned int object_num, std::string name)
		{
			int success;
			const std::string shader_program = "shader program";
			char infoLog[512];
			int isProgram = shader_program.compare(name);
			if (isProgram == 0)
				glGetProgramiv(object_num, GL_LINK_STATUS, &success);
			else
				glGetShaderiv(object_num, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				if (isProgram == 0)
				{
					SHOWRED("Program Can't Loaded Successfully");
					glGetProgramInfoLog(object_num, 512, NULL, infoLog);
					std::cout << "::ERROR::" << name << "\n" << infoLog << std::endl;
				}
				else
				{
					SHOWRED("Shaders Can't Loaded Successfully");
					glGetShaderInfoLog(object_num, 512, NULL, infoLog);
					std::cout << "::ERROR::" << name << "\n" << infoLog << std::endl;
				}
			}
		}
	};

	class UniformBuffer
	{
	public:
		UniformBuffer()
		{
			size = 0;
			ubo = 0;
		}

		GLint getSize(Shader& shader, std::string uniformName)
		{
			GLuint blockIndex = glGetUniformBlockIndex(shader.ID, uniformName.c_str());

			glGetActiveUniformBlockiv(shader.ID, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &size);

			return size;
		}

		GLint* getOffset(Shader& shader, const GLchar* names[], int arrSize)
		{
			GLuint* indices = (GLuint*)malloc(arrSize);
			glGetUniformIndices(shader.ID, arrSize, names, indices);

			GLint* offset = (GLint*)malloc(arrSize);
			glGetActiveUniformsiv(shader.ID, arrSize, indices, GL_UNIFORM_OFFSET, offset);

			return offset;

			free(indices);
			free(offset);
		}

		void createBuffer(GLubyte* buffer)
		{
			glGenBuffers(1, &ubo);
			glBindBuffer(GL_UNIFORM_BUFFER, ubo);
			glBufferData(GL_UNIFORM_BUFFER, size, buffer, GL_DYNAMIC_DRAW);
		}

		void bind(int binding = 0)
		{
			glBindBufferBase(GL_UNIFORM_BUFFER, binding, ubo);
		}

		~UniformBuffer()
		{
			if (ubo != 0)
				glDeleteBuffers(1, &ubo);
		}

	private:
		GLint size;
		GLuint ubo;
	};
}