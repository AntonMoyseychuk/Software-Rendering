#pragma once
#include "buffer_engine.hpp"
#include "render_engine.hpp"
#include "shader_engine.hpp"


namespace gl {
    class gl_api final {
        friend struct _render_engine;

    public:
        gl_api(const gl_api& api) = delete;
        gl_api& operator=(const gl_api& api) = delete;

        static gl_api& get() noexcept;

    #pragma region rasterizer_api
        bool bind_window(win_framewrk::Window* window) noexcept;
        const win_framewrk::Window* is_window_binded() const noexcept;

        void render(render_mode mode) const noexcept;
        void swap_buffers() const noexcept;
        void clear_depth_buffer() const noexcept;

        void set_clear_color(const math::color& color) noexcept;
    #pragma endregion rasterizer_api

    #pragma region buffer_engine_api
        size_t create_vertex_buffer(const void* buffer, size_t size) const noexcept;
        size_t create_index_buffer(const size_t* buffer, size_t count) const noexcept;
        
        void delete_vertex_buffer(size_t id) const noexcept;
        void delete_index_buffer(size_t id) const noexcept;

        void bind_buffer(buffer_type type, size_t id) const noexcept;

        void set_buffer_element_size(size_t size) const noexcept;
    #pragma endregion buffer_engine_api

    #pragma region shader_engine_api
        size_t create_shader(const std::shared_ptr<_shader>& shader) const noexcept;
        void bind_shader(size_t shader_id) const noexcept;

        void uniform(const std::string& uniform_name, const math::quaternion& q) const noexcept;
        void uniform(const std::string& uniform_name, const math::mat4f& mat) const noexcept;
        void uniform(const std::string& uniform_name, const math::vec4f& vec) const noexcept;
        void uniform(const std::string& uniform_name, const math::vec3f& vec) const noexcept;
        void uniform(const std::string& uniform_name, const math::vec2f& vec) const noexcept;
        void uniform(const std::string& uniform_name, float value) const noexcept;
        void uniform(const std::string& uniform_name, double value) const noexcept;
        void uniform(const std::string& uniform_name, int8_t value) const noexcept;
        void uniform(const std::string& uniform_name, int16_t value) const noexcept;
        void uniform(const std::string& uniform_name, int32_t value) const noexcept;
        void uniform(const std::string& uniform_name, int64_t value) const noexcept;
    #pragma endregion shader_engine_api

    // #pragma region texture_engine_api
    //     size_t create_unique_texture() const noexcept;
    //     void texture_image_2D(size_t detail_level, /*tex_internal_format format,*/size_t width, size_t height, /*tex_format format,*/ const void* data) const noexcept;
    //
    //     void bind_texture(size_t texture_id) const noexcept;
    //
    //     void texture_parameter() const noexcept;
    // #pragma endregion texture_engine_api

        void viewport(uint32_t width, uint32_t height) const noexcept;

    private:
        gl_api() noexcept;

    private:
        _buffer_engine& m_buf_engine;
        _render_engine& m_render_engine;
        _shader_engine& m_shader_engine;

        mutable math::mat4f m_viewport;
    };
}