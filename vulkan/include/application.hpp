#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace VkEngine
{
    class Application 
    {
        Application();
        virtual ~Application();

        Application(const Application&)            = delete;
        Application(Application&&)                 = delete;
        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&)      = delete;

        virtual int start(unsigned window_width, unsigned window_height, const char* title);
        virtual void on_update() {}
    };
}