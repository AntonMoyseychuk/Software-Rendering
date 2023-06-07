#include "shader_engine.hpp"
#include <cassert>

#define ASSERT_SHADER_ID_VALIDITY(id) assert(shader_programs.count((id)) == 1)

namespace rasterization::gfx {
    _shader_engine &_shader_engine::get() noexcept {
        static _shader_engine engine;
        return engine;
    }
    
    size_t _shader_engine::create_shader(const std::shared_ptr<shader> &shader) noexcept {
        size_t id;
        do {
            id = math::random((size_t)0, SIZE_MAX - 1) + 1;
        } while (shader_programs.count(id) != 0);

        shader_programs[id] = _shader_program { shader, uniform_buffer() };
        shader_programs[id].shader->m_uniform_buffer = &shader_programs[id].uniform_buffer;

        return id;
    }
    
    void _shader_engine::bind_shader(size_t shader_id) noexcept {
        ASSERT_SHADER_ID_VALIDITY(shader_id);
        curr_shader = shader_id;
    }
    
    void _shader_engine::uniform(const std::string &uniform_name, const math::mat4f &mat) noexcept {
        ASSERT_SHADER_ID_VALIDITY(curr_shader);
        shader_programs[curr_shader].uniform_buffer.mat4f_uniforms[uniform_name] = mat;
    }
    
    void _shader_engine::uniform(const std::string &uniform_name, const math::vec4f &vec) noexcept {
        ASSERT_SHADER_ID_VALIDITY(curr_shader);
        shader_programs[curr_shader].uniform_buffer.vec4f_uniforms[uniform_name] = vec;
    }
    
    void _shader_engine::uniform(const std::string &uniform_name, const math::vec3f &vec) noexcept {
        ASSERT_SHADER_ID_VALIDITY(curr_shader);
        shader_programs[curr_shader].uniform_buffer.vec3f_uniforms[uniform_name] = vec;
    }
    
    void _shader_engine::uniform(const std::string &uniform_name, const math::vec2f &vec) noexcept {
        ASSERT_SHADER_ID_VALIDITY(curr_shader);
        shader_programs[curr_shader].uniform_buffer.vec2f_uniforms[uniform_name] = vec;
    }
    
    void _shader_engine::uniform(const std::string &uniform_name, float value) noexcept {
        ASSERT_SHADER_ID_VALIDITY(curr_shader);
        shader_programs[curr_shader].uniform_buffer.float_uniforms[uniform_name] = value;
    }
}