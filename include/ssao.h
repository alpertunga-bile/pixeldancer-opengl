#pragma once

#include "shader.h"
#include "shapeFactory.h"

#include <random>

namespace pixeldancer
{
	class SSAO
	{
	public:
		SSAO()
		{
			kernelSize = 64;
			noiseSize = 4;
			width = WIDTH;
			height = HEIGHT;

			this->gbuffer = 0;
			this->gbuffer_position = 0;
			this->gbuffer_normal = 0;
			this->gbuffer_color = 0;
			this->renderbuffer_depth = 0;
			this->framebuffer_ssao = 0;
			this->texture_ssao = 0;
			this->framebuffer_ssaoBlur = 0;
			this->texture_ssaoBlur = 0;
			this->texture_noise = 0;
		}

		~SSAO()
		{
			if (framebuffer_ssao != 0)
				glDeleteFramebuffers(1, &framebuffer_ssao);
			if (framebuffer_ssaoBlur != 0)
				glDeleteFramebuffers(1, &framebuffer_ssaoBlur);
			if (gbuffer != 0)
				glDeleteFramebuffers(1, &framebuffer_ssao);

			if (renderbuffer_depth != 0)
				glDeleteRenderbuffers(1, &renderbuffer_depth);

			if (gbuffer_position != 0)
				glDeleteTextures(1, &gbuffer_position);
			if (gbuffer_normal != 0)
				glDeleteTextures(1, &gbuffer_normal);
			if (gbuffer_color != 0)
				glDeleteTextures(1, &gbuffer_color);

			if (texture_ssao != 0)
				glDeleteTextures(1, &texture_ssao);

			if (texture_noise != 0)
				glDeleteTextures(1, &texture_noise);


			if(!ssaoKernel.empty())
				ssaoKernel.clear();
			if(!ssaoNoise.empty())
				ssaoNoise.clear();
		}

		SSAO(unsigned int gbuffer, unsigned int gbuffer_position, unsigned int gbuffer_normal, unsigned int gbuffer_color,
			unsigned int renderbuffer_depth, unsigned int framebuffer_ssao, unsigned int texture_ssao,
			unsigned int framebuffer_ssaoBlur, unsigned int texture_ssaoBlur, unsigned int texture_noise)
		{
			this->gbuffer = gbuffer;
			this->gbuffer_position = gbuffer_position;
			this->gbuffer_normal = gbuffer_normal;
			this->gbuffer_color = gbuffer_color;
			this->renderbuffer_depth = renderbuffer_depth;
			this->framebuffer_ssao = framebuffer_ssao;
			this->texture_ssao = texture_ssao;
			this->framebuffer_ssaoBlur = framebuffer_ssaoBlur;
			this->texture_ssaoBlur = texture_ssaoBlur;
			this->texture_noise = texture_noise;

			kernelSize = 64;
			noiseSize = 4;
			width = WIDTH;
			height = HEIGHT;
		}

		void initialize(int kernel_size = 64, int noise_size = 4, int width = WIDTH, int height = HEIGHT)
		{
			kernelSize = kernel_size;
			noiseSize = noise_size;
			this->width = width;
			this->height = height;

			///////////////////////////////////////////////////////////////////////////////////////////////////
			// gbuffer framebuffer
			///////////////////////////////////////////////////////////////////////////////////////////////////

			glGenFramebuffers(1, &gbuffer);
			
			glBindFramebuffer(GL_FRAMEBUFFER, gbuffer);

			glGenTextures(1, &gbuffer_position);
			glBindTexture(GL_TEXTURE_2D, gbuffer_position);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gbuffer_position, 0);

			glGenTextures(1, &gbuffer_normal);
			glBindTexture(GL_TEXTURE_2D, gbuffer_normal);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gbuffer_normal, 0);

