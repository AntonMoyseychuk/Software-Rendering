#include "gouraud_shader.hpp"

namespace rasterization {
    struct VSInData {
        math::vec3f position;
        math::vec3f normal;
        math::vec2f texcoord;
    };

    math::vec4f GouraudShader::vertex(const void *vertex, pipeline_data& pack) const noexcept {
        using namespace math;
        const VSInData* v = (const VSInData*)vertex;
        
        const mat4f& model_matrix = get_uniform<mat4f>("model");

        out(v->position * model_matrix, "frag_position", pack);
        out(v->normal * transpose(inverse(model_matrix)), "normal", pack);
        out(v->texcoord, "texcoord", pack);

        return v->position * model_matrix * get_uniform<mat4f>("view") * get_uniform<mat4f>("projection");
    }
    
    math::color GouraudShader::pixel(const pipeline_data& pack) const noexcept {
        using namespace math;

        const color polygon_color = texture(sampler_2D(), in<vec2f>("texcoord", pack));
        // const color polygon_color = get_uniform<vec4f>("polygon_color");
        const color ambient = 0.1f * polygon_color;

        const vec3f light_dir = normalize(in<vec4f>("frag_position", pack).xyz - get_uniform<vec3f>("light_position"));
        const float diff = std::max(dot(-light_dir, in<vec4f>("normal", pack).xyz), 0.0f);
        const color diffuse = diff * get_uniform<vec4f>("light_color") * get_uniform<float>("light_intensity") * polygon_color;

        if (between(diff, 0.0f, 0.05f)) {
            return ambient + diffuse;
        }

        const vec3f view_dir = normalize(in<vec4f>("frag_position", pack).xyz - get_uniform<vec3f>("camera_position"));
        const vec3f reflected = normalize(reflect(light_dir, in<vec4f>("normal", pack).xyz));
        const float spec = std::max(std::powf(dot(reflected, -view_dir), 50.0f), 0.0f);
        const color specular = spec * polygon_color;

        return ambient + diffuse + specular;
    }
}