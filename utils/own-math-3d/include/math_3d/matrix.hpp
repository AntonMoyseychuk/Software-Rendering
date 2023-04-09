#pragma once
#include "vector.hpp"

namespace math {
    template <typename Type, std::size_t Dimension>
    struct Mat {
        static_assert(std::is_arithmetic_v<Type>, "template <typename Type, std::size_t Dimension> struct Mat: Type must be an arithmetic");
        static_assert(Dimension >= 3 && Dimension <= 4, "template <typename Type, std::size_t Dimension> struct Mat: Dimension must be 3 <= Dimension <= 4");
    };

    template <typename Type>
    struct Mat<Type, 4> {
        static_assert(std::is_arithmetic_v<Type>, "template <typename Type, std::size_t Dimension> struct Mat: Type must be an arithmetic");

        using type = typename Type;
        static constexpr std::size_t dimension = 4;

        Mat() {
            this->SetIdentity();
        }

        Mat(const Vector<Type, 4>& r0, const Vector<Type, 4>& r1, const Vector<Type, 4>& r2, const Vector<Type, 4>& r3) {
            const auto raw_size = sizeof(m_solid_array[0]);
            memcpy_s(m_solid_array[0], raw_size, &r0.x, raw_size);
            memcpy_s(m_solid_array[1], raw_size, &r1.x, raw_size);
            memcpy_s(m_solid_array[2], raw_size, &r2.x, raw_size);
            memcpy_s(m_solid_array[3], raw_size, &r3.x, raw_size);
        }

        Mat(Type m00, Type m01, Type m02, Type m03, 
            Type m10, Type m11, Type m12, Type m13,
            Type m20, Type m21, Type m22, Type m23, 
            Type m30, Type m31, Type m32, Type m33) 
        {
            const auto arr_size = sizeof(m_solid_array);
            memcpy_s(m_solid_array, arr_size, &m00, arr_size);
        }

        Mat(const Mat& mat) {
            const auto arr_size = sizeof(m_solid_array);
            memcpy_s(m_solid_array, arr_size, mat.m_solid_array, arr_size);
        }

        Mat(Mat&& mat) {
            const auto arr_size = sizeof(m_solid_array);
            memmove_s(m_solid_array, arr_size, mat.m_solid_array, arr_size);
        }

        void SetIdentity() noexcept {
            memset(m_solid_array, 0, sizeof(m_solid_array));
            for (std::size_t i = 0; i < dimension; ++i) {
                m_solid_array[i][i] = 1.0f;
            }
        }
        
        float Determinant() const noexcept {
            float det = 0.0f;

            det += m_solid_array[0][0] * Mat<Type, 3>(m_solid_array[1][1], m_solid_array[1][2], m_solid_array[1][3], 
                                                        m_solid_array[2][1], m_solid_array[2][2], m_solid_array[2][3], 
                                                        m_solid_array[3][1], m_solid_array[3][2], m_solid_array[3][3]).Determinant();
            
            det -= m_solid_array[0][1] * Mat<Type, 3>(m_solid_array[1][0], m_solid_array[1][2], m_solid_array[1][3], 
                                                        m_solid_array[2][0], m_solid_array[2][2], m_solid_array[2][3], 
                                                        m_solid_array[3][0], m_solid_array[3][2], m_solid_array[3][3]).Determinant();

            det += m_solid_array[0][2] * Mat<Type, 3>(m_solid_array[1][0], m_solid_array[1][1], m_solid_array[1][3], 
                                                        m_solid_array[2][0], m_solid_array[2][1], m_solid_array[2][3], 
                                                        m_solid_array[3][0], m_solid_array[3][1], m_solid_array[3][3]).Determinant();

            det -= m_solid_array[0][3] * Mat<Type, 3>(m_solid_array[1][0], m_solid_array[1][1], m_solid_array[1][2], 
                                                        m_solid_array[2][0], m_solid_array[2][1], m_solid_array[2][2], 
                                                        m_solid_array[3][0], m_solid_array[3][1], m_solid_array[3][2]).Determinant();

            return det;
        }

        bool operator==(const Mat &m) const noexcept {
            return memcmp(m_solid_array, m.m_solid_array, sizeof(type) * dimension * dimension) == 0;
        }

        bool operator!=(const Mat &m) const noexcept {
            return !(*this == m);
        }

