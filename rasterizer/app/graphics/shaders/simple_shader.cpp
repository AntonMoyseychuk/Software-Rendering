#include "simple_shader.hpp"

namespace rasterization {
    struct VSInData {
        math::vec3f position;
        math::color color;
    };

    std::any SimpleShader::vertex(const void *vertex) const noexcept {
        using namespace math;
        
        const VSInData* v = (const VSInData*)vertex;
        gl_Position = v->position * get_uniform<mat4f>("model") * get_uniform<mat4f>("view") * get_uniform<mat4f>("projection");

        return v->color;
    }
    
    math::color SimpleShader::pixel(const std::any& vs_out) const noexcept {
        return std::any_cast<math::color>(vs_out);
    }
}