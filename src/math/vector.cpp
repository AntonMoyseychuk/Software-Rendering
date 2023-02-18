#include "vector.hpp"
#include "mat3x3.hpp"

#include <stdexcept>


namespace math
{
    Vector3::Vector3()
        : Vector3(0.0f) {}

    Vector3::Vector3(float x, float y, float z)
        : m_x(x), m_y(y), m_z(z) {}

    Vector3::Vector3(float value)
        : m_x(value), m_y(value), m_z(value) {}

    float Vector3::Length() const noexcept {
        return sqrtf(m_x * m_x + m_y * m_y + m_z * m_z);
    }

    Vector3 Vector3::Normalize() const noexcept {
        auto length = this->Length();

        if (length <= 0.000001f) {
            return {};
        }

        return Vector3(m_x / length, m_y / length, m_z / length);
    }

    Vector3 Vector3::operator+(const Vector3 &vec) const noexcept {
        return Vector3(m_x + vec.m_x, m_y + vec.m_y, m_z + vec.m_z);
    }

    Vector3 &Vector3::operator+=(const Vector3 &vec) noexcept {
        m_x += vec.m_x;
        m_y += vec.m_y;
        m_z += vec.m_z;
        return *this;
    }

    Vector3 Vector3::operator-(const Vector3 &vec) const noexcept {
        return Vector3(m_x - vec.m_x, m_y - vec.m_y, m_z - vec.m_z);
    }

    Vector3 &Vector3::operator-=(const Vector3 &vec) noexcept {
        m_x -= vec.m_x;
        m_y -= vec.m_y;
        m_z -= vec.m_z;
        return *this;
    }

    Vector3 Vector3::operator*(const Mat3x3 &mat) const noexcept {
        return Vector3(
            m_x * mat[0][0] + m_y * mat[1][0] + m_z * mat[2][0],
            m_x * mat[0][1] + m_y * mat[1][1] + m_z * mat[2][1],
            m_x * mat[0][2] + m_y * mat[1][2] + m_z * mat[2][2]
        );
    }

    Vector3 Vector3::operator*(float value) const noexcept {
        return Vector3(m_x * value, m_y * value, m_z * value);
    }

    Vector3 &Vector3::operator*=(float value) noexcept {
        m_x *= value;
        m_y *= value;
        m_z *= value;
        return *this;
    }

    Vector3 Vector3::operator-() const noexcept {
        return Vector3(-m_x, -m_y, -m_z);
    }

    float &Vector3::X() noexcept {
        return m_x;
    }

    const float &Vector3::X() const noexcept {
        return m_x;
    }

    float &Vector3::Y() noexcept {
        return m_y;
    }

    const float &Vector3::Y() const noexcept {
        return m_y;
    }

    float &Vector3::Z() noexcept {
        return m_z;
    }

    const float &Vector3::Z() const noexcept {
        return m_z;
    }

    float &Vector3::operator[](std::size_t idx) {
        if (idx > 2) {
            throw std::out_of_range("idx must be 0 <= idx <= 2");
        }

        return *(&m_x + idx);
    }

    const float &Vector3::operator[](std::size_t idx) const {
        if (idx > 2) {
            throw std::out_of_range("idx must be 0 <= idx <= 2");
        }

        return *(&m_x + idx);
    }

    const Vector3 Vector3::RIGHT(1.0f, 0.0f, 0.0f);
    const Vector3 Vector3::DOWN(0.0f, -1.0f, 0.0f);
    const Vector3 Vector3::LEFT(-1.0f, 0.0f, 0.0f);
    const Vector3 Vector3::UP(0.0f, 1.0f, 0.0f);
    const Vector3 Vector3::FORWARD(0.0f, 0.0f, 1.0f);
    const Vector3 Vector3::BACKWARD(0.0f, 0.0f, -1.0f);
}