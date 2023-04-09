#pragma once
#include "matrix.hpp"
#include "math.hpp"

#include <cassert>

namespace math {
    template <typename Type, std::size_t Dimension>
    Mat<Type, Dimension> Transpose(const Mat<Type, Dimension>& mat) noexcept {
        auto temp = mat;

        for (std::size_t i = 0; i < temp.dimension; ++i) {
            for (std::size_t j = i; j < temp.dimension; ++j) {
                if (i != j) std::swap(temp[j][i], temp[i][j]);
            }
        }

        return temp;
    }

    template <typename Matrix>
    auto Identity() noexcept -> Mat<typename Matrix::type, Matrix::dimension> { 
        return Mat<Matrix::type, Matrix::dimension>(); 
    }

    mat3f Inverse(const mat3f& mat) noexcept;
    mat4f Inverse(const mat4f& mat) noexcept;

    mat4f Scale(const mat4f& mat, const vec3f& scale) noexcept;
    mat4f Translate(const mat4f& mat, const vec3f& translation) noexcept;
    
    mat4f RotateX(const mat4f& mat, float angle_radians) noexcept;
    mat4f RotateY(const mat4f& mat, float angle_radians) noexcept;
    mat4f RotateZ(const mat4f& mat, float angle_radians) noexcept;

    mat4f LookAtRH(const vec3f& eye, const vec3f& look_at, const vec3f& up) noexcept;
    mat4f Perspective(float fovy_radians, float aspect, float z_near, float z_far) noexcept;
}