#pragma once
#include "vector.hpp"

namespace math {
    template <typename Type>
    struct Mat3x3 {
        static_assert(std::is_arithmetic_v<Type>, "Type must be an arithmetic value");

        Mat3x3() {
            this->SetIdentity();
        }
        Mat3x3(const Vector<Type, 3>& r0, const Vector<Type, 3>& r1, const Vector<Type, 3>& r2) {
            const auto raw_size = sizeof(m_solid_array[0]);
            memcpy_s(m_solid_array[0], raw_size, &r0.x, raw_size);
            memcpy_s(m_solid_array[1], raw_size, &r1.x, raw_size);
            memcpy_s(m_solid_array[2], raw_size, &r2.x, raw_size);
        }
        Mat3x3(const Mat3x3& mat) {
            const auto arr_size = sizeof(m_solid_array);
            memcpy_s(m_solid_array, arr_size, mat.m_solid_array, arr_size);
        }
        Mat3x3(Mat3x3&& mat) {
            const auto arr_size = sizeof(m_solid_array);
            memmove_s(m_solid_array, arr_size, mat.m_solid_array, arr_size);
        }

        void SetIdentity() noexcept {
            memset(m_solid_array, 0, sizeof(m_solid_array));
            for (std::size_t i = 0; i < order; ++i) {
                m_solid_array[i][i] = 1.0f;
            }
        }
        float Determinant() const noexcept {
            return m_solid_array[0][0] * (m_solid_array[1][1] * m_solid_array[2][2] - m_solid_array[1][2] * m_solid_array[2][1]) +
                m_solid_array[0][1] * (m_solid_array[1][2] * m_solid_array[2][0] - m_solid_array[1][0] * m_solid_array[2][2]) +
                m_solid_array[0][2] * (m_solid_array[1][0] * m_solid_array[2][1] - m_solid_array[1][1] * m_solid_array[2][0]);
        }

        Mat3x3 Transpose() const noexcept {
            auto temp = *this;

            for (std::size_t i = 0; i < temp.order; ++i) {
                for (std::size_t j = i; j < temp.order; ++j) {
                    if (i != j) std::swap(temp[j][i], temp[i][j]);
                }
            }

            return temp;
        }

        Mat3x3 Inverse() const noexcept {
            auto determinant = this->Determinant();
            assert(IsTendsTo(determinant, 0.0f));
            
            Mat3x3 temp;
            for (std::size_t i = 0; i < mat.order; ++i) {
                for (std::size_t j = 0; j < mat.order; ++j) {
                    temp[i][j] = (this->operator[]((j + 1) % 3)[(i + 1) % 3] * this->operator[]((j + 2) % 3)[(i + 2) % 3] 
                        - (this->operator[]((j + 1) % 3)[(i + 2) % 3] * this->operator[]((j + 2) % 3)[(i + 1) % 3])) / determinant;
                }
            }

            return temp;
        }

        Mat3x3 operator*(const Mat3x3 &m) const noexcept {
            Mat3x3 temp;

            for (std::size_t i = 0; i < order; ++i) {
                for (std::size_t j = 0; j < order; ++j) {
                    temp[i][j] = 0.0f;
                    for (std::size_t k = 0; k < order; ++k) {
                        temp[i][j] += m_solid_array[i][k] * m.m_solid_array[k][j];
                    }
                }
            }

            return temp;
        }
        Mat3x3 operator*(float value) const noexcept {
            Mat3x3 mat;
            for (std::size_t i = 0; i < order; ++i) {
                mat[i][0] = m_solid_array[i][0] * value;
                mat[i][1] = m_solid_array[i][1] * value;
                mat[i][2] = m_solid_array[i][2] * value;
            }

            return mat;
        }

        float *operator[](std::size_t idx) {
            assert(idx < 3);
            return &m_solid_array[idx][0];
        }
        const float *operator[](std::size_t idx) const {
            assert(idx < 3);
            return &m_solid_array[idx][0];
        }

        static const std::size_t order = 3;

    protected:
        Type m_solid_array[order][order];
    };
}