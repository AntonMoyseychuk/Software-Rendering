#include "mat_operations.hpp"

namespace math {
    mat4f scale(const mat4f &mat, const vec3f &scale) noexcept {
        return mat * mat4f(
            scale[0],       0,          0,      0,
                0,      scale[1],       0,      0,
                0,          0,      scale[2],   0,
                0,          0,          0,      1
        );
    }
    
    mat4f translate(const mat4f &mat, const vec3f &translation) noexcept {
        return mat * mat4f(
                1,              0,              0,       0,
                0,              1,              0,       0,
                0,              0,              1,       0,
            translation.x, translation.y, translation.z, 1
        );
    }
    
    mat4f rotate_x(const mat4f &mat, float angle_radians) noexcept {
        const float cos_angle = std::cos(angle_radians);
        const float sin_angle = std::sin(angle_radians);

        return mat * mat4f(
            1,     0,         0,        0,
			0,  cos_angle, sin_angle,   0,
            0, -sin_angle, cos_angle,   0,
            0,     0,         0,        1
        );
    }
    
    mat4f rotate_y(const mat4f &mat, float angle_radians) noexcept {
        const float cos_angle = std::cos(angle_radians);
        const float sin_angle = std::sin(angle_radians);

        return mat * mat4f(
            cos_angle,  0,  sin_angle, 0,
			    0,      1,      0,     0,
            -sin_angle, 0, cos_angle,  0,
                0,      0,      0,     1
        );
    }
    
    mat4f rotate_z(const mat4f &mat, float angle_radians) noexcept {
        const float cos_angle = cos(angle_radians);
        const float sin_angle = sin(angle_radians);

        return mat * mat4f(
            cos_angle, -sin_angle, 0, 0,
            sin_angle,  cos_angle, 0, 0,
                0,         0,      1, 0,
                0,         0,      0, 1
        );
    }

    mat4f look_at_rh(const vec3f &eye, const vec3f &look_at, const vec3f &up) noexcept {
        const auto forward = normalize(eye - look_at);
        const auto right = normalize(cross(normalize(up), forward));
        const auto new_up = cross(forward, right);

        return mat4f(vec4f(right, 0.0f), vec4f(new_up, 0.0f), vec4f(forward, 0.0f), vec4f(-eye, 1.0f));
    }

    mat4f perspective(float fovy_radians, float aspect, float near, float far) noexcept {
        assert(aspect > 0.0f);
		assert(near > 0.0f && far > 0.0f && !is_tends_to(near, far));

		const float tan_half_fovy = std::tanf(fovy_radians / 2.0f);

		mat4f result;
        result[0][0] = 1.0f / (aspect * tan_half_fovy);
		result[1][1] = 1.0f / (tan_half_fovy);
		result[2][2] = (far + near) / (far - near);
		result[2][3] = -1.0f;
		result[3][2] = -2.0f * far * near / (far - near);
		result[3][3] = 0.0f;

		return result;
    }
    
    mat4f viewport(uint32_t width, uint32_t height) noexcept {
        return mat4f(
            0.5f * width,             0.0f,     0.0f,      0.0f,
                    0.0f,   -0.5f * height,     0.0f,      0.0f,
                    0.0f,             0.0f,     1.0f,      0.0f,
            0.5f * width,    0.5f * height,     0.0f,      1.0f
        );
    }
}