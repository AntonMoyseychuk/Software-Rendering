#include "model_shader.hpp"

namespace rasterization {
    struct VSInData {
        math::vec3f position;
    };

    std::any ModelShader::vertex(const void *vertex) const noexcept {
        using namespace math;
        
        const VSInData* v = (const VSInData*)vertex;
        gl_Position = v->position * get_mat4_uniform("model") * get_mat4_uniform("view") * get_mat4_uniform("projection");

        return nullptr;
    }
    
    math::color ModelShader::pixel(const std::any& vs_out) const noexcept {
        return get_vec4_uniform("polygon_color");
    }
}