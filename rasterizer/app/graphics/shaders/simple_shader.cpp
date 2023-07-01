#include "simple_shader.hpp"

namespace rasterization {
    struct VSInData {
        math::vec3f position;
        math::color color;
    };

    math::vec4f SimpleShader::vertex(const void *vertex, pipeline_data& pack) const noexcept {
        using namespace math;
        
        const VSInData* v = (const VSInData*)vertex;

        out(v->color, "color", pack);
        return v->position * get_uniform<mat4f>("model") * get_uniform<mat4f>("view") * get_uniform<mat4f>("projection");
    }
    
    math::color SimpleShader::pixel(const pipeline_data& pack) const noexcept {
        return in<math::color>("color", pack);
    }
}