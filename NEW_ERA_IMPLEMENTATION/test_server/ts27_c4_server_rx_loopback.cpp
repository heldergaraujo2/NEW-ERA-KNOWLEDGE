#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include "../mvp_login/crypto_cpacketmanager_server.h"
#include "../mvp_login/crypto_c4_server_rx.h"

static std::vector<uint8_t> ApplyWireXor(const std::vector<uint8_t>& plain,
                                         const std::array<uint8_t,32>& filter) {
    std::vector<uint8_t> wire = plain;
    for (std::size_t n = 3; n < wire.size(); ++n)
        wire[n] = static_cast<uint8_t>(plain[n] ^ wire[n-1] ^ filter[n%32]);
    return wire;
}

static newera::crypto::PacketCryptoSM::Keys Invert(const newera::crypto::PacketCryptoSM::Keys& in) {
    auto out=in;
    for (int i=0;i<4;++i) {
        uint32_t m=in.modulus[i], a=in.key[i]%m;
        int64_t t=0,nt=1,r=m,nr=a;
        while(nr){int64_t q=r/nr; int64_t z=t-q*nt; t=nt; nt=z; int64_t rr=r-q*nr; r=nr; nr=rr;}
        assert(r==1); if(t<0)t+=m; out.key[i]=static_cast<uint32_t>(t);
    }
    return out;
}

int main() {
    static const uint8_t enc1[54] = {
        0x12,0x11,0x36,0x00,0x00,0x00,0xD4,0x53,0x09,0x3F,0x01,0x41,0x5E,0xE2,0xE2,0x68,
        0xD3,0x93,0x2D,0x06,0xDF,0x20,0x5A,0xFC,0x08,0x3F,0x00,0xEC,0x5C,0xE2,0xD1,0x37,
        0xD2,0x93,0xF0,0x92,0xDE,0x20,0x86,0x1A,0x08,0x3F,0xD2,0x76,0x5C,0xE2,0xFA,0x41,
        0xD2,0x93,0x86,0x35,0xDE,0x20
    };
    const std::string path="/tmp/newera_enc1_c4.dat";
    FILE* f=std::fopen(path.c_str(),"wb"); assert(f);
    assert(std::fwrite(enc1,1,54,f)==54); std::fclose(f);

    newera::crypto::PacketCryptoSM enc,dec; std::string err;
    assert(enc.LoadKeysFromFile(path,&err,0));
    dec.SetKeys(Invert(enc.DebugKeys()));

    std::vector<uint8_t> plain(300,0);
    plain[0]=0xC2; plain[1]=0x01; plain[2]=0x2C;
    plain[3]=0xF3; plain[4]=0x10;
    for(std::size_t i=5;i<plain.size();++i) plain[i]=static_cast<uint8_t>((i*37u+11u)&0xFF);

    std::array<uint8_t,32> filter = {
        0xE7,0x6D,0x3A,0x89,0xBC,0xB2,0x9F,0x73,0x23,0xA8,0xFE,0xB6,0x49,0x5D,0x39,0x5D,
        0x8A,0xCB,0x63,0x8D,0xEA,0x7D,0x2B,0x5F,0xC3,0xB1,0xE9,0x83,0x29,0x51,0xE8,0x56
    };
    const auto xored=ApplyWireXor(plain,filter);
    std::vector<uint8_t> cipher(((xored.size()+7)/8)*11);
    const int clen=enc.Encrypt(cipher.data(),xored.data(),static_cast<int>(xored.size()));
    assert(clen>0); cipher.resize(static_cast<std::size_t>(clen));

    std::vector<uint8_t> wire(3+cipher.size());
    wire[0]=0xC4; wire[1]=static_cast<uint8_t>(wire.size()>>8); wire[2]=static_cast<uint8_t>(wire.size());
    std::memcpy(wire.data()+3,cipher.data(),cipher.size());

    newera::crypto::C4DecodedPacket out;
    assert(newera::crypto::DecodeC4ServerFrame(wire,dec,out,&err));
    assert(out.classic_c2==plain);

    auto bad=wire; bad[2] ^= 1;
    assert(!newera::crypto::DecodeC4ServerFrame(bad,dec,out,&err));
    std::cout<<"TS-27 C4 SimpleModulus + server XOR32 decode: PASS\n";
}
