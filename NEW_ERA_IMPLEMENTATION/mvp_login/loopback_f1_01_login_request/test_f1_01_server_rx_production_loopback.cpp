#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "../f1_01_login_request.h"
#include "../f1_01_login_server_rx.h"

int main() {
    static const uint8_t enc1[54] = {
        0x12,0x11,0x36,0x00,0x00,0x00,0xD4,0x53,0x09,0x3F,0x01,0x41,0x5E,0xE2,0xE2,0x68,
        0xD3,0x93,0x2D,0x06,0xDF,0x20,0x5A,0xFC,0x08,0x3F,0x00,0xEC,0x5C,0xE2,0xD1,0x37,
        0xD2,0x93,0xF0,0x92,0xDE,0x20,0x86,0x1A,0x08,0x3F,0xD2,0x76,0x5C,0xE2,0xFA,0x41,
        0xD2,0x93,0x86,0x35,0xDE,0x20
    };
    const std::string path = "/tmp/newera_enc1_f9.dat";
    { FILE* f = std::fopen(path.c_str(), "wb"); assert(f); assert(std::fwrite(enc1,1,54,f)==54); std::fclose(f); }
    newera::crypto::PacketCryptoSM enc;
    std::string err; assert(enc.LoadKeysFromFile(path,&err,0));
    newera::crypto::PacketCryptoSM::Keys dec_keys{};
    assert(newera::login::DeriveDecryptKeysFromEncryptKeys(enc.DebugKeys(), dec_keys, err));
    newera::crypto::PacketCryptoSM dec; dec.SetKeys(dec_keys);

    newera::login::LoginRequestInput in;
    in.id="TestHero"; in.password="P@ss123"; in.tick_count=0x78563412;
    in.version[0]='1'; in.version[1]='2'; in.version[2]='3'; in.version[3]='4'; in.version[4]='5';
    for (int i=0;i<16;++i) in.protocol_serial[i]=static_cast<uint8_t>(0xA0+i);
    in.packet_serial=0x07;
    std::vector<uint8_t> c3;
    assert(newera::login::BuildC3(in,enc,c3,err));
    assert(c3.size()==79);

    newera::login::DecodedLoginRequest out;
    assert(newera::login::DecodeF101(c3,dec,out,err));
    assert(out.packet_serial==in.packet_serial);
    assert(out.id==in.id && out.password==in.password);
    assert(out.tick_count==in.tick_count);
    for (int i=0;i<5;++i) assert(out.version[i]==in.version[i]);
    assert(std::memcmp(out.protocol_serial,in.protocol_serial,16)==0);

    std::cout << "TS-23 F1:01 production-key server RX decode round-trip: PASS\\n";
    return 0;
}
