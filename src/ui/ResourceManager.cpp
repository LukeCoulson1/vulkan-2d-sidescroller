#include "ResourceManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glew.h>   // GLEW must be included before gl.h
// Do NOT include <GL/gl.h> after GLEW! GLEW includes it for you.

std::unordered_map<std::string, GLuint> ResourceManager::textures;

GLuint ResourceManager::loadTexture(const std::string& filename) {
    if (textures.count(filename)) return textures[filename];
    int width, height, channels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 4);
    if (!data) return 0;
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);
    textures[filename] = tex;
    return tex;
}
void ResourceManager::clear() {
    for (auto& pair : textures) glDeleteTextures(1, &pair.second);
    textures.clear();
}