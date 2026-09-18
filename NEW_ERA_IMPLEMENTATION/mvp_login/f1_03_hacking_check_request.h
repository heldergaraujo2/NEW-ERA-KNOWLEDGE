// NEW-ERA — MVP LOGIN — F1:03 hacking-check request builder
// Normative source: pinned wongddd/muonline@580472e, Source Client/source/wsclientinline.h.
// SendHackingChecked(byType,byParam): C1/F1/03 + type + param, spe.Send(TRUE).
#pragma once
#include <stdint.h>
#include <cstring>
#include <string>
#include <vector>
#include "crypto_xordata32.h"
#include "crypto_cpacketmanager_min.h"
namespace newera { namespace login {
struct HackingCheckInput { uint8_t type=0; uint8_t param=0; };
inline bool BuildLogicalC1(const HackingCheckInput& in,std::vector<uint8_t>& out,std::string& error){
    (void)error; out={0xC1,0x06,0xF1,0x03,in.type,in.param}; crypto::XorData32(out.data(),3,out.size()); return true;
}
inline bool BuildC3(const HackingCheckInput& in,crypto::PacketCryptoSM& sm,std::vector<uint8_t>& out,std::string& error){
    if(!sm.KeysLoaded()){error="SimpleModulus Enc1 keys not loaded";return false;}
    std::vector<uint8_t> logical; if(!BuildLogicalC1(in,logical,error)) return false;
    uint8_t padded[8]={}; std::memcpy(padded,logical.data(),logical.size());
    uint8_t cipher[11]={}; if(sm.Encrypt(cipher,padded,(int)logical.size())!=11){error="unexpected SimpleModulus ciphertext length";return false;}
    out.clear();out.push_back(0xC3);out.push_back(0x0D);out.insert(out.end(),cipher,cipher+11);return true;
}
}}
