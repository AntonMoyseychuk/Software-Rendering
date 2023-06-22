#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include "math_3d/vec3.hpp"
#include "math_3d/vec2.hpp"
#include "math_3d/hash.hpp"

namespace rasterization {
    class Mesh {
    public:
        struct Vertex {
            Vertex() noexcept = default;
            Vertex(const math::vec3f& p, const math::vec3f& n, const math::vec2f& t) noexcept : position(p), normal(n), texcoord(t) {}

            bool operator==(const Vertex& v) const noexcept { return position == v.position && normal == v.normal && texcoord == v.texcoord; }
            bool operator!=(const Vertex& v) const noexcept { return !this->operator==(v); }

            math::vec3f position;
            math::vec3f normal;
            math::vec2f texcoord;
        };
    
        struct Content {
            std::vector<Vertex> vertexes;
            std::vector<size_t> indexes;
        };

    public:
        Mesh() noexcept = default;
        Mesh(const char* filename);

        const Content* Load(const char* filename) noexcept;

        const Content* GetContent() const noexcept;

    private:
        static std::unordered_map<std::string, Content> already_loaded_meshes;

    private:
        Content* m_content = nullptr;
        std::string m_error_msg, m_warn_message;
    };
}