#include <iostream>
#include <memory>
#include <imgui/imgui.h>

#include <Input.hpp>
#include <Application.hpp>
#include <imgui/imgui_internal.h>
#include "intersect.hpp"
#include "VertexArray.hpp"

class SimpleEngineEditor : public SimpleEngine::Application
{
    double m_initial_mouse_pos_x = 0.0;
    double m_initial_mouse_pos_y = 0.0;
    float delta = 0.05f;    

    virtual void on_update() override
    {
        glm::vec3 movement_delta{ 0, 0, 0 };
        glm::vec3 rotation_delta{ 0, 0, 0 };

        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_W))
        {
            movement_delta.x += delta;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_S))
        {
            movement_delta.x -= delta;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_A))
        {
            movement_delta.y -= delta;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_D))
        {
            movement_delta.y += delta;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_E))
        {
            movement_delta.z += delta;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_Q))
        {
            movement_delta.z -= delta;
        }

        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_UP))
        {
            rotation_delta.y -= 0.5f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_DOWN))
        {
            rotation_delta.y += 0.5f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_RIGHT))
        {
            rotation_delta.z -= 0.5f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_LEFT))
        {
            rotation_delta.z += 0.5f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_P))
        {
            rotation_delta.x += 0.5f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_O))
        {
            rotation_delta.x -= 0.5f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_LEFT_SHIFT))
        {
            delta = 0.1f;
        } else {
            delta = 0.05f;
        }

        if (SimpleEngine::Input::IsMouseButtonPressed(SimpleEngine::MouseButton::MOUSE_BUTTON_LEFT))
        {
            glm::vec2 current_cursor_position = get_current_cursor_position();
            if (SimpleEngine::Input::IsMouseButtonPressed(SimpleEngine::MouseButton::MOUSE_BUTTON_RIGHT))
            {
                camera.move_right(static_cast<float>(current_cursor_position.x - m_initial_mouse_pos_x) / 100.f);
                camera.move_up(static_cast<float>(m_initial_mouse_pos_y - current_cursor_position.y) / 100.f);
            }
            else
            {
                rotation_delta.z += static_cast<float>(m_initial_mouse_pos_x - current_cursor_position.x) / 5.f;
                rotation_delta.y -= static_cast<float>(m_initial_mouse_pos_y - current_cursor_position.y) / 5.f;
            }
            m_initial_mouse_pos_x = current_cursor_position.x;
            m_initial_mouse_pos_y = current_cursor_position.y;
        }

        camera.add_movement_and_rotation(movement_delta, rotation_delta);
    }


    virtual void on_mouse_button_event(const SimpleEngine::MouseButton button_code,
                                       const double x_pos,
                                       const double y_pos,
                                       const bool pressed) override
    {
        m_initial_mouse_pos_x = x_pos;
        m_initial_mouse_pos_y = y_pos;
    }


    SimpleEngine::Vertex makeVertex(GeomObj::Triangle& tr, int ver, bool flag, GeomObj::Vector& normal) {
        auto vert = tr[ver];

        auto blue = glm::vec3(0.0,0.0,1.0);
        auto red  = glm::vec3(1.0,0.0,0.0);

        SimpleEngine::Vertex ret = {
            glm::vec3(vert.x, vert.y, vert.z),
        };

        flag ? ret.color = red : ret.color = blue;
        ret.normal = glm::vec3(normal.x, normal.y, normal.z);

        return ret;
    }


    public:

        SimpleEngineEditor()
        {
            auto [tr, flag] = GeomObj::GetTriangles();

            auto size = tr.size();
            
            SimpleEngine::VertexArray vertices;
            SimpleEngine::IndexArray indices;
            
            indices.reserve(size * 3 * 2);
            vertices.reserve(size * 3 * 2);

            for (auto i = 0UL; i < size * 3; ++i) {
                indices.push_back(i);
            }

            for (auto i = 0UL; i < size; ++i) {
                auto normal = cross(tr[i][2] - tr[i][0], tr[i][1] - tr[i][0]); 

                SimpleEngine::Vertex firstVertex  = makeVertex(tr[i], 0, flag[i], normal);
                SimpleEngine::Vertex secondVertex = makeVertex(tr[i], 1, flag[i], normal);
                SimpleEngine::Vertex thirdVertex  = makeVertex(tr[i], 2, flag[i], normal);
                vertices.push_back(firstVertex);
                vertices.push_back(secondVertex);
                vertices.push_back(thirdVertex);
            }

            for (auto i = size * 3; i < size * 3 * 2; ++i) {
                indices.push_back(i);
            }

            for (auto i = 0UL; i < size; ++i) {
                auto normal = cross(tr[i][1] - tr[i][0], tr[i][2] - tr[i][0]); 

                SimpleEngine::Vertex firstVertex  = makeVertex(tr[i], 2, flag[i], normal);
                SimpleEngine::Vertex secondVertex = makeVertex(tr[i], 1, flag[i], normal);
                SimpleEngine::Vertex thirdVertex  = makeVertex(tr[i], 0, flag[i], normal);
                vertices.push_back(firstVertex);
                vertices.push_back(secondVertex);
                vertices.push_back(thirdVertex);
            }

            setVertexArray(vertices);
            setIndexArray(indices);
        }
};

int main()
{
    auto pSimpleEngineEditor = std::make_unique<SimpleEngineEditor>();

    int returnCode = pSimpleEngineEditor->start(1024, 1024, "SimpleEngine Editor");

    return returnCode;
}