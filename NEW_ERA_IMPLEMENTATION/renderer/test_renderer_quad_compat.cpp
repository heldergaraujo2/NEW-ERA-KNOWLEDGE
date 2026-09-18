#include "../renderer/renderer_quad_compat.h"
#include <cassert>
#include <iostream>
using namespace new_era::renderer;
int main(){SourceQuad q{};q.polygon=4;q.vertex_index[0]=0;q.vertex_index[1]=1;q.vertex_index[2]=2;q.vertex_index[3]=3;q.normal_index[0]=4;q.normal_index[1]=5;q.normal_index[2]=6;q.normal_index[3]=7;q.texcoord_index[0]=8;q.texcoord_index[1]=9;q.texcoord_index[2]=10;q.texcoord_index[3]=11;MeshBatch b;std::string e;assert(BuildQuadCompatibilityBatch(&q,1,b,e));assert(b.vertices.size()==4&&b.indices.size()==6);assert(b.indices[0]==0&&b.indices[1]==1&&b.indices[2]==2&&b.indices[3]==0&&b.indices[4]==2&&b.indices[5]==3);assert(b.vertices[3].texcoord_index==11);std::cout<<"0D-4 quad compatibility triangulation: PASS\n";}