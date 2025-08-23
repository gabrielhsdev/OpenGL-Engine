#pragma once
#include <iostream>
#include <string>
#include <engine/stb_image.h>
#include <glad/glad.h>

class Texture {
  public:
    explicit Texture(const std::string& path) {
        id = createTextureFromFile(path.c_str());
    }
    ~Texture() {
        glDeleteTextures(1, &id);
    }
    unsigned int getId() const {
        return id;
    }

  private:
    unsigned int id;
    static unsigned int createTextureFromFile(const char* path) {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format = (nrChannels == 1) ? GL_RED : (nrChannels == 3) ? GL_RGB : GL_RGBA;
            glTexImage2D(
                GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture: " << path << "\n";
        }
        stbi_image_free(data);
        return textureID;
    }
};

// Loader ( Needed for all the resources )
static std::unique_ptr<Texture> textureLoader(const std::string& path) {
    return std::make_unique<Texture>(path);
}
