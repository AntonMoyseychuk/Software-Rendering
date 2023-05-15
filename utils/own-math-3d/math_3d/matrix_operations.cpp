#include "matrix_operations.hpp"

namespace math {
    mat3f math::Inverse(const mat3f &mat) noexcept {
        auto determinant = mat.Determinant();
        assert(IsTendsTo(determinant, 0.0f));
        
        mat3f temp;
        for (std::size_t i = 0; i < mat.dimension; ++i) {
            for (std::size_t j = 0; j < mat.dimension; ++j) {
                temp[i][j] = (mat[(j + 1) % 3][(i + 1) % 3] * mat[(j + 2) % 3][(i + 2) % 3] 
                    - (mat[(j + 1) % 3][(i + 2) % 3] * mat[(j + 2) % 3][(i + 1) % 3])) / determinant;
            }
        }
    
        return temp;
    }
    
    mat4f Inverse(const mat4f &mat) noexcept {
        auto determinant = mat.Determinant();
        assert(IsTendsTo(determinant, 0.0f) == false);
        
        mat4f result;
        
        auto transpoced_mat = math::Transpose(mat);
        mat3f minor;
        for (std::size_t i = 0; i < mat.dimension; ++i) {
            for (std::size_t j = 0; j < mat.dimension; ++j) {
                
                for (std::size_t y = 0, curr_y = 0; y < mat.dimension; ++y) {
                    if (y != i) {
                        for (std::size_t x = 0, curr_x = 0; x < mat.dimension; ++x) {
                            if (x != j) {
                                minor[curr_y][curr_x] = transpoced_mat[y][x];
                                ++curr_x;
                            }
                        }
                        ++curr_y;
                    }
                }
                
                result[i][j] = minor.Determinant() * powf(-1.0f, static_cast<float>(i + j));
            }
        }

        return result /= determinant;
    }

    mat4f math::Scale(const mat4f &mat, const vec3f &scale) noexcept {
        return mat * mat4f(
            scale[0],       0,          0,      0,
                0,      scale[1],       0,      0,
                0,          0,      scale[2],   0,
                0,          0,          0,      1
        );
    }
    
    // mat4f Rotate(const mat4f &mat, const vec3f &axis, float angle_radians) noexcept {
    //     assert(false && "mat4f Rotate(const mat4f &mat, const vec3f &axis, float angle_radians) noexcept: Not implemented");
	// 	// const float cos_angle = cos(angle_radians);
	// 	// const float sin_angle = sin(angle_radians);
    // 
	// 	// axis = Normalize(axis);
	// 	// vec3f temp((1.0f - cos_angle) * axis);
    // 
	// 	// Mat<Type, 4> rotate;
	// 	// rotate[0][0] = cos_angle + temp[0] * axis[0];
	// 	// rotate[0][1] = temp[0] * axis[1] + sin_angle * axis[2];
	// 	// rotate[0][2] = temp[0] * axis[2] - sin_angle * axis[1];
    // 
	// 	// rotate[1][0] = temp[1] * axis[0] - sin_angle * axis[2];
	// 	// rotate[1][1] = cos_angle + temp[1] * axis[1];
	// 	// rotate[1][2] = temp[1] * axis[2] + sin_angle * axis[0];
    // 
	// 	// rotate[2][0] = temp[2] * axis[0] + sin_angle * axis[1];
	// 	// rotate[2][1] = temp[2] * axis[1] - sin_angle * axis[0];
	// 	// rotate[2][2] = cos_angle + temp[2] * axis[2];
    // 
	// 	// Mat<Type, 4> result;
	// 	// result[0] = mat[0] * rotate[0][0] + mat[1] * rotate[0][1] + mat[2] * rotate[0][2];
	// 	// result[1] = mat[0] * rotate[1][0] + mat[1] * rotate[1][1] + mat[2] * rotate[1][2];
	// 	// result[2] = mat[0] * rotate[2][0] + mat[1] * rotate[2][1] + mat[2] * rotate[2][2];
	// 	// result[3] = mat[3];
	// 	// return result;
    //  
    //     return mat4f();
    // }

    mat4f RotateX(const mat4f& mat, float angle_radians) noexcept {
        const float cos_angle = cos(angle_radians);
        const float sin_angle = sin(angle_radians);

        return mat * mat4f(
            1, 	   0, 		   0,     0,
            0, cos_angle, -sin_angle, 0,
            0, sin_angle,  cos_angle, 0,
            0, 	   0, 		   0,     1
        );
    }

    mat4f RotateY(const mat4f& mat, float angle_radians) noexcept {
        const float cos_angle = cos(angle_radians);
        const float sin_angle = sin(angle_radians);

        return mat * mat4f(
            cos_angle,  0,  sin_angle, 0,
			    0,      1,      0,     0,
            -sin_angle, 0, cos_angle,  0,
                0,      0,      0,     1
        );
    }

    mat4f RotateZ(const mat4f& mat, float angle_radians) noexcept {
        const float cos_angle = cos(angle_radians);
        const float sin_angle = sin(angle_radians);

        return mat * mat4f(
            cos_angle, -sin_angle, 0, 0,
            sin_angle,  cos_angle, 0, 0,
                0,         0,      1, 0,
                0,         0,      0, 1
        );
    }
    
    mat4f Translate(const mat4f &mat, const vec3f &translation) noexcept {
        return mat * mat4f(
                1,              0,              0,       0,
                0,              1,              0,       0,
                0,              0,              1,       0,
            translation.x, translation.y, translation.z, 1
        );
    }
    
    mat4f LookAtRH(const vec3f& eye, const vec3f& look_at, const vec3f& up) noexcept {
        // assert(false && "inline mat4f LookAtRH(const vec3f& eye, const vec3f& look_at, const vec3f& up) noexcept: Not implemented");

        const auto forward = Normalize(eye - look_at);
        const auto right = Normalize(Cross(up, forward));
        const auto new_up = Cross(forward, right);

        return mat4f(vec4f(right), vec4f(new_up), vec4f(forward), vec4f(eye, 1.0f));
    }
    
    mat4f Perspective(float fovy_radians, float aspect, float z_near, float z_far) noexcept {
        // assert(false && "inline mat4f Perspective(float fovy, float aspect, float z_near, float z_far) noexcept: Not implemented");
        
        assert(aspect > 0.0f && "aspect must be greater than 0");
		assert(IsTendsTo(z_near, z_far) == false && "z_near must not to tend to z_far");

		const auto tan_half_fovy = tanf(fovy_radians / 2.0f);

		mat4f result;
		result[0][0] = 1.0f / (aspect * tan_half_fovy);
		result[1][1] = 1.0f / (tan_half_fovy);
		result[2][2] = -(z_far + z_near) / (z_far - z_near);
		result[2][3] = -1.0f;
		result[3][2] = -(2.0f * z_far * z_near) / (z_far - z_near);
		return Transpose(result);
    }
}