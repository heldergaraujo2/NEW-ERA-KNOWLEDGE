#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "renderer_mesh_batch.h"

namespace new_era::renderer {
struct SourceQuad {
    uint8_t polygon=0;
    int16_t vertex_index[4]{};
    int16_t normal_index[4]{};
    int16_t texcoord_index[4]{};
};
inline bool BuildQuadCompatibilityBatch(const SourceQuad* quads,uint16_t count,MeshBatch& out,std::string& error){
    out.vertices.clear(); out.indices.clear();
    if(count && !quads){error="quads pointer is null";return false;}
    for(uint16_t i=0;i<count;++i){
        const auto&t=quads[i];
        if(t.polygon!=4){error="source is not a quad";out.vertices.clear();out.indices.clear();return false;}
        for(int k=0;k<4;++k) if(t.vertex_index[k]<0||t.normal_index[k]<0||t.texcoord_index[k]<0){error="negative quad index";out.vertices.clear();out.indices.clear();return false;}
        const uint32_t base=static_cast<uint32_t>(out.vertices.size());
        for(int k=0;k<4;++k) out.vertices.push_back(MeshVertex{static_cast<uint16_t>(t.vertex_index[k]),static_cast<uint16_t>(t.normal_index[k]),static_cast<uint16_t>(t.texcoord_index[k])});
        out.indices.insert(out.indices.end(),{base,base+1,base+2,base,base+2,base+3});
    } return true;
}
}