#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <cstring>
#include "crypto_cpacketmanager_server.h"

namespace newera::crypto {

struct C4DecodedPacket {
    std::vector<uint8_t> classic_c2;
};

inline bool UndoServerStreamXor(std::vector<uint8_t>& packet, std::string* err = nullptr) {
    if (packet.size() < 3 || packet[0] != 0xC2) {
        if (err) *err = "inner packet is not C2";
        return false;
    }
    const std::size_t declared = (static_cast<std::size_t>(packet[1]) << 8) | packet[2];
    if (declared != packet.size()) {
        if (err) *err = "inner C2 size mismatch";
        return false;
    }
    static const uint8_t filter[32] = {
        0xE7,0x6D,0x3A,0x89,0xBC,0xB2,0x9F,0x73,
        0x23,0xA8,0xFE,0xB6,0x49,0x5D,0x39,0x5D,
        0x8A,0xCB,0x63,0x8D,0xEA,0x7D,0x2B,0x5F,
        0xC3,0xB1,0xE9,0x83,0x29,0x51,0xE8,0x56
    };
    for (std::size_t n = packet.size() - 1; n > 2; --n)
        packet[n] = static_cast<uint8_t>(packet[n] ^ packet[n - 1] ^ filter[n % 32]);
    return true;
}

inline bool DecodeC4ServerFrame(const std::vector<uint8_t>& wire,
                                PacketCryptoSM& decryptor,
                                C4DecodedPacket& out,
                                std::string* err = nullptr) {
    if (wire.size() < 14 || wire[0] != 0xC4) {
        if (err) *err = "expected C4 frame";
        return false;
    }
    const std::size_t declared = (static_cast<std::size_t>(wire[1]) << 8) | wire[2];
    if (declared != wire.size()) {
        if (err) *err = "outer C4 size mismatch";
        return false;
    }
    const std::size_t cipher_size = wire.size() - 3;
    if (cipher_size == 0 || (cipher_size % 11) != 0) {
        if (err) *err = "C4 ciphertext is not 11-byte block aligned";
        return false;
    }
    std::vector<uint8_t> recovered((cipher_size / 11) * 8, 0);
    const int n = decryptor.Decrypt(recovered.data(), wire.data() + 3, static_cast<int>(cipher_size));
    if (n < 3) {
        if (err) *err = "SimpleModulus C4 decrypt failed";
        return false;
    }
    recovered.resize(static_cast<std::size_t>(n));
    if (!UndoServerStreamXor(recovered, err)) return false;
    out.classic_c2 = std::move(recovered);
    return true;
}

} // namespace newera::crypto
