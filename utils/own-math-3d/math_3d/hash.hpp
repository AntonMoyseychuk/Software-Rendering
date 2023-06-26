#pragma once

#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"

#include "quaternion.hpp"

#include "mat4.hpp"

namespace math::detail {
    inline void hash_combine(size_t &seed, size_t hash) {
		hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hash;
	}
}

namespace std {
    template <typename T>
    struct hash<math::vec2<T>> {
        size_t operator()(const math::vec2<T>& v) const noexcept {
            size_t seed = 0;
            hash<T> hasher;
            math::detail::hash_combine(seed, hasher(v.x));
            math::detail::hash_combine(seed, hasher(v.y));
            return seed;
        }
    };

    template <typename T>
    struct hash<math::vec3<T>> {
        size_t operator()(const math::vec3<T>& v) const noexcept {
            size_t seed = 0;
            hash<T> hasher;
            math::detail::hash_combine(seed, hasher(v.x));
            math::detail::hash_combine(seed, hasher(v.y));
            math::detail::hash_combine(seed, hasher(v.z));
            return seed;
        }
    };

    template <>
    struct hash<math::vec4f> {
        size_t operator()(const math::vec4f& v) const noexcept {
            size_t seed = 0;
            hash<float> hasher;
            math::detail::hash_combine(seed, hasher(v.x));
            math::detail::hash_combine(seed, hasher(v.y));
            math::detail::hash_combine(seed, hasher(v.z));
            math::detail::hash_combine(seed, hasher(v.w));
            return seed;
        }
    };

    template <>
    struct hash<math::quaternion> {
        size_t operator()(const math::quaternion& q) const noexcept {
            size_t seed = 0;
            hash<float> hasher;
            math::detail::hash_combine(seed, hasher(q.x));
            math::detail::hash_combine(seed, hasher(q.y));
            math::detail::hash_combine(seed, hasher(q.z));
            math::detail::hash_combine(seed, hasher(q.w));
            return seed;
        }
    };

    template <>
    struct hash<math::mat4f> {
        size_t operator()(const math::mat4f& mat) const noexcept {
            size_t seed = 0;
            hash<math::vec4f> hasher;
            math::detail::hash_combine(seed, hasher(mat.vec_row0));
            math::detail::hash_combine(seed, hasher(mat.vec_row1));
            math::detail::hash_combine(seed, hasher(mat.vec_row2));
            math::detail::hash_combine(seed, hasher(mat.vec_row3));
            return seed;
        }
    };
}