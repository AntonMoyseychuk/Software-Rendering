#include "lin_math.hpp"

#include "vector.hpp"
#include "mat3x3.hpp"

#include <stdexcept>


namespace math {
    float LinMath::Dot(const Vector3 &a, const Vector3 &b) noexcept {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    Vector3 LinMath::Cross(const Vector3 &a, const Vector3 &b) noexcept {
        return Vector3(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
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