#include "application.hpp"
#include "window.hpp"

namespace VkEngine 
{
    Application::Application() 
    {

    }
    
    Application::~Application() 
    {

    }

    int Application::start(unsigned window_width, unsigned window_height, std::string title)
    {
        m_window = std::make_unique<Window>(title, window_width, window_height);
        
        while (true)
        {
            m_window->on_update();
            on_update();
        }

        return 0;
    }

}