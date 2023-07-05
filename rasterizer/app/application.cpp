#include "application.hpp"

#include "math_3d/math.hpp"

#include "core/gl_api.hpp"
#include "graphics/mesh.hpp"
#include "graphics/texture.hpp"

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

        m_window->SetResizeCallback([](uint32_t width, uint32_t height) {
            core.viewport(width, height);
        });

        const Vertex triangle[] = {
            { {-0.5f, -0.5f, 0.0f}, color::RED },
            { { 0.5f, -0.5f, 0.0f}, color::GREEN },
            { { 0.0f,  0.5f, 0.0f}, color::BLUE },
        };
        const size_t triangle_indexes[] = { 0, 1, 2, 0 };

        m_objects["triangle"] = {
            core.create_vertex_buffer(triangle, sizeof(triangle)),
            core.create_index_buffer(triangle_indexes, sizeof(triangle_indexes) / sizeof(triangle_indexes[0]))
        };
        core.bind_buffer(buffer_type::VERTEX, m_objects["triangle"].vbo);
        core.set_buffer_element_size(sizeof(triangle[0]));


        m_camera_position = 2.5f * vec3f::FORWARD();
        m_light_position = 10.0f * vec3f::FORWARD() + 7.0f * vec3f::RIGHT();

        m_view_matrix = look_at_rh(m_camera_position, vec3f::ZERO(), vec3f::UP());
        m_proj_matrix = perspective(math::to_radians(90.0f), float(width) / height, 1.0f, 100.0f);

        m_simple_shader = core.create_shader(std::make_shared<SimpleShader>());
        core.bind_shader(m_simple_shader);
        core.uniform(m_transform.scale * m_transform.rotation * m_transform.translation, "model");
        core.uniform(m_view_matrix, "view");
        core.uniform(m_proj_matrix, "projection");

        m_gouraud_shader = core.create_shader(std::make_shared<GouraudShader>());
        core.bind_shader(m_gouraud_shader); 
        core.uniform(m_light_position, "light_position");
        core.uniform(m_camera_position, "camera_position");
        core.uniform(1.0f, "light_intensity");
        core.uniform(color::WHITE, "light_color");
        core.uniform(color::TANGERINE, "polygon_color");
        core.uniform(m_transform.scale * m_transform.rotation * m_transform.translation, "model");
        core.uniform(m_view_matrix, "view");
        core.uniform(m_proj_matrix, "projection");


        try {
            Mesh head("..\\..\\..\\rasterizer\\app\\assets\\human.obj");
            const Mesh::Content* head_buffer = head.GetContent();
            
            m_objects["head"] = {
                core.create_vertex_buffer(head_buffer->vertexes.data(), head_buffer->vertexes.size() * sizeof(head_buffer->vertexes[0])),
                core.create_index_buffer(head_buffer->indexes.data(), head_buffer->indexes.size())
            };
            core.bind_buffer(buffer_type::VERTEX, m_objects["head"].vbo);
            core.set_buffer_element_size(sizeof(head_buffer->vertexes[0]));




            Mesh suzanne("..\\..\\..\\rasterizer\\app\\assets\\suzanne.obj");
            const Mesh::Content* suzanne_buffer = suzanne.GetContent();
            
            m_objects["suzanne"] = {
                core.create_vertex_buffer(suzanne_buffer->vertexes.data(), suzanne_buffer->vertexes.size() * sizeof(suzanne_buffer->vertexes[0])),
                core.create_index_buffer(suzanne_buffer->indexes.data(), suzanne_buffer->indexes.size())
            };
            core.bind_buffer(buffer_type::VERTEX, m_objects["suzanne"].vbo);
            core.set_buffer_element_size(sizeof(suzanne_buffer->vertexes[0]));


            

            Mesh cube("..\\..\\..\\rasterizer\\app\\assets\\cube.obj");
            const Mesh::Content* cube_buffer = cube.GetContent();

            m_objects["cube"] = {
                core.create_vertex_buffer(cube_buffer->vertexes.data(), cube_buffer->vertexes.size() * sizeof(cube_buffer->vertexes[0])),
                core.create_index_buffer(cube_buffer->indexes.data(), cube_buffer->indexes.size())
            };
            core.bind_buffer(buffer_type::VERTEX, m_objects["cube"].vbo);
            core.set_buffer_element_size(sizeof(cube_buffer->vertexes[0]));


            

            Mesh diablo("..\\..\\..\\rasterizer\\app\\assets\\diablo.obj");
            const Mesh::Content* diablo_buffer = diablo.GetContent();
            
            m_objects["diablo"] = {
                core.create_vertex_buffer(diablo_buffer->vertexes.data(), diablo_buffer->vertexes.size() * sizeof(diablo_buffer->vertexes[0])),
                core.create_index_buffer(diablo_buffer->indexes.data(), diablo_buffer->indexes.size())
            };
            core.bind_buffer(buffer_type::VERTEX, m_objects["diablo"].vbo);
            core.set_buffer_element_size(sizeof(diablo_buffer->vertexes[0]));


            

            Texture texture("..\\..\\..\\rasterizer\\app\\assets\\head.tga");
            const Texture::Content* texture_content = texture.GetContent();

            size_t colored_texture = core.create_texture(texture_content->width, texture_content->height, texture_content->channel_count, texture_content->data.data());
            core.bind_texture(colored_texture);
            core.activate_texture(0);

            texture.Load("..\\..\\..\\rasterizer\\app\\assets\\head_nm.tga");
            texture_content = texture.GetContent();

            size_t normal_texture = core.create_texture(texture_content->width, texture_content->height, texture_content->channel_count, texture_content->data.data());
            core.bind_texture(normal_texture);
            core.activate_texture(1);
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

        render_mode model_render_mode = render_mode::TRIANGLES;

        MouseState prev_mouse_state, curr_mouse_state;
        m_window->IsMousePressed(prev_mouse_state);
        while (m_window->IsOpen()) {
            m_window->PollEvent();

            const float dt = _LockFPS();
            std::cout << "FPS: " << std::to_string(1.0f / dt) << "\ttime: " << dt << "\tms\n";
  
        #pragma region input
            if (m_window->IsMousePressed(curr_mouse_state) && curr_mouse_state.pressed_button == MouseState::PressedButton::LEFT) {
                m_transform.rotation = rotate_y(m_transform.rotation, (prev_mouse_state.x - curr_mouse_state.x) * dt);
                m_transform.rotation = rotate_x(m_transform.rotation, -(prev_mouse_state.y - curr_mouse_state.y) * dt);
                core.uniform(m_transform.scale * m_transform.rotation * m_transform.translation, "model");
            }
            prev_mouse_state = curr_mouse_state;

        
            const float angle = to_radians(dt) * 60.0f;
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

            if (m_window->IsKeyPressed(Key::LALT)) {
                if (m_window->IsKeyPressed(Key::RIGHT_ARROW)) {
                    m_view_matrix = rotate_y(m_view_matrix, -angle);
                    core.uniform(m_view_matrix, "view");
                } else if (m_window->IsKeyPressed(Key::LEFT_ARROW)) {
                    m_view_matrix = rotate_y(m_view_matrix, angle);
                    core.uniform(m_view_matrix, "view");
                }

                if (m_window->IsKeyPressed(Key::UP_ARROW)) {
                    m_view_matrix = rotate_x(m_view_matrix, -angle);
                    core.uniform(m_view_matrix, "view");
                } else if (m_window->IsKeyPressed(Key::DOWN_ARROW)) {
                    m_view_matrix = rotate_x(m_view_matrix, angle);
                    core.uniform(m_view_matrix, "view");
                }

                if (m_window->IsKeyPressed(Key::D)) {
                    m_view_matrix = translate(m_view_matrix, vec3f::LEFT() * distance);
                    core.uniform(m_view_matrix, "view");
                } else if (m_window->IsKeyPressed(Key::A)) {
                    m_view_matrix = translate(m_view_matrix, vec3f::RIGHT() * distance);
                    core.uniform(m_view_matrix, "view");
                }

                if (m_window->IsKeyPressed(Key::W)) {
                    m_view_matrix = translate(m_view_matrix, vec3f::FORWARD() * distance);
                    core.uniform(m_view_matrix, "view");
                } else if (m_window->IsKeyPressed(Key::S)) {
                    m_view_matrix = translate(m_view_matrix, vec3f::BACKWARD() * distance);
                    core.uniform(m_view_matrix, "view");
                }

                if (m_window->IsKeyPressed(Key::SPASE)) {
                    m_view_matrix = translate(m_view_matrix, vec3f::DOWN() * distance);
                    core.uniform(m_view_matrix, "view");
                } else if (m_window->IsKeyPressed(Key::LCTRL)) {
                    m_view_matrix = translate(m_view_matrix, vec3f::UP() * distance);
                    core.uniform(m_view_matrix, "view");
                }
            } else {
                if (m_window->IsKeyPressed(Key::RIGHT_ARROW)) {
                    m_transform.rotation = rotate_y(m_transform.rotation, -angle);
                    core.uniform(m_transform.scale * m_transform.rotation * m_transform.translation, "model");
                } else if (m_window->IsKeyPressed(Key::LEFT_ARROW)) {
                    m_transform.rotation = rotate_y(m_transform.rotation, angle);
                    core.uniform(m_transform.scale * m_transform.rotation * m_transform.translation, "model");
                }

                if (m_window->IsKeyPressed(Key::UP_ARROW)) {
                    m_transform.rotation = rotate_x(m_transform.rotation, -angle);
                    core.uniform(m_transform.scale * m_transform.rotation * m_transform.translation, "model");
                } else if (m_window->IsKeyPressed(Key::DOWN_ARROW)) {
                    m_transform.rotation = rotate_x(m_transform.rotation, angle);
                    core.uniform(m_transform.scale * m_transform.rotation * m_transform.translation, "model");
                }

                if (m_window->IsKeyPressed(Key::D)) {
                    m_transform.translation = translate(m_transform.translation, vec3f::RIGHT() * distance);
                    core.uniform(m_transform.scale * m_transform.rotation * m_transform.translation, "model");
                } else if (m_window->IsKeyPressed(Key::A)) {
                    m_transform.translation = translate(m_transform.translation, vec3f::LEFT() * distance);
                    core.uniform(m_transform.scale * m_transform.rotation * m_transform.translation, "model");
                }

                if (m_window->IsKeyPressed(Key::W)) {
                    m_transform.translation = translate(m_transform.translation, vec3f::UP() * distance);
                    core.uniform(m_transform.scale * m_transform.rotation * m_transform.translation, "model");
                } else if (m_window->IsKeyPressed(Key::S)) {
                    m_transform.translation = translate(m_transform.translation, vec3f::DOWN() * distance);
                    core.uniform(m_transform.scale * m_transform.rotation * m_transform.translation, "model");
                }

                if (m_window->IsKeyPressed(Key::Z)) {
                    m_transform.translation = translate(m_transform.translation, vec3f::BACKWARD() * distance);
                    core.uniform(m_transform.scale * m_transform.rotation * m_transform.translation, "model");
                } else if (m_window->IsKeyPressed(Key::X)) {
                    m_transform.translation = translate(m_transform.translation, vec3f::FORWARD() * distance);
                    core.uniform(m_transform.scale * m_transform.rotation * m_transform.translation, "model");
                }
            }
        #pragma endregion input
         
            core.bind_shader(m_gouraud_shader);
            core.uniform(perspective(math::to_radians(90.0f), (float)m_window->GetWidth() / m_window->GetHeight(), 1.0f, 100.0f), "projection");
            const Object& object = m_objects["head"];
            core.bind_buffer(buffer_type::VERTEX, object.vbo);
            core.bind_buffer(buffer_type::INDEX, object.ibo);
            core.render(model_render_mode);

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