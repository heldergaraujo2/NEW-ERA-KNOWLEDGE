#include <cassert>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "../f1_01_login_request.h"
static const uint8_t kEnc1[54]={0x12,0x11,0x36,0x00,0x00,0x00,0xd4,0x53,0x09,0x3f,0x01,0x41,0x5e,0xe2,0xe2,0x68,0xd3,0x93,0x2d,0x06,0xdf,0x20,0x5a,0xfc,0x08,0x3f,0x00,0xec,0x5c,0xe2,0xd1,0x37,0xd2,0x93,0xf0,0x92,0xde,0x20,0x86,0x1a,0x08,0x3f,0xd2,0x76,0x5c,0xe2,0xfa,0x41,0xd2,0x93,0x86,0x35,0xde,0x20};
int main(){
 const char* path="/tmp/newera_real_Enc1.dat";{std::ofstream f(path,std::ios::binary);assert(f.write(reinterpret_cast<const char*>(kEnc1),54));}
 newera::crypto::PacketCryptoSM sm;std::string err;assert(sm.LoadKeysFromFile(path,&err,0));assert(sm.DebugKeys().modulus[0]==0x1f44f&&sm.DebugKeys().key[0]==0x5bc1&&sm.DebugKeys().xor_[0]==0xbd1d);
 newera::login::LoginRequestInput in;in.id="PLAYER";in.password="SECRET";in.tick_count=0x12345678;const uint8_t v[5]={0x35,0x30,0x32,0x41,0x42};for(int i=0;i<5;i++)in.version[i]=v[i];for(int i=0;i<16;i++)in.protocol_serial[i]=uint8_t(0xa0+i);in.packet_serial=0x2a;
 std::vector<uint8_t> frame;assert(newera::login::BuildC3(in,sm,frame,err));assert(frame.size()==79);
 const char* golden="c34f1fac23f486f52295d0daefbe0868fac969b3b604d9ecead643f6163f566f2c093ccd111749d3502568e4102542337e4d96cf03b064f7c293bc61e5caf8e00a15c9fc2e397d52c62f214b046f53";
 for(size_t i=0;i<frame.size();++i){char h[3];std::snprintf(h,sizeof(h),"%02x",frame[i]);assert(h[0]==golden[i*2]&&h[1]==golden[i*2+1]);}
 std::remove(path);std::cout<<"TS-21 F1:01 production Enc1 loader + C3 golden: PASS\n";return 0;
}