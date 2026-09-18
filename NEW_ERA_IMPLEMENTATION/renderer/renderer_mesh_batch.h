#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace new_era::renderer {

struct SourceTriangle {
    uint8_t polygon = 0;
    int16_t vertex_index[4]{};
    int16_t normal_index[4]{};
    int16_t texcoord_index[4]{};
};

struct MeshVertex {
    uint16_t vertex_index = 0;
    uint16_t normal_index = 0;
    uint16_t texcoord_index = 0;
};

struct MeshBatch {
    std::vector<MeshVertex> vertices;
    std::vector<uint32_t> indices;
};

inline bool BuildTriangleBatch(const SourceTriangle* triangles, uint16_t triangle_count,
                               MeshBatch& out, std::string& error) {
    out.vertices.clear();
    out.indices.clear();
    if (triangle_count != 0 && triangles == nullptr) {
        error = "triangles pointer is null";
        return false;
    }
    for (uint16_t i=0; i<triangle_count; ++i) {
        const SourceTriangle& t = triangles[i];
        if (t.polygon != 3) {
            error = "non-triangle polygon requires a separate compatibility path";
            out.vertices.clear(); out.indices.clear();
            return false;
        }
        const uint32_t base = static_cast<uint32_t>(out.vertices.size());
        for (int k=0; k<3; ++k) {
            if (t.vertex_index[k] < 0 || t.normal_index[k] < 0 || t.texcoord_index[k] < 0) {
                error = "negative mesh index";
                out.vertices.clear(); out.indices.clear();
                return false;
            }
            out.vertices.push_back(MeshVertex{
                static_cast<uint16_t>(t.vertex_index[k]),
                static_cast<uint16_t>(t.normal_index[k]),
                static_cast<uint16_t>(t.texcoord_index[k])});
        }
        out.indices.push_back(base); out.indices.push_back(base+1); out.indices.push_back(base+2);
    }
    return true;
}

}