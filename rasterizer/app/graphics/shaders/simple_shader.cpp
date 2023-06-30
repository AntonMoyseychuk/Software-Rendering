#include "simple_shader.hpp"

namespace rasterization {
    struct VSInData {
        math::vec3f position;
        math::color color;
    };

    void SimpleShader::vertex(const void *vertex) const noexcept {
        using namespace math;
        
        const VSInData* v = (const VSInData*)vertex;
        gl_Position = v->position * get_uniform<mat4f>("model") * get_uniform<mat4f>("view") * get_uniform<mat4f>("projection");

        out(v->color, "color");
        // return v->color;
    }
    
    math::color SimpleShader::pixel() const noexcept {
        using namespace math;
        
        return in<color>("color");;

        // return std::any_cast<color>(vs_out);
    }
}