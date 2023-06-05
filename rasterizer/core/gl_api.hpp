#pragma once
#include "math_3d/vec4.hpp"
#include "math_3d/mat4.hpp"

#include "buffer_engine.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace rasterization::gfx {
    class gl_api final {
        friend class Rasterizer;

    public:
        static const gl_api& get() noexcept;

    #pragma region buffer_engine_api
        size_t create_vertex_buffer(const void* buffer, size_t size) const noexcept;
        void vertex_attrib_pointer(size_t vbo_id, size_t size, attrib_data_type type, size_t stride, const void* ptr) const noexcept;

        size_t create_index_buffer(const size_t* buffer, size_t count) const noexcept;
        
        void erase_buffer(size_t id) const noexcept;
        void erase_index_buffer(size_t id) const noexcept;

        void bind(buffer_type type, size_t id) const noexcept;
    #pragma endregion buffer_engine_api

        void uniform(const std::string& uniform_name, const math::mat4f& mat) const noexcept;
        void uniform(const std::string& uniform_name, const math::vec4f& vec) const noexcept;
        void uniform(const std::string& uniform_name, const math::vec3f& vec) const noexcept;
        void uniform(const std::string& uniform_name, const math::vec2f& vec) const noexcept;
        void uniform(const std::string& uniform_name, float value) const noexcept;

        void viewport(uint32_t width, uint32_t height) const noexcept;

    private:
        gl_api() noexcept;

        //temporary
        const std::vector<uint8_t>& _get_binded_vertex_buffer() const noexcept;
        const std::vector<size_t>& _get_binded_index_buffer() const noexcept;
        //

    private:
        const buffer_engine& m_buf_engine;

        mutable std::unordered_map<std::string, math::mat4f> m_mat4_uniforms;
        mutable std::unordered_map<std::string, math::vec4f> m_vec4f_uniforms;
        mutable std::unordered_map<std::string, math::vec3f> m_vec3f_uniforms;
        mutable std::unordered_map<std::string, math::vec2f> m_vec2f_uniforms;

        mutable std::unordered_map<std::string, float> m_float_uniforms;

        mutable math::mat4f m_viewport;
    };
}