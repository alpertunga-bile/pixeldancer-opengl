#pragma once

#include "texture.h"
#include "shader.h"
#include "shapeFactory.h"

namespace pixeldancer
{
	class PBR
	{
	public:
		PBR()
		{
			framebuffer_capture = 0;
			renderbuffer_capture = 0;
			irradianceMap = 0;
			envCubemap = 0;
			prefilterMap = 0;
			texture_lut = 0;
		}

		void setPBRTextures(Shader& shader, unsigned int albedo, unsigned int normal, unsigned int metallic,
			unsigned int roughness, unsigned int ao)
		{
			shader.load();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, albedo);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, normal);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, metallic);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, roughness);
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, ao);
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
			glActiveTexture(GL_TEXTURE6);
			glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
			glActiveTexture(GL_TEXTURE7);
			glBindTexture(GL_TEXTURE_2D, texture_lut);
		}

		void initializeWPreShaders(std::string texture_hdr)
		{
			Shader toCubemapShader("shader/pbr/toCubemap.vs", "shader/pbr/toCubemap.ps");
			Shader irradianceShader("shader/pbr/toCubemap.vs", "shader/pbr/irradianceMap.ps");
			Shader prefilterShader("shader/pbr/toCubemap.vs", "shader/pbr/prefiltering.ps");
			Shader lutShader("shader/pbr/lut.vs", "shader/pbr/lut.ps");

			initialize(texture_hdr, toCubemapShader, irradianceShader, prefilterShader, lutShader);
		}

		void initialize(std::string texture_hdr, Shader& toCubemap, Shader& irradianceShader, Shader& prefilterShader, 
			Shader& lutShader)
		{
			glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

			SHOWYELLOW("Initializing PBR");

			unsigned int hdrMap = pixeldancer::loadHDR("data/textures/hdr/newport_loft.hdr");
			ShapeFactory factory;

			glGenFramebuffers(1, &framebuffer_capture);
			glGenRenderbuffers(1, &renderbuffer_capture);

			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_capture);
			glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_capture);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer_capture);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				SHOWRED("PBR Framebuffer Cannot Initialized");
				return;
			}

			SHOWGREEN("PBR Framebuffer Initialized");

			// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// CUBE MAP
			// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			glGenTextures(1, &envCubemap);
			glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
			for (unsigned int i = 0; i < 6; ++i)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
			glm::mat4 captureViews[] =
			{
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
			};

			toCubemap.load();
			toCubemap.setMat4("proj_matrix", captureProjection);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, hdrMap);

			glViewport(0, 0, 512, 512);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_capture);
			for (unsigned int i = 0; i < 6; ++i)
			{
				toCubemap.setMat4("view_matrix", captureViews[i]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				
				factory.renderCube();
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			SHOWGREEN("HDR Cubemap Generated");

			// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// IRRADIANCE MAP
			// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			glGenTextures(1, &irradianceMap);
			glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
			for (unsigned int i = 0; i < 6; ++i)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_capture);
			glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_capture);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

			irradianceShader.load();
			irradianceShader.setMat4("proj_matrix", captureProjection);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

			glViewport(0, 0, 32, 32);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_capture);
			for (unsigned int i = 0; i < 6; ++i)
			{
				irradianceShader.setMat4("view_matrix", captureViews[i]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				factory.renderCube();
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			SHOWGREEN("HDR Irradiance Map Generated");

			// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// PREFILTER MAP
			// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			glGenTextures(1, &prefilterMap);
			glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
			for (unsigned int i = 0; i < 6; ++i)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

			prefilterShader.load();
			prefilterShader.setMat4("proj_matrix", captureProjection);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_capture);
			unsigned int maxMipLevels = 5;
			for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
			{
				unsigned int mipWidth = 128 * std::pow(0.5, mip);
				unsigned int mipHeight = 128 * std::pow(0.5, mip);
				glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_capture);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);

				glViewport(0, 0, mipWidth, mipHeight);

				float roughness = (float)mip / (float)(maxMipLevels - 1);
				prefilterShader.setFloat("roughness", roughness);

				for (unsigned int i = 0; i < 6; ++i)
				{
					prefilterShader.setMat4("view_matrix", captureViews[i]);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

					factory.renderCube();
				}
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			
			SHOWGREEN("HDR Prefilter Map Generated");
				
			glGenTextures(1, &texture_lut);

			glBindTexture(GL_TEXTURE_2D, texture_lut);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_capture);
			glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_capture);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_lut, 0);

			glViewport(0, 0, 512, 512);
			lutShader.load();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			factory.renderQuad();

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			SHOWGREEN("LUT Texture Generated");
		}

		~PBR()
		{
			if (framebuffer_capture != 0)
				glDeleteFramebuffers(1, &framebuffer_capture);

			if (renderbuffer_capture != 0)
				glDeleteRenderbuffers(1, &renderbuffer_capture);

			if (irradianceMap != 0)
				glDeleteTextures(1, &irradianceMap);

			if (envCubemap != 0)
				glDeleteTextures(1, &envCubemap);

			if (prefilterMap != 0)
				glDeleteTextures(1, &prefilterMap);

			if (texture_lut != 0)
				glDeleteTextures(1, &texture_lut);
		}

		public:
			unsigned int framebuffer_capture, renderbuffer_capture;
			unsigned int irradianceMap;
			unsigned int envCubemap;
			unsigned int prefilterMap;
			unsigned int texture_lut;
	};
}