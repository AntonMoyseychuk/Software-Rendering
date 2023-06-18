#include "not_crazy_mesh.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"

#define LINE_STRING(line) #line
#define MESH_LOADER_ERROR(file, function, line) "[MESH LOAD ERROR]\nfile: " file "\nfunction: " function "\nline: " LINE_STRING(line)

namespace rasterization {
    std::unordered_map<std::string, NotCrazyMesh::Buffer> NotCrazyMesh::already_loaded_meshes;

    NotCrazyMesh::NotCrazyMesh(const char *filename) {
        if (!Load(filename)) {
            throw std::runtime_error(MESH_LOADER_ERROR(__FILE__, __FUNCTION__, __LINE__) + std::string("\nerror: ") + m_error_msg + "\nwarning: " + m_warn_message);
        }
    }

    const NotCrazyMesh::Buffer* NotCrazyMesh::Load(const char *filename) noexcept {
        if (already_loaded_meshes.count(filename) == 1) {
            return &already_loaded_meshes[filename];
        }

        tinyobj::attrib_t attribute;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;

        if (tinyobj::LoadObj(&attribute, &shapes, &materials, &m_warn_message, &m_error_msg, filename) == false) {
            return nullptr;
        }

        Buffer buffer;
        
        // std::unordered_map<Vertex, size_t> cached_vertex_indexes;
        for (;;) {
            for (;;) {
                //...
            }
        }

        already_loaded_meshes[filename] = buffer;
        return &already_loaded_meshes[filename];
    }

    const NotCrazyMesh::Buffer* NotCrazyMesh::GetBuffer() const noexcept {
        return m_buffer;
    }
}