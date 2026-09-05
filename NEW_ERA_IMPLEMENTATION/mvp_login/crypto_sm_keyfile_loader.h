// NEW-ERA — MVP LOGIN — crypto_sm_keyfile_loader.h
// Loader de chaves SimpleModulus (formato ENCDEC) — SEM invenção:
// EVIDÊNCIA primária: NEW_ERA_EVIDENCE/0C10A__PacketManager…cpp @580472e (sha 3e82e5d2…)
//   LoadEncryptionKey :121 → LoadKey(name, 4370, 0)   (:129)
//   LoadDecryptionKey :134 → LoadKey(name, 4370, 1)   (:142)
//   LoadKey :147-:212: lê ENCDEC_HEADER{WORD header; DWORD size} (:155-:166),
//   valida header==4370 && size==sizeof(HDR)+sizeof(DATA)==54 (:168-:172),
//   lê 3 tabelas DWORD[4] LE (Modulus :187, Key :194, Xor :201) e decompõe
//   cada dword com m_SaveLoadXor[n] (:191/:198/:205), tabela fixa do ctor :37-:40:
//     {0x3F08A79B, 0xE25CC287, 0x93D27AB9, 0x20DEA7BF}
// Validado em 1.0-D contra Enc1/Dec2/Enc2/Dec1 reais (54 B, header 4370) — ver keys/KEYS_MANIFEST.md.
#pragma once
#include <stdint.h>
#include <stddef.h>
#include <string>
#include <cstdio>

namespace newera {
namespace crypto {

struct EncDecKeys {                 // = ENCDEC_DATA (PacketManager.h:25-30)
    uint32_t modulus[4];
    uint32_t key[4];
    uint32_t xor_[4];
};

// Lê um arquivo Enc/Dec .dat e decompõe as chaves (fiel a LoadKey :147-:212).
inline bool LoadEncDecKeysFromFile(const std::string& path, EncDecKeys& out, std::string* err) {
    FILE* f = std::fopen(path.c_str(), "rb");
    if (!f) { if (err) *err = "nao abriu: " + path; return false; }

    uint8_t buf[54];
    if (std::fread(buf, 1, sizeof(buf), f) != sizeof(buf)) {
        std::fclose(f); if (err) *err = "arquivo < 54 bytes: " + path; return false;
    }
    std::fclose(f);

    // Header: uint16 LE header; uint32 LE size (ENCDEC_HEADER, PacketManager.h:17-23)
    uint16_t header = static_cast<uint16_t>(buf[0] | (buf[1] << 8));
    uint32_t size = static_cast<uint32_t>(buf[2]) | (static_cast<uint32_t>(buf[3]) << 8)
                  | (static_cast<uint32_t>(buf[4]) << 16) | (static_cast<uint32_t>(buf[5]) << 24);
    if (header != 4370 || size != 54) {                   // evidência :129/:142/:168
        if (err) *err = "header/size invalidos (esperado 4370/54)"; return false;
    }

    static const uint32_t kSaveLoadXor[4] = {             // ctor :37-:40
        0x3F08A79Bu, 0xE25CC287u, 0x93D27AB9u, 0x20DEA7BFu
    };
    uint32_t* dst[3] = { out.modulus, out.key, out.xor_ }; // ordem :187/:194/:201
    for (int t = 0; t < 3; ++t) {
        for (int n = 0; n < 4; ++n) {
            const uint8_t* p = buf + 6 + (t * 16) + (n * 4);
            uint32_t v = static_cast<uint32_t>(p[0]) | (static_cast<uint32_t>(p[1]) << 8)
                       | (static_cast<uint32_t>(p[2]) << 16) | (static_cast<uint32_t>(p[3]) << 24);
            dst[t][n] = v ^ kSaveLoadXor[n];              // :191/:198/:205
        }
    }
    return true;
}

} // namespace crypto
} // namespace newera