        Mat& operator=(const Mat &m) noexcept {
            for (std::size_t i = 0; i < dimension; ++i) {
                m_vector_array[i] = m.m_vector_array[i];
            }

            return *this;
        }

        Mat operator*(const Mat &m) const noexcept {
            Mat temp;

            for (std::size_t i = 0; i < dimension; ++i) {
                for (std::size_t j = 0; j < dimension; ++j) {
                    temp[i][j] = 0.0f;
                    for (std::size_t k = 0; k < dimension; ++k) {
                        temp[i][j] += m_solid_array[i][k] * m.m_solid_array[k][j];
                    }
                }
            }

            return temp;
        }

        Mat operator*(float value) const noexcept {
            Mat mat;
            for (std::size_t i = 0; i < dimension; ++i) {
                mat[i][0] = m_solid_array[i][0] * value;
                mat[i][1] = m_solid_array[i][1] * value;
                mat[i][2] = m_solid_array[i][2] * value;
                mat[i][3] = m_solid_array[i][3] * value;
            }

            return mat;
        }

        Mat& operator*=(float value) noexcept {
            for (std::size_t i = 0; i < dimension; ++i) {
                m_solid_array[i][0] *= value;
                m_solid_array[i][1] *= value;
                m_solid_array[i][2] *= value;
                m_solid_array[i][3] *= value;
            }

            return *this;
        }

        Mat operator/(float value) const noexcept {
            Mat mat;
            for (std::size_t i = 0; i < dimension; ++i) {
                mat[i][0] = m_solid_array[i][0] / value;
                mat[i][1] = m_solid_array[i][1] / value;
                mat[i][2] = m_solid_array[i][2] / value;
                mat[i][4] = m_solid_array[i][3] / value;
            }

            return mat;
        }

        Mat& operator/=(float value) noexcept {
            for (std::size_t i = 0; i < dimension; ++i) {
                m_solid_array[i][0] /= value;
                m_solid_array[i][1] /= value;
                m_solid_array[i][2] /= value;
                m_solid_array[i][3] /= value;
            }

            return *this;
        }



        operator Mat<Type, 3>() const noexcept {
            return Mat<Type, 3>(m_solid_array[0][0], m_solid_array[0][1], m_solid_array[0][2],
                                m_solid_array[1][0], m_solid_array[1][1], m_solid_array[1][2],
                                m_solid_array[2][0], m_solid_array[2][1], m_solid_array[2][2]);
        }

        Vector<Type, dimension>& operator[](std::size_t idx) {
            assert(idx < dimension && __FUNCTION__ ": invalid idx value");
            return m_vector_array[idx];
        }

        const Vector<Type, dimension>& operator[](std::size_t idx) const {
            (idx < dimension && __FUNCTION__ ": invalid idx value");
            return m_vector_array[idx];
        }

    protected:
        union {
            Type m_solid_array[dimension][dimension];
            Vector<Type, dimension> m_vector_array[dimension];
        };
    };

    template <typename Type>
    struct Mat<Type, 3> {
        static_assert(std::is_arithmetic_v<Type>, "template <typename Type, std::size_t Dimension> struct Mat: Type must be an arithmetic");

        using type = typename Type;
        static constexpr std::size_t dimension = 3;

        Mat() {
            this->SetIdentity();
        }

        Mat(const Vector<Type, 3>& r0, const Vector<Type, 3>& r1, const Vector<Type, 3>& r2) {
            const auto raw_size = sizeof(m_solid_array[0]);
            memcpy_s(m_solid_array[0], raw_size, &r0.x, raw_size);
            memcpy_s(m_solid_array[1], raw_size, &r1.x, raw_size);
            memcpy_s(m_solid_array[2], raw_size, &r2.x, raw_size);
        }

        Mat(Type m00, Type m01, Type m02,
            Type m10, Type m11, Type m12,
            Type m20, Type m21, Type m22) 
        {
            const auto arr_size = sizeof(m_solid_array);
            memcpy_s(m_solid_array, arr_size, &m00, arr_size);
        }

        Mat(const Mat& mat) {
            const auto arr_size = sizeof(m_solid_array);
            memcpy_s(m_solid_array, arr_size, mat.m_solid_array, arr_size);
        }

