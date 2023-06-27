#include "gouraud_shader.hpp"

namespace rasterization {
    struct VSInData {
        math::vec3f position;
        math::vec3f normal;
        math::vec2f texcoord;
    };

    struct VSOutData {
        math::vec3f frag_position;
        math::vec3f normal;
    };

    std::any GouraudShader::vertex(const void *vertex) const noexcept {
        using namespace math;
        const VSInData* v = (const VSInData*)vertex;
        
        const mat4f& model_matrix = get_mat4_uniform("model");

        VSOutData out;
        out.frag_position = (v->position * model_matrix).xyz;
        out.normal = (v->normal * transpose(inverse(model_matrix))).xyz;

        gl_Position = v->position * model_matrix * get_mat4_uniform("view") * get_mat4_uniform("projection");

        return out;
    }
    
    math::color GouraudShader::pixel(const std::any& vs_out) const noexcept {
        using namespace math;

        const VSOutData data = std::any_cast<VSOutData>(vs_out);

        // const color polygon_color = texture(data.texcoord);
        const color polygon_color = get_vec4_uniform("polygon_color");
        const color ambient = 0.05f * polygon_color;

        const vec3f light_dir = normalize(data.frag_position - get_vec3_uniform("light_position"));
        const float diff = std::max(dot(-light_dir, data.normal), 0.0f);
        const color diffuse = diff * get_vec4_uniform("light_color") * get_f32_uniform("light_intensity") * polygon_color;

        const vec3f view_dir = normalize(data.frag_position - get_vec3_uniform("camera_position"));
        const vec3f reflected = normalize(reflect(light_dir, data.normal));
        const float spec = std::max(std::powf(dot(reflected, -view_dir), 50.0f), 0.0f);
        const color specular = spec * get_f32_uniform("light_intensity") * polygon_color;

        return ambient + diffuse + specular;
    }
}