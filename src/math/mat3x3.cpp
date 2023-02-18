#include "mat3x3.hpp"
#include "vector.hpp"

#include <stdexcept>

namespace math
{
    Mat3x3::Mat3x3() {
        this->SetIdentity();
    }

    Mat3x3::Mat3x3(const Vector3 &r0, const Vector3 &r1, const Vector3 &r2) {
        m_matrix.vectors[0] = r0;
        m_matrix.vectors[1] = r1;
        m_matrix.vectors[2] = r2;        
    }

    void Mat3x3::SetIdentity() noexcept {
        m_matrix.vectors[0] = {1.0f, 0.0f, 0.0f};
        m_matrix.vectors[1] = {0.0f, 1.0f, 0.0f};
        m_matrix.vectors[2] = {0.0f, 0.0f, 1.0f};
    }

    float Mat3x3::Determinant() const noexcept  {
        return m_matrix.vectors[0][0] * (m_matrix.vectors[1].Y() * m_matrix.vectors[2].Z() - m_matrix.vectors[1].Z() * m_matrix.vectors[2].Y()) +
               m_matrix.vectors[0][1] * (m_matrix.vectors[1].Z() * m_matrix.vectors[2].X() - m_matrix.vectors[1].X() * m_matrix.vectors[2].Z()) +
               m_matrix.vectors[0][2] * (m_matrix.vectors[1].X() * m_matrix.vectors[2].Y() - m_matrix.vectors[1].Y() * m_matrix.vectors[2].X());
    }

    // Mat3x3 Mat3x3::Transpose() const noexcept {
    //     auto temp = *this;
    //
    //     for (std::size_t i = 0; i < RAW_COUNT; ++i) {
    //         for (std::size_t j = i; j < COLUMN_COUNT; ++j) {
    //             if (i != j) std::swap(temp[i][j], temp[j][i]);
    //         }
    //     }
    //
    //     return temp;
    // }

    // Mat3x3 Mat3x3::Invert() const {
    //     Mat3x3 temp;
    //
    //     auto determinant = this->Determinant();
    //     if (abs(determinant) < 0.000001f) {
    //         throw std::runtime_error("Inverse matrix can't be found: determinant is equal 0");
    //     }
    //
    //     for (std::size_t i = 0; i < RAW_COUNT; ++i) {
    //         for (std::size_t j = 0; j < COLUMN_COUNT; ++j) {
    //             temp[i][j] = (m_matrix.solid_arrays[(j + 1) % 3][(i + 1) % 3] * m_matrix.solid_arrays[(j + 2) % 3][(i + 2) % 3] 
    //                 - (m_matrix.solid_arrays[(j + 1) % 3][(i + 2) % 3] * m_matrix.solid_arrays[(j + 2) % 3][(i + 1) % 3])) / determinant;
    //         }
    //     }
    //
    //     return temp;
    // }

    Mat3x3 Mat3x3::operator*(const Mat3x3 &m) const noexcept {
        Mat3x3 temp;

        for (std::size_t i = 0; i < RAW_COUNT; ++i) {
            for (std::size_t j = 0; j < COLUMN_COUNT; ++j) {
                temp[i][j] = 0.0f;
                for (std::size_t k = 0; k < COLUMN_COUNT; ++k) {
                    temp[i][j] += m_matrix.solid_arrays[i][k] * m.m_matrix.solid_arrays[k][j];
                }
            }
        }

        return temp;
    }

    Mat3x3 Mat3x3::operator*(float value) const noexcept {
        Mat3x3 mat;
        for (std::size_t i = 0; i < RAW_COUNT; ++i) {
            mat[i][0] = m_matrix.solid_arrays[i][0] * value;
            mat[i][1] = m_matrix.solid_arrays[i][1] * value;
            mat[i][2] = m_matrix.solid_arrays[i][2] * value;
        }

        return mat;
    }

    float *Mat3x3::operator[](std::size_t idx) {
        return &m_matrix.solid_arrays[idx][0];
    }

    const float *Mat3x3::operator[](std::size_t idx) const  {
        return &m_matrix.solid_arrays[idx][0];
    }

    std::size_t Mat3x3::Order() noexcept {
        return RAW_COUNT;
    }
}