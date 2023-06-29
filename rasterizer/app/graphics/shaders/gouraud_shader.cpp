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
        
        const mat4f& model_matrix = get_uniform<mat4f>("model");

        // out(v->position * model_matrix, "frag_position");
        // out(v->normal * transpose(inverse(model_matrix)), "normal");

        VSOutData out;
        out.frag_position = (v->position * model_matrix).xyz;
        out.normal = (v->normal * transpose(inverse(model_matrix))).xyz;

        gl_Position = v->position * model_matrix * get_uniform<mat4f>("view") * get_uniform<mat4f>("projection");

        return out;
    }
    
    math::color GouraudShader::pixel(const std::any& vs_out) const noexcept {
        using namespace math;

        const VSOutData data = std::any_cast<VSOutData>(vs_out);

        // const color polygon_color = texture(sampler_2D(), data.texcoord);
        const color polygon_color = get_uniform<vec4f>("polygon_color");
        const color ambient = 0.05f * polygon_color;

        // const vec3f light_dir = normalize(in<vec4f>("frag_position").xyz - get_uniform<vec3f>("light_position"));
        const vec3f light_dir = normalize(data.frag_position - get_uniform<vec3f>("light_position"));
        // const float diff = std::max(dot(-light_dir, in<vec4f>("normal").xyz), 0.0f);
        const float diff = std::max(dot(-light_dir, data.normal), 0.0f);
        const color diffuse = diff * get_uniform<vec4f>("light_color") * get_uniform<float>("light_intensity") * polygon_color;

        if (between(diff, 0.0f, 0.05f)) {
            return ambient + diffuse;
        }

        // const vec3f view_dir = normalize(in<vec4f>("frag_position").xyz - get_uniform<vec3f>("camera_position"));
        const vec3f view_dir = normalize(data.frag_position - get_uniform<vec3f>("camera_position"));
        // const vec3f reflected = normalize(reflect(light_dir, in<vec4f>("normal").xyz));
        const vec3f reflected = normalize(reflect(light_dir, data.normal));
        const float spec = std::max(std::powf(dot(reflected, -view_dir), 50.0f), 0.0f);
        const color specular = spec * polygon_color;

        return ambient + diffuse + specular;
    }
}