			glGenTextures(1, &gbuffer_color);
			glBindTexture(GL_TEXTURE_2D, gbuffer_color);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gbuffer_color, 0);

			// tell OpenGL which textures we want to use with this framebuffer
			unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
			glDrawBuffers(3, attachments);

			glGenRenderbuffers(1, &renderbuffer_depth);
			glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_depth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
			

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer_depth);

			if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				SHOWRED("gBuffer Framebuffer cant initialized");

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			///////////////////////////////////////////////////////////////////////////////////////////////////
			// ssao framebuffer
			///////////////////////////////////////////////////////////////////////////////////////////////////

			glGenFramebuffers(1, &framebuffer_ssao);
			

			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_ssao);

			glGenTextures(1, &texture_ssao);
			glBindTexture(GL_TEXTURE_2D, texture_ssao);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_ssao, 0);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				SHOWRED("SSAO Framebuffer cant initialized");

			///////////////////////////////////////////////////////////////////////////////////////////////////
			// ssao blur framebuffer
			///////////////////////////////////////////////////////////////////////////////////////////////////

			glGenFramebuffers(1, &framebuffer_ssaoBlur);
			

			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_ssaoBlur);

			glGenTextures(1, &texture_ssaoBlur);
			glBindTexture(GL_TEXTURE_2D, texture_ssaoBlur);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_ssaoBlur, 0);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				SHOWRED("SSAO Blur Framebuffer cant initialized");

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			///////////////////////////////////////////////////////////////////////////////////////////////////
			// generate ssao kernel
			// in tangent space
			// normal vector pointing +z position
			///////////////////////////////////////////////////////////////////////////////////////////////////
			
			// number generator
			// generates numbers between 0 and 1
			std::uniform_real_distribution<GLfloat> randomFloats;
			std::default_random_engine generator;

			for (unsigned int i = 0; i < kernel_size; ++i)
			{
				glm::vec3 sample(
					randomFloats(generator) * 2.0 - 1.0,
					randomFloats(generator) * 2.0 - 1.0,
					randomFloats(generator)
					);
				sample = glm::normalize(sample);
				sample *= randomFloats(generator);
				float scale = float(i) / kernel_size;

				scale = lerp(0.1f, 1.0f, scale * scale);
				sample *= scale;
				ssaoKernel.push_back(sample);
			}

			///////////////////////////////////////////////////////////////////////////////////////////////////
			// generate random kernel rotations
			// for get good result with less sample size
			// leave z 0.0 for rotate around z axis
			///////////////////////////////////////////////////////////////////////////////////////////////////

			for (unsigned int i = 0; i < noise_size * noise_size; i++)
			{
				glm::vec3 noise(
					randomFloats(generator) * 2.0 - 1.0,
					randomFloats(generator) * 2.0 - 1.0,
					0.0f
				);

				ssaoNoise.push_back(noise);
			}

			///////////////////////////////////////////////////////////////////////////////////////////////////
			// noise texture
			///////////////////////////////////////////////////////////////////////////////////////////////////

			glGenTextures(1, &texture_noise);
			glBindTexture(GL_TEXTURE_2D, texture_noise);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, noise_size, noise_size, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			

			std::cout << "SSAO Shader: " << std::endl;
			Shader ssao("shader/ssao/ssao.vs", "shader/ssao/ssao.ps");
			ssaoShader = ssao;

			std::cout << "SSAO Blur Shader: " << std::endl;
			Shader ssaoBlur("shader/ssao/ssao.vs", "shader/ssao/ssaoBlur.ps");
			ssaoBlurShader = ssaoBlur;

			SHOWGREEN("SSAO Framebuffers initialized");
		}

		Shader getSceneShader()
		{
			std::cout << "SSAO Scene Shader: " << std::endl;
			return Shader("shader/ssao/ssaoScene.vs", "shader/ssao/ssaoScene.ps");
		}

		void render(glm::mat4 proj_matrix, float radius = 0.5)
		{
			///////////////////////////////////////////////////////////////////////////////////////////////////
			// generate SSAO texture
			///////////////////////////////////////////////////////////////////////////////////////////////////

			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_ssao);

			glClear(GL_COLOR_BUFFER_BIT);

			ssaoShader.load();

			ssaoShader.setInt("kernel_size", kernelSize);
			ssaoShader.setInt("noise_size", noiseSize);
			ssaoShader.setInt("width", width);
			ssaoShader.setInt("height", height);
			ssaoShader.setFloat("radius", radius);

			for (unsigned int i = 0; i < kernelSize; ++i)
				ssaoShader.setVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);

			ssaoShader.setMat4("proj_matrix", proj_matrix);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gbuffer_position);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, gbuffer_normal);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, texture_noise);

			factory.renderQuad();

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			///////////////////////////////////////////////////////////////////////////////////////////////////
			// SSAO Blur to remove noise
			///////////////////////////////////////////////////////////////////////////////////////////////////

			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_ssaoBlur);

			glClear(GL_COLOR_BUFFER_BIT);

			ssaoBlurShader.load();

			ssaoBlurShader.setInt("noise_size", noiseSize);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_ssaoBlur);

			factory.renderQuad();

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

	private:
		float lerp(float a, float b, float f)
		{
			return a + f * (b - a);
		}

	public:
		// gbuffer framebuffer variables
		unsigned int gbuffer;
		unsigned int gbuffer_position;
		unsigned int gbuffer_normal;
		unsigned int gbuffer_color;
		unsigned int renderbuffer_depth;

		// ssao framebuffer variables
		unsigned int framebuffer_ssao;
		unsigned int texture_ssao;

		// ssao blur framebuffer variables
		unsigned int framebuffer_ssaoBlur;
		unsigned int texture_ssaoBlur;

		// ssao variables
		unsigned int texture_noise;

	private:
		// hemispheres
		std::vector<glm::vec3> ssaoKernel;

		// noise
		std::vector<glm::vec3> ssaoNoise;

		int kernelSize, noiseSize, width, height;

		Shader ssaoShader;
		Shader ssaoBlurShader;

		ShapeFactory factory;
	};
}