#include "../renderer_material.h"
#include <cassert>
#include <iostream>
using namespace new_era::renderer;
int main(){
 auto a=EvaluateMaterial(RENDER_TEXTURE,1.0f,false); assert(a.path==MaterialPath::Texture&&!a.alpha_blend&&!a.alpha_test&&a.depth_test&&a.depth_write);
 auto b=EvaluateMaterial(RENDER_TEXTURE|RENDER_BRIGHT,1.0f,false); assert(b.alpha_blend);
 auto c=EvaluateMaterial(RENDER_BRIGHT,1.0f,false); assert(c.path==MaterialPath::Bright&&c.alpha_blend&&!c.depth_write);
 auto d=EvaluateMaterial(RENDER_TEXTURE,0.5f,true); assert(d.alpha_blend);
 auto e=EvaluateMaterial(RENDER_TEXTURE|RENDER_NODEPTH,1.0f,false); assert(!e.depth_test);
 auto f=EvaluateMaterial(RENDER_CHROME,1.0f,false); assert(f.path==MaterialPath::Chrome&&!f.alpha_blend);
 auto g=EvaluateMaterial(RENDER_OIL,0.5f,false); assert(g.path==MaterialPath::Oil&&g.alpha_blend);
 std::cout<<"0D-2 renderer material evaluator: PASS\n";
}