#pragma once
#include <cstdint>

namespace new_era::renderer {

constexpr uint32_t RENDER_TEXTURE=0x00000002;
constexpr uint32_t RENDER_CHROME=0x00000004;
constexpr uint32_t RENDER_METAL=0x00000008;
constexpr uint32_t RENDER_LIGHTMAP=0x00000010;
constexpr uint32_t RENDER_SHADOWMAP=0x00000020;
constexpr uint32_t RENDER_BRIGHT=0x00000040;
constexpr uint32_t RENDER_DARK=0x00000080;
constexpr uint32_t RENDER_CHROME2=0x00000200;
constexpr uint32_t RENDER_WAVE=0x00000400;
constexpr uint32_t RENDER_CHROME3=0x00000800;
constexpr uint32_t RENDER_CHROME4=0x00001000;
constexpr uint32_t RENDER_NODEPTH=0x00002000;
constexpr uint32_t RENDER_CHROME5=0x00004000;
constexpr uint32_t RENDER_OIL=0x00008000;
constexpr uint32_t RENDER_CHROME6=0x00010000;
constexpr uint32_t RENDER_CHROME7=0x00020000;

enum class MaterialPath : uint8_t { Texture, Chrome, Oil, Bright, Fallback };
struct MaterialDecision { MaterialPath path; bool alpha_blend; bool alpha_test; bool depth_test; bool depth_write; };

inline MaterialDecision EvaluateMaterial(uint32_t flags, float alpha, bool texture_rgba, bool mesh_texture_match=false) {
    const bool chrome = (flags&RENDER_CHROME)!=0 || (flags&RENDER_CHROME2)!=0 || (flags&RENDER_CHROME3)!=0 ||
        (flags&RENDER_CHROME4)!=0 || (flags&RENDER_CHROME6)!=0 || (flags&RENDER_CHROME7)!=0;
    const bool alpha = alpha < 0.99f;
    MaterialDecision d{MaterialPath::Fallback,false,false,true,true};
    if ((flags&RENDER_TEXTURE)!=0) {
        d.path=MaterialPath::Texture;
        d.alpha_blend=(flags&RENDER_BRIGHT)!=0 || (flags&RENDER_DARK)!=0 || alpha;
        d.alpha_test=!d.alpha_blend && (texture_rgba || alpha);
    } else if ((flags&RENDER_BRIGHT)!=0) {
        d.path=MaterialPath::Bright; d.alpha_blend=true; d.depth_write=false;
    } else if (chrome) {
        d.path=MaterialPath::Chrome; d.alpha_blend=alpha;
    } else if ((flags&RENDER_OIL)!=0) {
        d.path=MaterialPath::Oil; d.alpha_blend=alpha;
    } else if (mesh_texture_match) {
        d.path=MaterialPath::Texture; d.alpha_blend=true;
    }
    if ((flags&RENDER_DARK)!=0 && d.path!=MaterialPath::Texture) d.alpha_blend=true;
    if ((flags&RENDER_LIGHTMAP)!=0) d.path=MaterialPath::Texture;
    if ((flags&RENDER_NODEPTH)!=0) d.depth_test=false;
    return d;
}
}
