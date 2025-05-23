#pragma once
#include <string>
#include <unordered_map>
#include <GL/glew.h>

class ResourceManager {
public:
    static GLuint loadTexture(const std::string& filename);
    static void clear();
private:
    static std::unordered_map<std::string, GLuint> textures;
};