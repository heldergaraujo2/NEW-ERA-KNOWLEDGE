#include "../crypto_cpacketmanager_server.h"
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static const uint8_t kEnc1[54] = {
    0x12,0x11,0x36,0x00,0x00,0x00,0xd4,0x53,0x09,0x3f,0x01,0x41,0x5e,0xe2,0xe2,0x68,
    0xd3,0x93,0x2d,0x06,0xdf,0x20,0x5a,0xfc,0x08,0x3f,0x00,0xec,0x5c,0xe2,0xd1,0x37,
    0xd2,0x93,0xf0,0x92,0xde,0x20,0x86,0x1a,0x08,0x3f,0xd2,0x76,0x5c,0xe2,0xfa,0x41,
    0xd2,0x93,0x86,0x35,0xde,0x20
};

int main() {
    const std::string path = "/tmp/newera_server_enc1.dat";
    {
        std::ofstream f(path, std::ios::binary);
        assert(f.write(reinterpret_cast<const char*>(kEnc1), sizeof(kEnc1)));
    }

    newera::crypto::ServerPacketManager pm;
    std::string err;
    assert(pm.XorFilter().size() == 32);
    assert(pm.XorFilter()[0] == 0xE7 && pm.XorFilter()[31] == 0x56);

    assert(pm.LoadEncryptionKey(path, &err));
    assert(pm.EncryptionKeysLoaded());
    assert(pm.EncryptionKeys().modulus[0] == 0x1F44F);
    assert(pm.EncryptionKeys().key[0] == 0x5BC1);
    assert(pm.EncryptionKeys().xor_[0] == 0xBD1D);

    // Reconstruct a wire packet with the exact inverse recurrence:
    // wire[n] = plain[n] ^ wire[n-1] ^ m_XorFilter[n%32].
    // ExtractPacket then applies the upstream descending transform and recovers
    // the original plaintext bytes.
    const std::vector<uint8_t> original = {
        0xC1, 0x0A, 0xF3, 0x03, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60};
    std::vector<uint8_t> wire = original;
    const auto& filter = pm.XorFilter();
    for (std::size_t n = 3; n < wire.size(); ++n) {
        wire[n] = static_cast<uint8_t>(
            original[n] ^ wire[n - 1] ^ filter[n % filter.size()]);
    }
    assert(pm.AddData(wire, &err));
    std::vector<uint8_t> transformed;
    assert(pm.ExtractPacket(transformed, &err));
    assert(transformed == original);

    std::remove(path.c_str());
    std::cout << "TS-26 server PacketManager seed + production Enc1 loader: PASS\n";
    return 0;
}
