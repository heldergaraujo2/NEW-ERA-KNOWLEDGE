// NEW-ERA — MVP LOGIN — F1:02 logout request builder
// Normative source: pinned wongddd/muonline@580472e, Source Client/source/wsclientinline.h.
// SendRequestLogOut: C1 F1 02 Flag, sent with encryption enabled.
#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <cstring>
#include "crypto_xordata32.h"
#include "crypto_cpacketmanager_min.h"

namespace newera {
namespace login {

struct LogoutRequestInput {
    uint8_t flag = 0;
};

inline bool BuildLogicalC1(const LogoutRequestInput& in,
                           std::vector<uint8_t>& out,
                           std::string& error) {
    (void)error;
    out = {0xC1, 0x05, 0xF1, 0x02, in.flag};
    crypto::XorData32(out.data(), 3, out.size());
    return true;
}

inline bool BuildC3(const LogoutRequestInput& in,
                    crypto::PacketCryptoSM& sm,
                    std::vector<uint8_t>& out,
                    std::string& error) {
    if (!sm.KeysLoaded()) {
        error = "SimpleModulus Enc1 keys not loaded";
        return false;
    }
    std::vector<uint8_t> logical;
    if (!BuildLogicalC1(in, logical, error)) {
        return false;
    }
    uint8_t padded[8] = {};
    std::memcpy(padded, logical.data(), logical.size());
    uint8_t cipher[11] = {};
    const int n = sm.Encrypt(cipher, padded, static_cast<int>(logical.size()));
    if (n != 11) {
        error = "unexpected SimpleModulus ciphertext length";
        return false;
    }
    out.clear();
    out.reserve(13);
    out.push_back(0xC3);
    out.push_back(0x0D);
    out.insert(out.end(), cipher, cipher + 11);
    return true;
}

} // namespace login
} // namespace newera
