#pragma once

class VulkanRenderer {
public:
    VulkanRenderer();
    ~VulkanRenderer();

    void init();
    void beginFrame();
    void endFrame();
    void draw();
    void cleanup();

private:
    // Vulkan-specific members
};