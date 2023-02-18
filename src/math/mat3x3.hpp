#ifndef MAT3X3_H
#define MAT3X3_H

#include "declarations.hpp"
#include "vector.hpp"


namespace math {
    class Mat3x3 {
    public:
        Mat3x3();
        Mat3x3(const Vector3 &r0, const Vector3 &r1, const Vector3 &r2);

        void SetIdentity() noexcept;
        float Determinant() const noexcept;
        // Mat3x3 Transpose() const noexcept;
        // Mat3x3 Invert() const;

        Mat3x3 operator*(const Mat3x3 &m) const noexcept;
        Mat3x3 operator*(float value) const noexcept;

        float *operator[](std::size_t idx);
        const float *operator[](std::size_t idx) const;

    public:
        static std::size_t Order() noexcept;

    private:
        static const std::size_t RAW_COUNT = 3, COLUMN_COUNT = RAW_COUNT;

        typedef union _Mat3x3 {
            _Mat3x3() : vectors() {}
            ~_Mat3x3() = default;

            Vector3 vectors[RAW_COUNT];
            float solid_arrays[RAW_COUNT][COLUMN_COUNT];
        } Buffer;

        Buffer m_matrix;
    };
}

#endif