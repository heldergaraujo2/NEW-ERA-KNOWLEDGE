// NEW-ERA — F1:01 server-side request decoder
// Evidence: 0C.9-F (PacketArgumentDecrypt = BuxConvert), 0C.10-A/B
// and KEYS_MANIFEST.md (Enc1 <-> Dec1: same modulus/xor, inverse Key).
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
struct DecodedLoginRequest { uint8_t packet_serial=0; std::string id; std::string password; uint32_t tick_count=0; uint8_t version[5]={0,0,0,0,0}; uint8_t protocol_serial[16]={0}; };
inline bool DeriveDecryptKeysFromEncryptKeys(const crypto::PacketCryptoSM::Keys& enc,crypto::PacketCryptoSM::Keys& dec,std::string& error){ dec=enc; for(int i=0;i<4;++i){uint32_t m=enc.modulus[i],a=enc.key[i]%m;if(m<2||a==0){error="invalid SimpleModulus key for inverse";return false;}int64_t t=0,newt=1,r=m,newr=a;while(newr){int64_t q=r/newr;int64_t tt=t-q*newt;t=newt;newt=tt;int64_t rr=r-q*newr;r=newr;newr=rr;}if(r!=1){error="SimpleModulus key has no modular inverse";return false;}if(t<0)t+=m;dec.key[i]=static_cast<uint32_t>(t);}return true;}
inline bool UndoLoginStreamXor(uint8_t* logical,size_t size,std::string& error){if(!logical||size!=50){error="logical F1:01 size must be 50";return false;}uint8_t previous=logical[2];for(size_t i=3;i<50;++i){uint8_t wire=logical[i];logical[i]=static_cast<uint8_t>(wire^previous^crypto::kXorFilter32[i%32]);previous=wire;}return true;}
inline bool ParseCredential(const uint8_t* src,std::string& out,std::string& error){size_t n=0;while(n<10&&src[n]!=0)++n;for(size_t i=n;i<10;++i)if(src[i]!=0){error="credential is not zero padded";return false;}out.assign(reinterpret_cast<const char*>(src),n);return true;}
inline bool DecodeF101(const std::vector<uint8_t>& c3,crypto::PacketCryptoSM& decryptor,DecodedLoginRequest& out,std::string& error){
 if(c3.size()<13||c3[0]!=0xC3){error="expected C3 F1:01";return false;}if(c3[1]!=c3.size()){error="C3 size mismatch";return false;}size_t cipher_size=c3.size()-2;if(cipher_size==0||(cipher_size%11)!=0){error="invalid SimpleModulus ciphertext size";return false;}std::vector<uint8_t> payload(cipher_size/11*8,0);int n=decryptor.Decrypt(payload.data(),c3.data()+2,static_cast<int>(cipher_size));if(n!=49){error="F1:01 decrypted payload length is not 49";return false;}
 uint8_t logical[50]={0};logical[0]=0xC1;std::memcpy(logical+1,payload.data(),49);if(logical[1]==0){error="packet serial must be non-zero";return false;}if(!UndoLoginStreamXor(logical,sizeof(logical),error))return false;if(logical[2]!=0xF1||logical[3]!=0x01){error="F1:01 opcode invalid after stream XOR";return false;}
 uint8_t id[10],pw[10];std::memcpy(id,logical+4,10);std::memcpy(pw,logical+14,10);crypto::BuxDecrypt(id,10);crypto::BuxDecrypt(pw,10);if(!ParseCredential(id,out.id,error)||!ParseCredential(pw,out.password,error))return false;out.packet_serial=logical[1];out.tick_count=uint32_t(logical[24])|(uint32_t(logical[25])<<8)|(uint32_t(logical[26])<<16)|(uint32_t(logical[27])<<24);for(size_t i=0;i<5;++i)out.version[i]=static_cast<uint8_t>(logical[28+i]+uint8_t(i+1));std::memcpy(out.protocol_serial,logical+33,16);return true;
}
}} // namespace newera::login
