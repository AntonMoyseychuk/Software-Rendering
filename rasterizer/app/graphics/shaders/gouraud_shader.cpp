#include "gouraud_shader.hpp"

namespace rasterization {
    struct VSInData {
        math::vec3f position;
        math::vec3f normal;
        math::vec2f texcoord;
    };

    struct VSOutData {
        math::vec3f frag_position;
        math::color polygon_color;
        math::vec3f normal;
    };

    std::any GouraudShader::vertex(const void *vertex) const noexcept {
        using namespace math;
        const VSInData* v = (const VSInData*)vertex;
        
        const mat4f& model_matrix = get_mat4_uniform("model");

        VSOutData out;
        out.frag_position = (v->position * model_matrix).xyz;
        out.normal = (v->normal * transpose(inverse(model_matrix))).xyz;
        out.polygon_color = get_vec4_uniform("polygon_color");

        gl_Position = v->position * model_matrix * get_mat4_uniform("view") * get_mat4_uniform("projection");

        return out;
    }
    
    math::color GouraudShader::pixel(const std::any& vs_out) const noexcept {
        using namespace math;

        const VSOutData data = std::any_cast<VSOutData>(vs_out);

        const color light_color = get_vec4_uniform("light_color");
        const vec3f light_dir = normalize(data.frag_position - get_vec3_uniform("light_position"));
        
        const color ambient = 0.1f * data.polygon_color;
        const color diffuse = std::max(dot(data.normal, light_dir), 0.0f) * get_float_uniform("light_intensity") * light_color * data.polygon_color;
        const color specular = std::powf(std::max(dot(light_dir, data.normal), 0.0f), 50.0f) * light_color * data.polygon_color;

        return ambient + diffuse + specular;
    }
}