#include "application.hpp"

#include "math_3d/math.hpp"

#include "core/gl_api.hpp"
#include "graphics/mesh.hpp"

#include "graphics/shaders/simple_shader.hpp"
#include "graphics/shaders/gouraud_shader.hpp"

#include <iostream>
#include <memory>
#include <cassert>


namespace rasterization {
    static gl::gl_api& core = gl::gl_api::get();

    struct Vertex {
        math::vec3f position;
        math::color color;
    };

    Application::Application(const std::string &title, std::uint32_t width, std::uint32_t height, size_t fps_lock)
        : m_window(win_framewrk::Window::Get()), m_last_frame(std::chrono::steady_clock::now()), m_fps_lock(1.0f / (fps_lock > 0 ? fps_lock : 1))
    {
        using namespace gl;
        using namespace math;
    
        m_window->Init(title, width, height);

        core.bind_window(m_window);
        assert(core.is_window_binded());

        core.viewport(width, height);

        m_window->SetResizeCallback([](uint32_t width, uint32_t height){
            core.viewport(width, height);
        });


        const Vertex triangle[] = {
            { {-0.5f, -0.5f, 0.0f}, color::RED },
            { { 0.5f, -0.5f, 0.0f}, color::GREEN },
            { { 0.0f,  0.5f, 0.0f}, color::BLUE },
        };
        const size_t triangle_indexes[] = { 0, 1, 2, 0 };

        m_VBO_IBO["triangle"] = {
            core.create_vertex_buffer(triangle, sizeof(triangle)),
            core.create_index_buffer(triangle_indexes, sizeof(triangle_indexes) / sizeof(triangle_indexes[0]))
        };
        core.bind_buffer(buffer_type::VERTEX, m_VBO_IBO["triangle"].vbo);
        core.set_buffer_element_size(sizeof(triangle[0]));

        try {
            Mesh model("..\\..\\..\\rasterizer\\app\\assets\\suzanne.obj");
            const Mesh::Content* model_buffer = model.GetContent();
            
            m_VBO_IBO["model"] = {
                core.create_vertex_buffer(model_buffer->vertexes.data(), model_buffer->vertexes.size() * sizeof(model_buffer->vertexes[0])),
                core.create_index_buffer(model_buffer->indexes.data(), model_buffer->indexes.size())
            };
            core.bind_buffer(buffer_type::VERTEX, m_VBO_IBO["model"].vbo);
            core.set_buffer_element_size(sizeof(model_buffer->vertexes[0]));

            Mesh cube("..\\..\\..\\rasterizer\\app\\assets\\cube.obj");
            const Mesh::Content* cube_buffer = cube.GetContent();

            m_VBO_IBO["cube"] = {
                core.create_vertex_buffer(cube_buffer->vertexes.data(), cube_buffer->vertexes.size() * sizeof(cube_buffer->vertexes[0])),
                core.create_index_buffer(cube_buffer->indexes.data(), cube_buffer->indexes.size())
            };
            core.bind_buffer(buffer_type::VERTEX, m_VBO_IBO["cube"].vbo);
            core.set_buffer_element_size(sizeof(cube_buffer->vertexes[0]));
        }
        catch(const std::exception& e) {
            std::cerr << e.what() << '\n';
            abort();
        }
    }

    void Application::Run() noexcept {
        using namespace gl;
        using namespace math;
        using namespace win_framewrk;

        size_t simple_shader = core.create_shader(std::make_shared<SimpleShader>());
        core.bind_shader(simple_shader); 
        core.uniform("model", mat4f::IDENTITY);
        core.uniform("view", look_at_rh(vec3f::FORWARD() * 2.5f, vec3f::ZERO(), vec3f::UP()));
        
        size_t model_shader = core.create_shader(std::make_shared<GouraudShader>());
        core.bind_shader(model_shader); 
        core.uniform("light_position", 10.0f * (vec3f::BACKWARD() + vec3f::LEFT()));
        core.uniform("light_intensity", 1.1f);
        core.uniform("light_color", color::WHITE);
        core.uniform("polygon_color", color::GOLDEN);
        core.uniform("model", mat4f::IDENTITY);
        core.uniform("view", look_at_rh(vec3f::FORWARD() * 2.5f, vec3f::ZERO(), vec3f::UP()));

        mat4f rotation, translation;
        render_mode model_render_mode = render_mode::TRIANGLES;
        while (m_window->IsOpen()) {
            m_window->PollEvent();

            const float dt = _LockFPS();
            std::cout << "FPS: " << std::to_string(1.0f / dt) << "\ttime: " << dt << "\tms\n";

        #pragma region input
            const float angle = to_radians(dt) * 45.0f;
            const float distance = dt * 3.0f;
            
            if (m_window->IsKeyPressed(Key::NUMBER_1)) {
                model_render_mode = render_mode::POINTS;
            } else if (m_window->IsKeyPressed(Key::NUMBER_2)) {
                model_render_mode = render_mode::LINES;
            } else if (m_window->IsKeyPressed(Key::NUMBER_3)) {
                model_render_mode = render_mode::LINE_STRIP;
            } else if (m_window->IsKeyPressed(Key::NUMBER_4)) {
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
                translation = translate(translation, vec3f::RIGHT() * distance);
                core.uniform("model", rotation * translation);
            } else if (m_window->IsKeyPressed(Key::A)) {
                translation = translate(translation, vec3f::LEFT() * distance);
                core.uniform("model", rotation * translation);
            }

            if (m_window->IsKeyPressed(Key::W)) {
                translation = translate(translation, vec3f::UP() * distance);
                core.uniform("model", rotation * translation);
            } else if (m_window->IsKeyPressed(Key::S)) {
                translation = translate(translation, vec3f::DOWN() * distance);
                core.uniform("model", rotation * translation);
            }

            if (m_window->IsKeyPressed(Key::Z)) {
                translation = translate(translation, vec3f::BACKWARD() * distance);
                core.uniform("model", rotation * translation);
            } else if (m_window->IsKeyPressed(Key::X)) {
                translation = translate(translation, vec3f::FORWARD() * distance);
                core.uniform("model", rotation * translation);
            }
        #pragma endregion input

            // core.bind_shader(simple_shader);
            // core.uniform("projection", perspective(math::to_radians(90.0f), float(m_window->GetWidth()) / m_window->GetHeight(), 1.0f, 100.0f));
            // core.bind_buffer(buffer_type::VERTEX, m_VBO_IBO["triangle"].vbo);
            // core.bind_buffer(buffer_type::INDEX, m_VBO_IBO["triangle"].ibo);
            // core.render(model_render_mode);

            core.bind_shader(model_shader);
            core.uniform("projection", perspective(math::to_radians(90.0f), float(m_window->GetWidth()) / m_window->GetHeight(), 1.0f, 100.0f));
            core.bind_buffer(buffer_type::VERTEX, m_VBO_IBO["model"].vbo);
            core.bind_buffer(buffer_type::INDEX, m_VBO_IBO["model"].ibo);
            core.render(model_render_mode);

            // core.bind_shader(model_shader);
            // core.uniform("projection", perspective(math::to_radians(90.0f), float(m_window->GetWidth()) / m_window->GetHeight(), 1.0f, 100.0f));
            // core.bind_buffer(buffer_type::VERTEX, m_VBO_IBO["cube"].vbo);
            // core.bind_buffer(buffer_type::INDEX, m_VBO_IBO["cube"].ibo);
            // core.render(model_render_mode);

            core.swap_buffers(); 
            core.clear_depth_buffer();
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