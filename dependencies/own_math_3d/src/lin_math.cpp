#include "math_3d/lin_math.hpp"
#include <algorithm>


namespace math {
    float LinMath::ToDegrees(float radians) noexcept {
        return (180.0f * radians) / PI;
    }
    
    float LinMath::ToRadians(float degrees) noexcept {
        return degrees * (PI / 180.0);
    }

    Mat3x3 LinMath::Transpose(const Mat3x3 &mat) noexcept
    {
        auto temp = mat;

        for (std::size_t i = 0; i < temp.order; ++i) {
            for (std::size_t j = i; j < temp.order; ++j) {
                if (i != j) std::swap(temp[j][i], temp[i][j]);
            }
        }

        return temp;
    }

    bool LinMath::Inverse(const Mat3x3& mat, Mat3x3& out) {
        auto determinant = mat.Determinant();
        if (IsTendsTo(determinant, 0.0f)) {
            return false;
        }

        for (std::size_t i = 0; i < mat.order; ++i) {
            for (std::size_t j = 0; j < mat.order; ++j) {
                out[i][j] = (mat[(j + 1) % 3][(i + 1) % 3] * mat[(j + 2) % 3][(i + 2) % 3] 
                    - (mat[(j + 1) % 3][(i + 2) % 3] * mat[(j + 2) % 3][(i + 1) % 3])) / determinant;
            }
        }

        return true;
    }
}