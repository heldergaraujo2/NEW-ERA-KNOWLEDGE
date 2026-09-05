// NEW-ERA — MVP LOGIN — packet_headers.h
// Framing C1/C3/C4 conforme NEW_ERA_PROTOCOL_MVP_LOGIN_SPEC.md (sha e583db6b…) §3/§4.
// Proveniência: PBMSG_HEADER (WSclient.h:77-81), PWMSG_HEADER (:83-89),
// PRECEIVE_JOIN_SERVER (:270-279), PBMSG_ENCRYPTED/PWMSG_ENCRYPTED (wsclientinline.h:97-:98).
#pragma once
#include <stdint.h>
#include <stddef.h>
#include <string.h>

namespace newera {
namespace proto {

#pragma pack(push, 1)

struct C1Header {              // pacote de 1 byte de tamanho (<=255)
    uint8_t code;              // [0] 0xC1
    uint8_t size;              // [1] tamanho total
    uint8_t head;              // [2] head code (ex.: 0xF1)
};                             // 3 bytes

struct C2Header {              // pacote grande, 2 bytes de tamanho
    uint8_t code;              // [0] 0xC2
    uint8_t sizeH;             // [1]
    uint8_t sizeL;             // [2]
    uint8_t head;              // [3]
};                             // 4 bytes

struct C3Header {              // envelope criptografado (ciphertext <256 B)
    uint8_t code;              // [0] 0xC3
    uint8_t size;              // [1] tamanho total (ct + 2)
    // ciphertext a partir de [2]
};                             // 2 bytes (header)

struct C4Header {              // envelope criptografado grande
    uint8_t code;              // [0] 0xC4
    uint8_t sizeL;             // [1]
    uint8_t sizeH;             // [2]
    // ciphertext a partir de [3]
};                             // 3 bytes (header)

struct HeaderSubValue {        // = PHEADER_DEFAULT_SUBCODE (WSclient.h:129-134)
    C1Header hdr;              // [0..2]
    uint8_t sub;               // [3]
    uint8_t value;             // [4]
};                             // 5 bytes

struct F1_00_JoinServer {      // = PRECEIVE_JOIN_SERVER (WSclient.h:270-279) — spec §4.1
    C1Header hdr;              // [0..2] C1, 0x0C, 0xF1
    uint8_t sub;               // [3] 0x00
    uint8_t result;            // [4] 0x01 = SUCCESS
    uint8_t numberH;           // [5] HeroKey hi
    uint8_t numberL;           // [6] HeroKey lo
    uint8_t version[5];        // [7..11] Version[i]-(i+1)
};                             // 12 bytes

#pragma pack(pop)

// Larguras fixas do fluxo 0xF1 (spec §4.2; MAX_ID/MAX_PASSWORD=10 [INFERIDO])
static const size_t LOGIN_ID_SIZE   = 10;
static const size_t LOGIN_PASS_SIZE = 10;
static const size_t PROTO_VERSION_SIZE = 5;   // SIZE_PROTOCOLVERSION (WSclient.h:13)
static const size_t PROTO_SERIAL_SIZE = 16;   // SIZE_PROTOCOLSERIAL (WSclient.h:14)
static const size_t LOGIN_REQ_LOGICAL_SIZE =
    4 + LOGIN_ID_SIZE + LOGIN_PASS_SIZE + 4 + PROTO_VERSION_SIZE + PROTO_SERIAL_SIZE; // = 49

// Helpers
inline void set_c1(void* pkt, size_t cap, uint8_t head, uint8_t sub) {
    // Inicializa header C1 com head/sub; size deve ser fixado ao final (spec §3.2).
    auto* h = static_cast<C1Header*>(pkt);
    h->code = 0xC1;
    h->head = head;
    (void)cap; (void)sub; // sub fica em [3] — responsabilidade do montador do campo
}

inline void set_c1_size(void* pkt, uint8_t total_size) {
    static_cast<C1Header*>(pkt)->size = total_size;
}

inline void set_c3(C3Header* h, uint8_t total_size) {
    h->code = 0xC3;
    h->size = total_size;
}

} // namespace proto
} // namespace newera
