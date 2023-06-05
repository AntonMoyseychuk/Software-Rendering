#include "application.hpp"

#include "math_3d/math.hpp"

#include "core/gl_api.hpp"
#include "graphics/model.hpp"

#include <iostream>
#include <memory>
#include <cassert>


namespace rasterization {
    static rasterization::gfx::gl_api& core = gfx::gl_api::get();

    Application::Application(const std::string &title, std::uint32_t width, std::uint32_t height, size_t fps_lock)
        : m_window(win_framewrk::Window::Get()), m_last_frame(std::chrono::steady_clock::now()), m_fps_lock(1.0f / (fps_lock > 0 ? fps_lock : 1))
    {
        using namespace gfx;
        using namespace math;
    
        m_window->Init(title, width, height);

        core.bind_window(m_window);
        assert(core.is_window_binded() != nullptr);

        core.viewport(width, height);

        m_window->SetResizeCallback([](uint32_t width, uint32_t height){
            gfx::gl_api::get().viewport(width, height);
        });

        Model model("..\\..\\..\\rasterizer\\assets\\human.obj");
        
        m_VBO_IBO["model"] = std::make_pair(
            core.create_vertex_buffer(model.vertexes.data(), model.vertexes.size() * sizeof(model.vertexes[0])),
            core.create_index_buffer(model.vert_indexes.data(), model.vert_indexes.size())
        );
        core.vertex_attrib_pointer(m_VBO_IBO["model"].first, sizeof(vec3f), attrib_data_type::FLOAT, sizeof(vec3f), (void*)0);

        const vec3f cube[] = {
            { -1.0f, -1.0f, 1.0f },
            { -1.0f,  1.0f, 1.0f },
            {  1.0f,  1.0f, 1.0f },
            {  1.0f, -1.0f, 1.0f },

            { -1.0f, -1.0f, -1.0f },
            { -1.0f,  1.0f, -1.0f },
            {  1.0f,  1.0f, -1.0f },
            {  1.0f, -1.0f, -1.0f }
        };
        const size_t indexes[] = {
            0, 1,
            1, 2,
            2, 3,
            3, 0,

            4, 5,
            5, 6,
            6, 7,
            7, 4,

            0, 4,
            4, 5,
            5, 1,
            1, 0,

            2, 6,
            6, 7,
            7, 3,
            3, 2,
        };

        m_VBO_IBO["cube"] = std::make_pair(
            core.create_vertex_buffer(cube, sizeof(cube)),
            core.create_index_buffer(indexes, sizeof(indexes) / sizeof(size_t))
        );
        core.vertex_attrib_pointer(m_VBO_IBO["cube"].first, sizeof(vec3f), attrib_data_type::FLOAT, sizeof(vec3f), (void*)0);
    }

    void Application::Run() noexcept {
        using namespace gfx;
        using namespace math;
        using namespace win_framewrk;

        core.uniform("light_dir", normalize(vec3f::BACKWARD + vec3f::LEFT));
        
        core.uniform("polygon_color", color::GOLDEN);
        core.uniform("line_color", color::LIME);
        core.uniform("point_color", color::SKY_BLUE);

        core.uniform("model", mat4f::IDENTITY);
        core.uniform("view", look_at_rh(vec3f::FORWARD * 3.0f, vec3f::ZERO, vec3f::UP));

        mat4f rotation, translation;
        render_mode model_render_mode = render_mode::TRIANGLES;
        while (m_window->IsOpen()) {
            m_window->PollEvent();

            const auto dt = _LockFPS();
            std::cout << "FPS: " << std::to_string(1.0f / dt) << std::endl;

            const float angle = to_radians(dt) * 25.0f;

        #pragma region input
            if (m_window->IsKeyPressed(Key::NUMBER_1)) {
                model_render_mode = render_mode::POINTS;
            } else if (m_window->IsKeyPressed(Key::NUMBER_2)) {
                model_render_mode = render_mode::LINES;
            } else if (m_window->IsKeyPressed(Key::NUMBER_3)) {
                model_render_mode = render_mode::TRIANGLES;
            }

            if (m_window->IsKeyPressed(Key::RIGHT_ARROW)) {
                rotation = rotate_y(rotation, -angle);
                core.uniform("model", rotation * translation);
            } else if (m_window->IsKeyPressed(Key::LEFT_ARROW)) {
                rotation = rotate_y(rotation, angle);
                core.uniform("model", rotation * translation);
            }

            if (m_window->IsKeyPressed(Key::UP_ARROW)) {
                rotation = rotate_x(rotation, -angle);
                core.uniform("model", rotation * translation);
            } else if (m_window->IsKeyPressed(Key::DOWN_ARROW)) {
                rotation = rotate_x(rotation, angle);
                core.uniform("model", rotation * translation);
            }

            if (m_window->IsKeyPressed(Key::D)) {
                translation = translate(translation, vec3f::RIGHT * dt);
                core.uniform("model", rotation * translation);
            } else if (m_window->IsKeyPressed(Key::A)) {
                translation = translate(translation, vec3f::LEFT * dt);
                core.uniform("model", rotation * translation);
            }

            if (m_window->IsKeyPressed(Key::W)) {
                translation = translate(translation, vec3f::UP * dt);
                core.uniform("model", rotation * translation);
            } else if (m_window->IsKeyPressed(Key::S)) {
                translation = translate(translation, vec3f::DOWN * dt);
                core.uniform("model", rotation * translation);
            }

            if (m_window->IsKeyPressed(Key::Z)) {
                translation = translate(translation, vec3f::BACKWARD * dt);
                core.uniform("model", rotation * translation);
            } else if (m_window->IsKeyPressed(Key::X)) {
                translation = translate(translation, vec3f::FORWARD * dt);
                core.uniform("model", rotation * translation);
            }
        #pragma endregion input

            core.uniform("projection", perspective(math::to_radians(90.0f), float(m_window->GetWidth()) / m_window->GetHeight(), 1.0f, 100.0f));
            
            core.bind(buffer_type::VERTEX, m_VBO_IBO["model"].first);
            core.bind(buffer_type::INDEX, m_VBO_IBO["model"].second);
            core.render(model_render_mode);

            core.bind(buffer_type::VERTEX, m_VBO_IBO["cube"].first);
            core.bind(buffer_type::INDEX, m_VBO_IBO["cube"].second);
            core.render(render_mode::LINES);

            core.swap_buffers(); 
            core.clear_backbuffer();
        }
    }
    
    void Application::SetFPSLock(size_t fps) const noexcept {
        m_fps_lock = 1.0f / fps;
    }
    
    float Application::_LockFPS() const noexcept {
        using namespace std::chrono;

        float dt = 0.0f;
        steady_clock::time_point curr_frame;
        
        do {
            curr_frame = steady_clock::now();
            dt = duration<float>(curr_frame - m_last_frame).count();
        } while(dt < m_fps_lock);
        m_last_frame = curr_frame;

        return dt;
    }
}