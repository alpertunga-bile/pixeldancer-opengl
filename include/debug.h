#pragma once

#include "shader.h"
#include "shapeFactory.h"

GLenum checkError(const char* file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM: 
			error = "INVALID_ENUM"; 
			break;
		case GL_INVALID_VALUE: 
			error = "INVALID_VALUE"; 
			break;
		case GL_INVALID_OPERATION:
			error = "INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			error = "STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			error = "STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			error = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		}
		SHOWRED("::OPENGL ERROR::");
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}

	return errorCode;
}
#define glCheckError() checkError(__FILE__, __LINE__)

namespace pixeldancer
{
	void debugTexture(unsigned int texture, unsigned int texture0, Shader& debugShader, ShapeFactory& factory, int width = WIDTH, int height = HEIGHT)
	{
		int vwidth = width / 4;
		int vheight = vwidth * ((float)height / (float)width);
		int wstart = width - vwidth;
		int hstart = height - vheight;

		glViewport(wstart, hstart, vwidth, vheight);
		debugShader.load();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		factory.renderQuad();

		glViewport(0, 0, width, height);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
	}
}