#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <window.hpp>
#include <memory>

namespace VkEngine
{
    class Application 
    {
    public:
        Application();
        virtual ~Application();

        Application(const Application&)            = delete;
        Application(Application&&)                 = delete;
        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&)      = delete;

        virtual int start(unsigned window_width, unsigned window_height, std::string title);
        virtual void on_update() {}
    private:
        std::unique_ptr<Window> m_window;
    };
}