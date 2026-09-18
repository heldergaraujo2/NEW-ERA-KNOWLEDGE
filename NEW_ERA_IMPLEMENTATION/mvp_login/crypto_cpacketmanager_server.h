#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <vector>
#include "crypto_sm_keyfile_loader.h"
#include "crypto_cpacketmanager_min.h"

namespace newera::crypto {

// Server-side compatibility layer for the pinned CPacketManager.
// Provenance: wongddd/muonline@580472e, PacketManager.cpp Init/LoadKey/XorData
// and PacketManager.h m_XorFilter[32]. This layer intentionally isolates
// server framing/filter state from the already proven SimpleModulus primitive.
class ServerPacketManager {
public:
    using Keys = PacketCryptoSM::Keys;

    ServerPacketManager() { Init(); }

    void Init() {
        m_xor_filter_ = {
            0xE7,0x6D,0x3A,0x89,0xBC,0xB2,0x9F,0x73,
            0x23,0xA8,0xFE,0xB6,0x49,0x5D,0x39,0x5D,
            0x8A,0xCB,0x63,0x8D,0xEA,0x7D,0x2B,0x5F,
            0xC3,0xB1,0xE9,0x83,0x29,0x51,0xE8,0x56
        };
        buffer_.fill(0);
        size_ = 0;
    }

    const std::array<uint8_t, 32>& XorFilter() const { return m_xor_filter_; }

    bool LoadEncryptionKey(const std::string& path, std::string* err = nullptr) {
        return LoadKey(path, 0, err);
    }

    bool LoadDecryptionKey(const std::string& path, std::string* err = nullptr) {
        return LoadKey(path, 1, err);
    }

    bool LoadKey(const std::string& path, int type, std::string* err = nullptr) {
        EncDecKeys raw{};
        if (!LoadEncDecKeysFromFile(path, raw, err)) return false;
        Keys k{};
        for (int i = 0; i < 4; ++i) {
            k.modulus[i] = raw.modulus[i];
            k.key[i] = raw.key[i];
            k.xor_[i] = raw.xor_[i];
        }
        if (type == 0) {
            encryption_.SetKeys(k);
        } else if (type == 1) {
            decryption_.SetKeys(k);
        } else {
            if (err) *err = "type invalido (0=enc,1=dec)";
            return false;
        }
        return true;
    }

    bool AddData(const std::vector<uint8_t>& data, std::string* err = nullptr) {
        if (data.empty() || data.size() >= buffer_.size()) {
            if (err) *err = "packet size must be 1..2047";
            return false;
        }
        std::copy(data.begin(), data.end(), buffer_.begin());
        size_ = data.size();
        return true;
    }

    bool ExtractPacket(std::vector<uint8_t>& out, std::string* err = nullptr) {
        if (size_ < 2) {
            if (err) *err = "buffer too small";
            return false;
        }
        std::size_t packet_size = 0;
        int end = 0;
        if (buffer_[0] == 0xC1) {
            packet_size = buffer_[1];
            end = 2;
        } else if (buffer_[0] == 0xC2) {
            if (size_ < 3) {
                if (err) *err = "C2 header truncated";
                return false;
            }
            packet_size = static_cast<std::size_t>((buffer_[1] << 8) | buffer_[2]);
            end = 3;
        } else {
            if (err) *err = "unsupported classic packet head";
            return false;
        }
        if (packet_size < static_cast<std::size_t>(end) || packet_size > size_) {
            if (err) *err = "declared packet size exceeds buffered bytes";
            return false;
        }
        XorData(static_cast<int>(packet_size - 1), end);
        out.assign(buffer_.begin(), buffer_.begin() + packet_size);
        return true;
    }

    void XorData(int start, int end) {
        if (start < end) return;
        for (int n = start; n > end; --n) {
            buffer_[static_cast<std::size_t>(n)] ^=
                buffer_[static_cast<std::size_t>(n - 1)] ^
                m_xor_filter_[static_cast<std::size_t>(n) % 32];
        }
    }

    bool EncryptionKeysLoaded() const { return encryption_.KeysLoaded(); }
    bool DecryptionKeysLoaded() const { return decryption_.KeysLoaded(); }
    const Keys& EncryptionKeys() const { return encryption_.DebugKeys(); }
    const Keys& DecryptionKeys() const { return decryption_.DebugKeys(); }

private:
    std::array<uint8_t, 32> m_xor_filter_{};
    std::array<uint8_t, 2048> buffer_{};
    std::size_t size_ = 0;
    PacketCryptoSM encryption_;
    PacketCryptoSM decryption_;
};

} // namespace newera::crypto
