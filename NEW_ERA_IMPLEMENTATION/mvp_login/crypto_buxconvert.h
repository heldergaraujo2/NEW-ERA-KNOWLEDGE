// NEW-ERA — MVP LOGIN — crypto_buxconvert.h
// XOR cíclico de 3 bytes, chave {0xFC,0xCF,0xAB} — PROVADO (Ledger §41):
//   cliente  WSclient.cpp:209-215      (static bBuxCode)
//   tool     Encoder/Util.cpp:4-12     (static bBuxCode, idêntica)
//   servidor GS Util.cpp:101-109       (PacketArgumentDecrypt, XorTable local idêntica)
// Spec: NEW_ERA_PROTOCOL_MVP_LOGIN_SPEC.md §3.1 — aplicado a ID/senha ANTES do stream.
#pragma once
#include <stdint.h>
#include <stddef.h>

namespace newera {
namespace crypto {

inline void BuxConvert(uint8_t* buffer, size_t size) {
    static const uint8_t kBuxCode[3] = { 0xFC, 0xCF, 0xAB };
    for (size_t i = 0; i < size; ++i) {
        buffer[i] ^= kBuxCode[i % 3];
    }
}

// Involutivo: criptografar == descriptografar (uso do servidor: PacketArgumentDecrypt).
inline void BuxDecrypt(uint8_t* buffer, size_t size) { BuxConvert(buffer, size); }

} // namespace crypto
} // namespace newera
