#pragma once 

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stb_image.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace proc_gen {
    class Texture
    {
    public:
        unsigned int ID;

        Texture(std::string path, GLenum type)
        {
            glGenTextures(1, &ID);
            loadTexture(ID, path, type);
        }

    private:
        GLenum getFormat(const unsigned& nrChannels)
        {
            if (nrChannels == 1)
                return GL_RED;
            else if (nrChannels == 3)
                return GL_RGB;
            else if (nrChannels == 4)
                return GL_RGBA;
            else throw std::runtime_error("Can't detected the format of the image from the number of channels");
        }

        void loadTexture(unsigned& textureId, const std::string& fileName, GLenum type)
        {
            int width, height, nrChannels;
            unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
            if (!data)
            {
                stbi_image_free(data);
                throw std::runtime_error("Failed to load texture from file: " + fileName + ".");
            }

            GLenum format = getFormat(nrChannels);

            glBindTexture(type, textureId);
            glTexImage2D(type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);

            glGenerateMipmap(type);
            glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(type, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8);
        }
    };
}