#pragma once

#include "Event.hpp"

#include <string>
#include <functional>
#include <glm/ext/vector_float2.hpp>

struct GLFWwindow;

namespace SimpleEngine {

    class Window
    {
    public:
        using EventCallbackFn = std::function<void(BaseEvent&)>;

        Window(std::string title, unsigned int width, unsigned int height);
        ~Window();

        Window(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;

        void on_update();

        [[nodiscard]] unsigned int get_width() const { return m_data.width; }
        [[nodiscard]] unsigned int get_height() const { return m_data.height; }
        [[nodiscard]] glm::vec2 get_current_cursor_position() const;

        void set_event_callback(const EventCallbackFn& callback)
        {
            m_data.eventCallbackFn = callback;
        }

    private:
        struct WindowData
        {
            std::string title;
            unsigned int width;
            unsigned int height;
            EventCallbackFn eventCallbackFn;
        };

        int init();
        void shutdown();
        WindowData m_data;

    protected:
        GLFWwindow* m_pWindow = nullptr;
    };

}