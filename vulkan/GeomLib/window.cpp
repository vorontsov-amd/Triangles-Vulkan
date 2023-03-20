#include "window.hpp"
#include "log.hpp"

namespace VkEngine
{
    Window::Window(std::string title, unsigned width, unsigned height) :
        m_title{std::move(title)},
        m_width{width},
        m_height{height} 
        {
            int resultCode = init();
        }

    Window::~Window()
    {
        shutdown();
    }

    int Window::init() 
    {
        LOG_INFO("Creating window '{0}' with size {1}x{2}", m_data.title, m_data.width, m_data.height);

        glfwSetErrorCallback([](int error_code, const char* description)
            {
                LOG_CRITICAL("GLFW error: {0}", description);
            });

        if (!glfwInit())
        {
            LOG_CRITICAL("Can't initialize GLFW!");
            return -1;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
        if (!m_window)
        {
            LOG_CRITICAL("Can't create window {0} with size {1}x{2}", m_data.title, m_data.width, m_data.height);
            return -2;
        }    

        glfwMakeContextCurrent(m_window);
        return 0;
    }

    void Window::shutdown()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void Window::on_update()
    {
        glfwPollEvents();
    }
} 