        Mat(Mat&& mat) {
            const auto arr_size = sizeof(m_solid_array);
            memmove_s(m_solid_array, arr_size, mat.m_solid_array, arr_size);
        }

        void SetIdentity() noexcept {
            memset(m_solid_array, 0, sizeof(m_solid_array));
            for (std::size_t i = 0; i < dimension; ++i) {
                m_solid_array[i][i] = 1.0f;
            }
        }

        float Determinant() const noexcept {
            return m_solid_array[0][0] * (m_solid_array[1][1] * m_solid_array[2][2] - m_solid_array[1][2] * m_solid_array[2][1]) +
                m_solid_array[0][1] * (m_solid_array[1][2] * m_solid_array[2][0] - m_solid_array[1][0] * m_solid_array[2][2]) +
                m_solid_array[0][2] * (m_solid_array[1][0] * m_solid_array[2][1] - m_solid_array[1][1] * m_solid_array[2][0]);
        }

        bool operator==(const Mat &m) const noexcept {
            return memcmp(m_solid_array, m.m_solid_array, sizeof(type) * dimension * dimension) == 0;
        }

        bool operator!=(const Mat &m) const noexcept {
            return !(*this == m);
        }

        Mat& operator=(const Mat &m) noexcept {
            for (std::size_t i = 0; i < dimension; ++i) {
                m_vector_array[i] = m.m_vector_array[i];
            }
        }

        Mat operator*(const Mat &m) const noexcept {
            Mat temp;

            for (std::size_t i = 0; i < dimension; ++i) {
                for (std::size_t j = 0; j < dimension; ++j) {
                    temp[i][j] = 0.0f;
                    for (std::size_t k = 0; k < dimension; ++k) {
                        temp[i][j] += m_solid_array[i][k] * m.m_solid_array[k][j];
                    }
                }
            }

            return temp;
        }

        Mat operator*(float value) const noexcept {
            Mat mat;
            for (std::size_t i = 0; i < dimension; ++i) {
                mat[i][0] = m_solid_array[i][0] * value;
                mat[i][1] = m_solid_array[i][1] * value;
                mat[i][2] = m_solid_array[i][2] * value;
            }

            return mat;
        }

        Mat& operator*=(float value) noexcept {
            for (std::size_t i = 0; i < dimension; ++i) {
                m_solid_array[i][0] *= value;
                m_solid_array[i][1] *= value;
                m_solid_array[i][2] *= value;
            }

            return *this;
        }

        Mat operator/(float value) const noexcept {
            Mat mat;
            for (std::size_t i = 0; i < dimension; ++i) {
                mat[i][0] = m_solid_array[i][0] / value;
                mat[i][1] = m_solid_array[i][1] / value;
                mat[i][2] = m_solid_array[i][2] / value;
            }

            return mat;
        }

        Mat& operator/=(float value) noexcept {
            for (std::size_t i = 0; i < dimension; ++i) {
                m_solid_array[i][0] /= value;
                m_solid_array[i][1] /= value;
                m_solid_array[i][2] /= value;
            }

            return *this;
        }

        operator Mat<Type, 4>() const noexcept {
            return Mat<Type, 4>(m_solid_array[0][0], m_solid_array[0][1], m_solid_array[0][2], Type(0),
                                m_solid_array[1][0], m_solid_array[1][1], m_solid_array[1][2], Type(0),
                                m_solid_array[2][0], m_solid_array[2][1], m_solid_array[2][2], Type(0),
                                            Type(0),             Type(0),             Type(0), Type(1));
        }

        Vector<Type, dimension>& operator[](std::size_t idx) {
            assert(idx < dimension && __FUNCTION__ ": invalid idx value");
            return m_vector_array[idx];
        }

        const Vector<Type, dimension>& operator[](std::size_t idx) const {
            assert(idx < dimension && __FUNCTION__ ": invalid idx value");
            return m_vector_array[idx];
        }

    protected:
        union {
            Type m_solid_array[dimension][dimension];
            Vector<Type, dimension> m_vector_array[dimension];
        };
    };

    
    template <typename Type, std::size_t Dimension>
    auto operator*(float value, const Mat<Type, Dimension>& mat) noexcept -> decltype(mat * value) {
        return mat * value;
    }

    using mat3f = Mat<float, 3>;
    using mat4f = Mat<float, 4>;
}