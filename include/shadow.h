#pragma once

#include "shader.h"

namespace pixeldancer
{
	class ShadowMapping
	{
	public:
		ShadowMapping()
		{
			framebuffer_shadowMap = 0;
			texture_shadowMap = 0;
		}

		void initialize(int width = 1024, int height = 1024, glm::vec4 color = glm::vec4(1.0f))
		{
			glGenFramebuffers(1, &framebuffer_shadowMap);

			glGenTextures(1, &texture_shadowMap);
			glBindTexture(GL_TEXTURE_2D, texture_shadowMap);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

			float borderColor[] = { color.r, color.g, color.b, color.a };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_shadowMap);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_shadowMap, 0);

			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		~ShadowMapping()
		{
			if (framebuffer_shadowMap != 0)
				glDeleteFramebuffers(1, &framebuffer_shadowMap);
		}

	public:
		unsigned int framebuffer_shadowMap;
		unsigned int texture_shadowMap;
	};
}