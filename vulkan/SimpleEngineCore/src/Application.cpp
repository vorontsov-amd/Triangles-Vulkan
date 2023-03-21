#include "Application.hpp"
#include "Log.hpp"
#include "Window.hpp"
#include "Event.hpp"
#include "Input.hpp"

// #include "SimpleEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
// #include "SimpleEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
// #include "SimpleEngineCore/Rendering/OpenGL/VertexArray.hpp"
// #include "SimpleEngineCore/Rendering/OpenGL/IndexBuffer.hpp"
// #include "SimpleEngineCore/Rendering/OpenGL/Texture2D.hpp"
#include "Camera.hpp"
// #include "SimpleEngineCore/Rendering/OpenGL/Renderer_OpenGL.hpp"
#include "Rendering/Vulkan/VulkanRenderer.hpp"

#include <imgui/imgui.h>
#include <glm/mat3x3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

#define GLWF_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>


namespace SimpleEngine {
    void Application::setVerties(VertexArray& array) { VulkanRenderer::setVertexArray(array); }

    void Application::setIndies(IndexArray& array) { VulkanRenderer::setIndexArray(array); }

    Application::Application()
    {
        LOG_INFO("Starting Application");
    }


    Application::~Application()
    {
        LOG_INFO("Closing Application");
    }

    void Application::draw()
    {
        m_pWindow->on_update();
        m_pRenderer->drawFrame(camera.get_view_matrix(), camera.get_projection_matrix(), camera.get_position());
        on_update();
    }

    int Application::start(unsigned int window_width, unsigned int window_height, const char* title)
    {
        m_pWindow   = std::make_unique<Window>(title, window_width, window_height);
        m_pRenderer = std::make_unique<VulkanRenderer>(m_pWindow);

        camera.set_viewport_size(static_cast<float>(window_width), static_cast<float>(window_height));

        m_event_dispatcher.add_event_listener<EventMouseMoved>(
            [](EventMouseMoved& event)
            {
                //LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.x, event.y);
            });

        m_event_dispatcher.add_event_listener<EventWindowResize>(
            [&](EventWindowResize& event)
            {
                LOG_INFO("[Resized] Changed size to {0}x{1}", event.width, event.height);
                camera.set_viewport_size(static_cast<float>(event.width), static_cast<float>(event.height));
                draw();
            });

        m_event_dispatcher.add_event_listener<EventWindowClose>(
            [&](EventWindowClose& event)
            {
                LOG_INFO("[WindowClose]");
                close();
            });

        m_event_dispatcher.add_event_listener<EventMouseButtonPressed>(
            [&](EventMouseButtonPressed& event)
            {
                LOG_INFO("[Mouse button pressed: {0}, at ({1}, {2})", static_cast<int>(event.mouse_button), event.x_pos, event.y_pos);
                Input::PressMouseButton(event.mouse_button);
                on_mouse_button_event(event.mouse_button, event.x_pos, event.y_pos, true);
            });

        m_event_dispatcher.add_event_listener<EventMouseButtonReleased>(
            [&](EventMouseButtonReleased& event)
            {
                LOG_INFO("[Mouse button released: {0}, at ({1}, {2})", static_cast<int>(event.mouse_button), event.x_pos, event.y_pos);
                Input::ReleaseMouseButton(event.mouse_button);
                on_mouse_button_event(event.mouse_button, event.x_pos, event.y_pos, false);
            });

        m_event_dispatcher.add_event_listener<EventKeyPressed>(
            [&](EventKeyPressed& event)
            {
                if (event.key_code <= KeyCode::KEY_Z)
                {
                    if (event.repeated)
                    {
                        LOG_INFO("[Key pressed: {0}, repeated", static_cast<char>(event.key_code));
                    }
                    else
                    {
                        LOG_INFO("[Key pressed: {0}", static_cast<char>(event.key_code));
                    }
                }
                Input::PressKey(event.key_code);
            });

        m_event_dispatcher.add_event_listener<EventKeyReleased>(
            [&](EventKeyReleased& event)
            {
                if (event.key_code <= KeyCode::KEY_Z)
                {
                    LOG_INFO("[Key released: {0}", static_cast<char>(event.key_code));
                }
                Input::ReleaseKey(event.key_code);
            });

        m_pWindow->set_event_callback(
            [&](BaseEvent& event)
            {
                m_event_dispatcher.dispatch(event);
            }
        );
        
        while (!m_bCloseWindow)
        {
            draw();
        }

        m_pWindow = nullptr;

        return 0;
    }

    glm::vec2 Application::get_current_cursor_position() const
    {
        return m_pWindow->get_current_cursor_position();
    }

    void Application::close()
    {
        m_bCloseWindow = true;
    }
}