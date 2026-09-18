#include <cassert>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "../f1_01_login_request.h"
static const uint8_t kEnc1[54]={0x12,0x11,0x36,0x00,0x00,0x00,0xD5,0x53,0x09,0x3F,0x01,0x01,0x5E,0xE2,0xE2,0x62,0xE3,0x93,0x2D,0x07,0xDF,0x16,0xC2,0x17,0xC0,0x08,0x3F,0x00,0xEC,0x5E,0x3C,0xF0,0xD7,0xD8,0x93,0xF0,0xD2,0x43,0xF8,0x08,0x3F,0xD4,0x9E,0x00,0xF2,0x49,0x1C,0xD6,0xC3,0xE6,0x18,0x08,0x3F,0xB0};
int main(){
 const char* path="/tmp/newera_real_Enc1.dat";{std::ofstream f(path,std::ios::binary);assert(f.write(reinterpret_cast<const char*>(kEnc1),54));}
 newera::crypto::PacketCryptoSM sm;std::string err;assert(sm.LoadKeysFromFile(path,&err,0));assert(sm.DebugKeys().modulus[0]==0x1f44f&&sm.DebugKeys().key[0]==0x5bc1&&sm.DebugKeys().xor_[0]==0xbd1d);
 newera::login::LoginRequestInput in;in.id="PLAYER";in.password="SECRET";in.tick_count=0x12345678;const uint8_t v[5]={0x35,0x30,0x32,0x41,0x42};for(int i=0;i<5;i++)in.version[i]=v[i];for(int i=0;i<16;i++)in.protocol_serial[i]=uint8_t(0xa0+i);in.packet_serial=0x2a;
 std::vector<uint8_t> frame;assert(newera::login::BuildC3(in,sm,frame,err));assert(frame.size()==79);
 const char* golden="c34f1fac23f486f52295d0daefbe0868fac969b3b604d9ecead643f6163f566f2c093ccd111749d3502568e4102542337e4d96cf03b064f7c293bc61e5caf8e00a15c9fc2e397d52c62f214b046f53";
 for(size_t i=0;i<frame.size();++i){char h[3];std::snprintf(h,sizeof(h),"%02x",frame[i]);assert(h[0]==golden[i*2]&&h[1]==golden[i*2+1]);}
 std::remove(path);std::cout<<"TS-21 F1:01 production Enc1 loader + C3 golden: PASS\n";return 0;
}