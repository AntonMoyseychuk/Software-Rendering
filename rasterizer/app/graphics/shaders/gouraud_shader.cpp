#include "gouraud_shader.hpp"

namespace rasterization {
    struct VSInData {
        math::vec3f position;
        math::vec3f normal;
        math::vec2f texcoord;
    };

    math::vec4f GouraudShader::vertex(const void *vertex, pd& _pd) const noexcept {
        using namespace math;
        const VSInData* v = (const VSInData*)vertex;
        
        const mat4f& model_matrix = get_uniform<mat4f>("model");

        out(vec4f(v->position, 1.0f) * model_matrix, "frag_position", _pd);
        out(v->texcoord, "texcoord", _pd);
        out(transpose(inverse(model_matrix)), "transpose_inverse_model", _pd);

        return vec4f(v->position, 1.0f) * model_matrix * get_uniform<mat4f>("view") * get_uniform<mat4f>("projection");
    }
    
    math::color GouraudShader::pixel(const pd& _pd) const noexcept {
        using namespace math;

        const vec3f& frag_position = in<vec4f>("frag_position", _pd).xyz;
        const vec2f& texcoord = in<vec2f>("texcoord", _pd);
        
        const vec3f normal = ((2.0f * texture(sampler_2D(1), texcoord) - vec4f(1.0f)) * in<mat4f>("transpose_inverse_model", _pd)).xyz;

        const color polygon_color = texture(sampler_2D(0), texcoord);
        const color ambient = 0.1f * polygon_color;

        const vec3f light_dir = normalize(frag_position - get_uniform<vec3f>("light_position"));
        const float diff = std::max(dot(-light_dir, normal), 0.0f);
        const color diffuse = diff * get_uniform<vec4f>("light_color") * get_uniform<float>("light_intensity") * polygon_color;

        if (between(diff, 0.0f, 0.05f)) {
            return ambient + diffuse;
        }

        const vec3f view_dir = normalize(frag_position - get_uniform<vec3f>("camera_position"));
        const vec3f reflected = normalize(reflect(light_dir, normal));
        const float spec = std::max(std::powf(dot(reflected, -view_dir), 50.0f), 0.0f);
        const color specular = spec * polygon_color;

        return ambient + diffuse + specular;
    }
}