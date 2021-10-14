#pragma once

#include "helper.h"

namespace pixeldancer
{
    unsigned int loadTex2D(std::string str_tex, int* _width = nullptr, int* _height = nullptr, bool AF = false, int number_AF = 2)
    {
        unsigned int texId;
        glGenTextures(1, &texId);

        stbi_set_flip_vertically_on_load(true);

        int width, height, channel;
        unsigned char* tex = stbi_load(str_tex.c_str(), &width, &height, &channel, 0);

        if (tex)
        {
            GLenum tex_format;
            if (channel == 1)
                tex_format = GL_RED;
            else if (channel == 3)
                tex_format = GL_RGB;
            else if (channel == 4)
                tex_format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, texId);
            glTexImage2D(GL_TEXTURE_2D, 0, tex_format, width, height, 0, tex_format, GL_UNSIGNED_BYTE, tex);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            *_width = width;
            *_height = height;

            if (_width == nullptr && _height == nullptr)
            {
                SHOWGREEN("Texture Loaded");
                std::cout << "Texture: " << str_tex << std::endl;
                std::cout << "Width: " << width << " Height: " << height << " Channel: " << channel << std::endl;
            }

            stbi_image_free(tex);
        }
        else
        {
            SHOWRED("Can't load "); std::cout << str_tex;
            stbi_image_free(tex);
        }

        return texId;
    }

    // order:
    // +X (right)
    // -X (left)
    // +Y (top)
    // -Y (bottom)
    // +Z (front) 
    // -Z (back)
    unsigned int loadCubemap(std::vector<std::string> faces)
    {
        unsigned int textureID;

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                SHOWRED("Can't load "); std::cout << faces[i];
                stbi_image_free(data);
            }
        }

        SHOWGREEN("Cubemap Loaded");

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }

    unsigned int loadHDR(const std::string hdrTexture)
    {
        unsigned int hdr;
        int width, height, nrComponents;
        float* data = stbi_loadf(hdrTexture.c_str(), &width, &height, &nrComponents, 0);

        if (data)
        {
            glGenTextures(1, &hdr);
            glBindTexture(GL_TEXTURE_2D, hdr);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            SHOWGREEN("HDR Texture Loaded");

            stbi_image_free(data);
        }
        else
            SHOWRED("Failed to load HDR image");

        return hdr;
    }
}