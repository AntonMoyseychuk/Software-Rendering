#include "math_3d/lin_math.hpp"

#include "math_3d/vector.hpp"
#include "math_3d/mat3x3.hpp"

#include <stdexcept>


namespace math {
    float LinMath::Dot(const Vec3f &a, const Vec3f &b) noexcept {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    Vec3f LinMath::Cross(const Vec3f &a, const Vec3f &b) noexcept {
        return Vec3f(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }
    
    Vec3f LinMath::Mult(const Vec3f vec, const Mat3x3 &mat) noexcept {
        return Vec3f(
            vec.x * mat[0][0] + vec.y * mat[1][0] + vec.z * mat[2][0],
            vec.x * mat[0][1] + vec.y * mat[1][1] + vec.z * mat[2][1],
            vec.x * mat[0][2] + vec.y * mat[1][2] + vec.z * mat[2][2]
        );
    }

    Mat3x3 LinMath::Transpose(const Mat3x3& mat) noexcept {
        auto temp = mat;

        for (std::size_t i = 0; i < temp.order; ++i) {
            for (std::size_t j = i; j < temp.order; ++j) {
                if (i != j) std::swap(temp[i][j], temp[j][i]);
            }
        }

        return temp;
    }
    
    Mat3x3 LinMath::Inverse(const Mat3x3& mat) {
        Mat3x3 temp;

        auto determinant = mat.Determinant();
        if (abs(determinant) < 0.000001f) {
            throw std::runtime_error("Inverse matrix can't be found: determinant is equal 0");
        }

        for (std::size_t i = 0; i < mat.order; ++i) {
            for (std::size_t j = 0; j < mat.order; ++j) {
                temp[i][j] = (mat[(j + 1) % 3][(i + 1) % 3] * mat[(j + 2) % 3][(i + 2) % 3] 
                    - (mat[(j + 1) % 3][(i + 2) % 3] * mat[(j + 2) % 3][(i + 1) % 3])) / determinant;
            }
        }

        return temp;
    }
}