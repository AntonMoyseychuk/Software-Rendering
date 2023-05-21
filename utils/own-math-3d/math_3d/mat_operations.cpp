#include "mat_operations.hpp"

#include "vec4.hpp"
#include "vec_operations.hpp"

#include "util.hpp"

#include <cmath>
#include <cassert>

namespace math {
    mat4f scale(const mat4f &mat, const vec4f &scale) noexcept {
        return mat * mat4f(
            scale[0],       0,          0,      0,
                0,      scale[1],       0,      0,
                0,          0,      scale[2],   0,
                0,          0,          0,      1
        );
    }
    
    mat4f translate(const mat4f &mat, const vec4f &translation) noexcept {
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
            cos_angle,  0,  sin_angle, 0,
			    0,      1,      0,     0,
            -sin_angle, 0, cos_angle,  0,
                0,      0,      0,     1
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

    mat4f look_at_rh(const vec4f &eye, const vec4f &look_at, const vec4f &up) noexcept {
        const auto forward = normalize(eye - look_at);
        const auto right = normalize(cross(up, forward));
        const auto new_up = cross(forward, right);

        return mat4f(right, new_up, forward, vec4f(eye.xyz, 1.0f));
    }

    mat4f perspective(float fovy_radians, float aspect, float z_near, float z_far) noexcept {
        assert(aspect > 0.0f && "aspect must be greater than 0");
		assert(is_tends_to(z_near, z_far) == false && "z_near must not to tend to z_far");

		const auto tan_half_fovy = tanf(fovy_radians / 2.0f);

		mat4f result;
		result[0][0] = 1.0f / (aspect * tan_half_fovy);
		result[1][1] = 1.0f / (tan_half_fovy);
		result[2][2] = -(z_far + z_near) / (z_far - z_near);
		result[2][3] = -1.0f;
		result[3][2] = -(2.0f * z_far * z_near) / (z_far - z_near);
		return transpose(result);
    }
}