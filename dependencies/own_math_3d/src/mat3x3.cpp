#include "math_3d/mat3x3.hpp"
#include "math_3d/vector3.hpp"

#include <stdexcept>

namespace math
{
    Mat3x3::Mat3x3() {
        this->SetIdentity();
    }

    Mat3x3::Mat3x3(const Vector3f &r0, const Vector3f &r1, const Vector3f &r2) {
        const auto raw_size = sizeof(solid_array[0]);
        memcpy_s(solid_array[0], raw_size, &r0.x, raw_size);
        memcpy_s(solid_array[1], raw_size, &r1.x, raw_size);
        memcpy_s(solid_array[2], raw_size, &r2.x, raw_size);
    }

    void Mat3x3::SetIdentity() noexcept {
        memset(solid_array, 0, sizeof(solid_array));
        for (std::size_t i = 0; i < order; ++i) {
            solid_array[i][i] = 1.0f;
        }
    }

    float Mat3x3::Determinant() const noexcept  {
        return solid_array[0][0] * (solid_array[1][1] * solid_array[2][2] - solid_array[1][2] * solid_array[2][1]) +
               solid_array[0][1] * (solid_array[1][2] * solid_array[2][0] - solid_array[1][0] * solid_array[2][2]) +
               solid_array[0][2] * (solid_array[1][0] * solid_array[2][1] - solid_array[1][1] * solid_array[2][0]);
    }

    Mat3x3 Mat3x3::operator*(const Mat3x3 &m) const noexcept {
        Mat3x3 temp;

        for (std::size_t i = 0; i < order; ++i) {
            for (std::size_t j = 0; j < order; ++j) {
                temp[i][j] = 0.0f;
                for (std::size_t k = 0; k < order; ++k) {
                    temp[i][j] += solid_array[i][k] * m.solid_array[k][j];
                }
            }
        }

        return temp;
    }

    Mat3x3 Mat3x3::operator*(float value) const noexcept {
        Mat3x3 mat;
        for (std::size_t i = 0; i < order; ++i) {
            mat[i][0] = solid_array[i][0] * value;
            mat[i][1] = solid_array[i][1] * value;
            mat[i][2] = solid_array[i][2] * value;
        }

        return mat;
    }

    float *Mat3x3::operator[](std::size_t idx) {
        return &solid_array[idx][0];
    }

    const float *Mat3x3::operator[](std::size_t idx) const  {
        return &solid_array[idx][0];
    }
}