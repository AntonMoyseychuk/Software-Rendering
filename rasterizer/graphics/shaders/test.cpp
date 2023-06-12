#include "test.hpp"

namespace rasterization::gfx {
    struct Vertex {
        math::vec3f position;
    };

    math::vec4f SimpleAbstrShader::vertex(const void *vertex) const noexcept {
        using namespace math;
        
        const Vertex* v = (const Vertex*)vertex;
        return v->position * get_mat4_uniform("model") * get_mat4_uniform("view") * get_mat4_uniform("projection");
    }
    
    math::color SimpleAbstrShader::pixel(const void *vertex) const noexcept {
        return math::color();
    }
}