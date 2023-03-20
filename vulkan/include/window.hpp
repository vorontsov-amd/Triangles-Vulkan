#pragma once
#include <string>
#include <GLFW/glfw3.h>
#include <functional>
#include "Event.hpp"

namespace VkEngine
{
    class Window 
    {
    public:
        using EventCallbackFn = std::function<void(BaseEvent&)>;

        Window(std::string title, unsigned width, unsigned height);
        virtual ~Window();

        Window(const Window&)            = delete;
        Window(Window&&)                 = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&)      = delete;

        void on_update();
        unsigned getWidth()  const { return m_width; }
        unsigned getHeight() const { return m_height; }
    private:
        int init();
        void shutdown();

        GLFWwindow* m_window;
        std::string m_title;
        unsigned    m_width;
        unsigned    m_height;

    };
}