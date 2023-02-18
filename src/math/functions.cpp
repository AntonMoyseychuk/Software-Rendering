#include "functions.hpp"

#include "vector.hpp"
#include "mat3x3.hpp"


namespace math {
    // float Dot(const Vector3 &a, const Vector3 &b) {
    //     return a.X() * b.X() + a.Y() * b.Y() + a.Z() * b.Z();
    // }

    // Vector3 Cross(const Vector3 &a, const Vector3 &b) {
    //     return Vector3(
    //         a.Y() * b.Z() - a.Z() * b.Y(),
    //         a.Z() * b.X() - a.X() * b.Z(),
    //         a.X() * b.Y() - a.Y() * b.X()
    //     );
    // }

    float Dot(const Vector3 &a, const Vector3 &b) noexcept {
        return a.X() * b.X() + a.Y() * b.Y() + a.Z() * b.Z();
    }

    Vector3 Cross(const Vector3 &a, const Vector3 &b) noexcept {
        return Vector3(
            a.Y() * b.Z() - a.Z() * b.Y(),
            a.Z() * b.X() - a.X() * b.Z(),
            a.X() * b.Y() - a.Y() * b.X()
        );
    }

    float Clamp(float left_border, float right_border, float value) noexcept {
        return value < left_border ? left_border : value > right_border ? right_border : value;
    }
}