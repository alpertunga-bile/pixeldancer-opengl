#pragma once

#include "shader.h"
#include "shapeFactory.h"

namespace pixeldancer
{
	class HDR
	{
	public:
		HDR()
		{
			framebuffer_hdr = 0;
			texture_hdr = 0;
			renderbuffer_depth = 0;
		}

		HDR(unsigned int framebuffer, unsigned int texture, unsigned int renderbuffer)
		{
			framebuffer_hdr = framebuffer;
			texture_hdr = texture;
			renderbuffer_depth = renderbuffer;
		}

		void initialize(int width = WIDTH, int height = HEIGHT)
		{
			glGenFramebuffers(1, &framebuffer_hdr);

			glGenTextures(1, &texture_hdr);
			glBindTexture(GL_TEXTURE_2D, texture_hdr);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
			glGenRenderbuffers(1, &renderbuffer_depth);
			glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_depth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
			

			// attaching buffers to framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_hdr);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_hdr, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer_depth);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				SHOWRED("HDR Framebuffer cant initialized"); 

			SHOWGREEN("HDR Framebuffer initialized");

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			Shader hdrShader("shader/hdr/hdr.vs", "shader/hdr/hdr.ps");
			HDRShader = hdrShader;
		}

		void render(float exposure = 1.0f)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			HDRShader.load();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_hdr);
			HDRShader.setFloat("exposure", exposure);

			factory.renderQuad();
		}

		void render(Shader& shader, float exposure = 1.0f)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shader.load();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_hdr);
			shader.setFloat("exposure", exposure);

			factory.renderQuad();
		}

		~HDR()
		{
			if (framebuffer_hdr != 0)
				glDeleteFramebuffers(1, &framebuffer_hdr);

			if (texture_hdr != 0)
				glDeleteTextures(1, &texture_hdr);

			if (renderbuffer_depth != 0)
				glDeleteRenderbuffers(1, &renderbuffer_depth);
		}

	public:
		unsigned int framebuffer_hdr;
		unsigned int texture_hdr;
		unsigned int renderbuffer_depth;

	private:
		Shader HDRShader;
		ShapeFactory factory;
	};
}