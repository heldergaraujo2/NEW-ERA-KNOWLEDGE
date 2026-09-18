// NEW-ERA — MVP LOGIN — F1:01 login request builder
// Normative source: NEW_ERA_PROTOCOL_MVP_LOGIN_SPEC.md §3 and §4.2.
#pragma once
#include <stdint.h>
#include <stddef.h>
#include <string>
#include <vector>
#include <cstring>
#include "crypto_buxconvert.h"
#include "crypto_xordata32.h"
#include "crypto_cpacketmanager_min.h"
namespace newera { namespace login {
static constexpr size_t kLogicalLength = 50;
struct LoginRequestInput { std::string id; std::string password; uint32_t tick_count=0; uint8_t version[5]={0,0,0,0,0}; uint8_t protocol_serial[16]={0}; uint8_t packet_serial=0; };
inline bool EncodeCredential(const std::string& in,uint8_t out[10],std::string& error){ if(in.size()>10){error="credential exceeds 10 bytes";return false;} std::memset(out,0,10); std::memcpy(out,in.data(),in.size()); crypto::BuxConvert(out,10); return true; }
inline bool BuildLogicalC1(const LoginRequestInput& in,std::vector<uint8_t>& out,std::string& error){ uint8_t id[10],pw[10]; if(!EncodeCredential(in.id,id,error)||!EncodeCredential(in.password,pw,error))return false; out.assign(kLogicalLength,0); out[0]=0xC1; out[1]=in.packet_serial; out[2]=0xF1; out[3]=0x01; std::memcpy(out.data()+4,id,10);std::memcpy(out.data()+14,pw,10); out[24]=uint8_t(in.tick_count);out[25]=uint8_t(in.tick_count>>8);out[26]=uint8_t(in.tick_count>>16);out[27]=uint8_t(in.tick_count>>24); for(size_t i=0;i<5;++i)out[28+i]=uint8_t(in.version[i]-uint8_t(i+1)); std::memcpy(out.data()+33,in.protocol_serial,16); crypto::XorData32(out.data(),3,out.size()); return true; }
inline bool BuildC3(const LoginRequestInput& in,crypto::PacketCryptoSM& sm,std::vector<uint8_t>& out,std::string& error){ if(!sm.KeysLoaded()){error="SimpleModulus Enc1 keys not loaded";return false;} std::vector<uint8_t> logical;if(!BuildLogicalC1(in,logical,error))return false; // SendPacket() inserts the packet serial at C1 byte [1] before encryption. The 49-byte encrypted span is [1..49], so the logical request remains 50 bytes in memory including C1 header + serial byte.std::vector<uint8_t> cipher(size_t(blocks)*11);const int n=sm.Encrypt(cipher.data(),logical.data(),int(logical.size()));if(n<0){error="SimpleModulus encryption failed";return false;}if(n>=254){error="ciphertext requires C4 framing";return false;}out.clear();out.reserve(size_t(n)+2);out.push_back(0xC3);out.push_back(uint8_t(n+2));out.insert(out.end(),cipher.begin(),cipher.begin()+n);return true; }
}} // namespace newera::login
