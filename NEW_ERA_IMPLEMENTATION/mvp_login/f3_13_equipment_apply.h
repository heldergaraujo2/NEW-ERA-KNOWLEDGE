#pragma once

#include <array>
#include <cstdint>
#include <string>

namespace newera::equipment_apply {

// Semantic application reconstructed from ChangeCharacterExt in the pinned
// upstream client. This is a pure state transform: it does not create/render
// MU objects or invoke undocumented engine side effects.
struct ModelBases {
    int model_sword{};
    int model_wing{};
    int model_helper{};
    int model_helm{};
    int model_armor{};
    int model_pants{};
    int model_gloves{};
    int model_boots{};
    int model_body_helm{};
    int model_body_armor{};
    int model_body_pants{};
    int model_body_gloves{};
    int model_body_boots{};
};

struct BodyPart {
    int type = -1;
    int level = 0;
    int option1 = 0;
    int ext_option = 0;
};

struct State {
    int weapon0 = -1;
    int weapon1 = -1;
    int wing = -1;
    int helper = -1;
    int weapon0_level = 0;
    int weapon1_level = 0;
    int wing_level = 0;
    int helper_level = 0;
    int helper_option1 = 0;
    int extend_state = 0;
    std::array<BodyPart, 5> body{};
};

inline int LevelConvert(uint8_t level) {
    static constexpr int table[8] = {0, 3, 5, 7, 9, 11, 13, 15};
    return table[level & 7];
}

inline bool Apply(const std::array<uint8_t, 18>& e,
                  const ModelBases& bases,
                  int skin_model_index,
                  State& out,
                  std::string& error) {
    const std::array<int, 5> body_fallback = {
        bases.model_body_helm,
        bases.model_body_armor,
        bases.model_body_pants,
        bases.model_body_gloves,
        bases.model_body_boots};
    const std::array<int, 5> body_base = {
        bases.model_helm,
        bases.model_armor,
        bases.model_pants,
        bases.model_gloves,
        bases.model_boots};

    const int weapon0_type = ((e[11] & 0xF0) << 4) | e[0];
    const int weapon1_type = ((e[12] & 0xF0) << 4) | e[1];
    out.weapon0 = (weapon0_type == 0x0FFF) ? -1 : bases.model_sword + weapon0_type;
    out.weapon1 = (weapon1_type == 0x0FFF) ? -1 : bases.model_sword + weapon1_type;

    int type = (e[4] >> 2) & 3;
    out.wing = bases.model_wing + ((type == 3) ? (e[8] & 7) : type);
    type = (e[15] >> 2) & 7;
    if (type > 0) {
        out.wing = bases.model_wing + ((type == 6) ? 43 : 35 + type);
    }
    type = e[16] >> 5;
    if (type > 0 && type <= 5) {
        out.wing = bases.model_wing + 129 + type;
    }

    const uint8_t helper_type = e[4] & 3;
    if (helper_type == 3) {
        out.helper = (e[9] & 1) ? bases.model_helper + 3 : -1;
    } else {
        int helper_model = -1;
        switch (e[15] & 0xE0) {
        case 32: helper_model = 64; break;
        case 64: helper_model = 65; break;
        case 128: helper_model = 67; break;
        case 224: helper_model = 80; break;
        case 160: helper_model = 106; break;
        case 96: helper_model = 123; break;
        default: break;
        }
        out.helper = bases.model_helper + ((helper_model >= 0) ? helper_model : helper_type);
    }
    if (e[11] & 1) {
        out.helper = bases.model_helper + 4;
    }
    if (e[11] & 4) {
        int fenrir_type = e[15] & 3;
        if (e[16] & 1) {
            fenrir_type = 4;
        }
        out.helper = bases.model_helper + 37;
        out.helper_option1 = fenrir_type;
    }

    const std::array<int, 5> ext_type = {
        (e[2] >> 4) + ((e[8] >> 7) & 1) * 16 + (e[12] & 15) * 32,
        (e[2] & 15) + ((e[8] >> 6) & 1) * 16 + ((e[13] >> 4) & 15) * 32,
        (e[3] >> 4) + ((e[8] >> 5) & 1) * 16 + (e[13] & 15) * 32,
        (e[3] & 15) + ((e[8] >> 4) & 1) * 16 + ((e[14] >> 4) & 15) * 32,
        (e[4] >> 4) + ((e[8] >> 3) & 1) * 16 + (e[14] & 15) * 32};

    for (int i = 0; i < 5; ++i) {
        out.body[i].type = (ext_type[i] == 0x1FF)
            ? body_fallback[i] + skin_model_index
            : body_base[i] + ext_type[i];
    }

    const int level = (static_cast<int>(e[5]) << 16) |
                      (static_cast<int>(e[6]) << 8) | e[7];
    out.weapon0_level = LevelConvert(static_cast<uint8_t>(level));
    out.weapon1_level = LevelConvert(static_cast<uint8_t>(level >> 3));
    out.wing_level = 0;
    out.helper_level = 0;
    for (int i = 0; i < 5; ++i) {
        out.body[i].level = LevelConvert(static_cast<uint8_t>(level >> (6 + 3 * i)));
    }

    out.body[0].option1 = (e[9] & 128) / 128;
    out.body[1].option1 = (e[9] & 64) / 64;
    out.body[2].option1 = (e[9] & 32) / 32;
    out.body[3].option1 = (e[9] & 16) / 16;
    out.body[4].option1 = (e[9] & 8) / 8;

    out.body[0].ext_option = (e[10] & 128) / 128;
    out.body[1].ext_option = (e[10] & 64) / 64;
    out.body[2].ext_option = (e[10] & 32) / 32;
    out.body[3].ext_option = (e[10] & 16) / 16;
    out.body[4].ext_option = (e[10] & 8) / 8;
    out.extend_state = e[10] & 1;

    error.clear();
    return true;
}

} // namespace newera::equipment_apply
