#ifndef UTILS_H
#define UTILS_H

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <string>
#include <vector> // Added to fix std::vector errors

// Utility functions
namespace Utils {
    VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);
    glm::mat4 createTransformationMatrix(float x, float y, float width, float height);
    std::vector<char> readFile(const std::string& filename);
}

#endif // UTILS_H