#include "../renderer/renderer_mesh_batch.h"
#include <cassert>
#include <iostream>
using namespace new_era::renderer;
int main(){
 SourceTriangle t[2]{};
 t[0].polygon=3; t[0].vertex_index[0]=0; t[0].vertex_index[1]=2; t[0].vertex_index[2]=4;
 t[0].normal_index[0]=1; t[0].normal_index[1]=3; t[0].normal_index[2]=5;
 t[0].texcoord_index[0]=6; t[0].texcoord_index[1]=8; t[0].texcoord_index[2]=10;
 t[1].polygon=3; t[1].vertex_index[0]=4; t[1].vertex_index[1]=7; t[1].vertex_index[2]=9;
 t[1].normal_index[0]=5; t[1].normal_index[1]=8; t[1].normal_index[2]=11;
 t[1].texcoord_index[0]=10; t[1].texcoord_index[1]=12; t[1].texcoord_index[2]=14;
 MeshBatch b; std::string e; assert(BuildTriangleBatch(t,2,b,e));
 assert(b.vertices.size()==6 && b.indices.size()==6);
 assert(b.indices[0]==0 && b.indices[5]==5);
 assert(b.vertices[4].vertex_index==7 && b.vertices[4].normal_index==8 && b.vertices[4].texcoord_index==12);
 SourceTriangle q{}; q.polygon=4; assert(!BuildTriangleBatch(&q,1,b,e));
 assert(b.vertices.empty() && b.indices.empty());
 std::cout<<"0D-3 renderer triangle batch contract: PASS\n";
}