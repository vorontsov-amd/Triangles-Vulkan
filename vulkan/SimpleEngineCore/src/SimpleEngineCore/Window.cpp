#include "SimpleEngineCore/Window.hpp"
#include "SimpleEngineCore/Log.hpp"
#include "SimpleEngineCore/Modules/UIModule.hpp"
#include "SimpleEngineCore/Rendering/Vulkan/VulkanRenderer.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>


namespace SimpleEngine {

    Window::Window(std::string title, const unsigned int width, const unsigned int height)
        : m_data({ std::move(title), width, height })
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

        m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
        if (!m_pWindow)
        {
            LOG_CRITICAL("Can't create window {0} with size {1}x{2}", m_data.title, m_data.width, m_data.height);
            return -2;
        }

        glfwSetWindowUserPointer(m_pWindow, &m_data);

        glfwSetKeyCallback(m_pWindow,
            [](GLFWwindow* pWindow, int key, int scancode, int action, int mods)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                switch (action)
                {
                    case GLFW_PRESS:
                    {
                        EventKeyPressed event(static_cast<KeyCode>(key), false);
                        data.eventCallbackFn(event);
                        break;
                    }
                    case GLFW_RELEASE:
                    {
                        EventKeyReleased event(static_cast<KeyCode>(key));
                        data.eventCallbackFn(event);
                        break;
                    }
                    case GLFW_REPEAT:
                    {
                        EventKeyPressed event(static_cast<KeyCode>(key), true);
                        data.eventCallbackFn(event);
                        break;
                    }
                }
            }
        );

        glfwSetMouseButtonCallback(m_pWindow,
            [](GLFWwindow* pWindow, int button, int action, int mods)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                double x_pos;
                double y_pos;
                glfwGetCursorPos(pWindow, &x_pos, &y_pos);
                switch (action)
                {
                    case GLFW_PRESS:
                    {
                        EventMouseButtonPressed event(static_cast<MouseButton>(button), x_pos, y_pos);
                        data.eventCallbackFn(event);
                        break;
                    }
                    case GLFW_RELEASE:
                    {
                        EventMouseButtonReleased event(static_cast<MouseButton>(button), x_pos, y_pos);
                        data.eventCallbackFn(event);
                        break;
                    }
                }
            }
        );

        glfwSetWindowSizeCallback(m_pWindow, 
            [](GLFWwindow* pWindow, int width, int height)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                data.width = width;
                data.height = height;
                EventWindowResize event(width, height);
                data.eventCallbackFn(event);
            }
        );

        glfwSetCursorPosCallback(m_pWindow,
            [](GLFWwindow* pWindow, double x, double y)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                EventMouseMoved event(x, y);
                data.eventCallbackFn(event);
            }
        );

        glfwSetWindowCloseCallback(m_pWindow,
            [](GLFWwindow* pWindow)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                EventWindowClose event;
                data.eventCallbackFn(event);
            }
        );

        glfwSetFramebufferSizeCallback(m_pWindow,
            [](GLFWwindow* pWindow, int width, int height)
            {
                // int width = 0, height = 0;
                // glfwGetFramebufferSize(pWindow, &width, &height);
                // while (width == 0 || height == 0) {
                //     glfwGetFramebufferSize(pWindow, &width, &height);
                //     glfwWaitEvents();
                // }
                
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                EventFramebufferResize event(width, height);
                data.eventCallbackFn(event);
            }
        );

        return 0;
    }

    void Window::shutdown()
    {
        UIModule::on_window_close();
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
    }

    void Window::on_update()
    {
        glfwPollEvents();
    }

    glm::vec2 Window::get_current_cursor_position() const
    {
        double x_pos;
        double y_pos;
        glfwGetCursorPos(m_pWindow, &x_pos, &y_pos);
        return {x_pos, y_pos};
    }
}