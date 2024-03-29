#pragma once

#include "Event.hpp"
#include "Camera.hpp"
#include <memory>

namespace SimpleEngine {

    struct Vertex;
    using VertexArray = std::vector<Vertex>;
    using IndexArray  = std::vector<uint16_t>;
    class VulkanRenderer;

    class Application
    {
    public:
        Application();
        virtual ~Application();

        Application(const Application&) = delete;
        Application(Application&&) = delete;
        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&) = delete;

        virtual int start(unsigned int window_width, unsigned int window_height, const char* title);
        void close();

        virtual void on_update() {}

        virtual void on_ui_draw() {}

        virtual void on_mouse_button_event(const MouseButton button_code,
                                           const double x_pos,
                                           const double y_pos,
                                           const bool pressed) {}

        glm::vec2 get_current_cursor_position() const;

        Camera camera{glm::vec3(-5.f, 0.f, 0.f)};

    private:
        void draw();

        std::unique_ptr<class Window> m_pWindow;
        std::unique_ptr<VulkanRenderer>  m_pRenderer;
        
        EventDispatcher m_event_dispatcher;
        bool m_bCloseWindow = false;

    protected:
        void setVertexArray(VertexArray& array);
        void setIndexArray(IndexArray& array);  
    };

}