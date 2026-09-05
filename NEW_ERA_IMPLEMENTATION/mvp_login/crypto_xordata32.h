// NEW-ERA — MVP LOGIN — crypto_xordata32.h
// XOR ENCADEADO de stream: buf[i] ^= buf[i-1] ^ Filter[i%32] — PROVADO (Ledger §43):
//   cliente  StreamPacketEngine.h:66-80 (XorData; AddData bXor=TRUE :82-94 aplica por campo)
//   servidor PacketManager.cpp:85-116 (m_XorFilter — 32/32 bytes idênticos, conferido em 1.0-B)
// Spec: NEW_ERA_PROTOCOL_MVP_LOGIN_SPEC.md §3.2.
#pragma once
#include <stdint.h>
#include <stddef.h>

namespace newera {
namespace crypto {

// Tabela canônica extraída da EVIDÊNCIA (1.0-B, extração programática — não digitada):
//   primária: NEW_ERA_EVIDENCE/0E1L__10__StreamPacketEngine__path_Source_Client_source__580472e.h
//             (byXorFilter, XorData :68-74) · SHA256 797d26d9678852e2f586df967a090b451d1d5b07d97c6fba7ddbe21d39c2e684
//   cross-check 32/32 OK: NEW_ERA_EVIDENCE/0C10A__PacketManager…cpp (m_XorFilter :85-116)
//             · SHA256 3e82e5d21ecfb84367a4139293aedb0b3146a2f7e221ffe03d45c2499006cfe4
static constexpr uint8_t kXorFilter32[32] = {
    0xE7, 0x6D, 0x3A, 0x89, 0xBC, 0xB2, 0x9F, 0x73,
    0x23, 0xA8, 0xFE, 0xB6, 0x49, 0x5D, 0x39, 0x5D,
    0x8A, 0xCB, 0x63, 0x8D, 0xEA, 0x7D, 0x2B, 0x5F,
    0xC3, 0xB1, 0xE9, 0x83, 0x29, 0x51, 0xE8, 0x56
};

// Fiel a StreamPacketEngine::XorData(iStart,iEnd) (:76-79):
//   for (i = start; i != end; i++) buf[i] ^= buf[i-1] ^ filter[i % 32];
// FRONTeira (evidência Init :29-46 / AddData :82-94): em pacotes C1 os 3 bytes do
// header ([0]=C1, [1]=size, [2]=head) são gravados com bXor=FALSE — NÃO são XORed.
// O PRIMEIRO byte XORed é [3] (subcode), cujo predecessor é o head [2].
// Campos sucessivos encadeiam naturalmente (aplicação por campo ≡ aplicação
// contígua, pois cada campo usa o byte já transformado do anterior).
inline void XorData32(uint8_t* buffer, size_t start, size_t end) {
    for (size_t i = start; i < end; ++i) {
        buffer[i] ^= static_cast<uint8_t>(buffer[i - 1] ^ kXorFilter32[i % 32]);
    }
}

// Conveniência para o payload do login C1 (spec §4.2): XOR em [3..49).
inline void ApplyStreamXorLikeEvidence(uint8_t* buffer49) {
    XorData32(buffer49, 3, 49);
}

} // namespace crypto
} // namespace newera
