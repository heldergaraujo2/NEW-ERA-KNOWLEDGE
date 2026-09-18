#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <thread>
#include <vector>
#include "../f1_01_login_request.h"
#ifndef _WIN32
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif
static void ReadAll(int s,uint8_t*p,size_t n){while(n){int r=recv(s,(char*)p,n,0);assert(r>0);p+=r;n-=size_t(r);}}
static void WriteAll(int s,const uint8_t*p,size_t n){while(n){int r=send(s,(const char*)p,n,0);assert(r>0);p+=r;n-=size_t(r);}}
static void InverseStreamXor(std::vector<uint8_t>&b){uint8_t prev=b[2];for(size_t i=3;i<b.size();++i){uint8_t cur=b[i];b[i]^=uint8_t(prev^newera::crypto::kXorFilter32[i%32]);prev=cur;}}
static void MakeKeys(newera::crypto::PacketCryptoSM::Keys&k,uint32_t key){for(int i=0;i<4;++i){k.modulus[i]=65521;k.key[i]=key;k.xor_[i]=7;}}
int main(){newera::login::LoginRequestInput in;in.id="PLAYER";in.password="SECRET";in.tick_count=0x12345678;uint8_t v[5]={0x35,0x30,0x32,0x41,0x42};memcpy(in.version,v,5);for(int i=0;i<16;++i)in.protocol_serial[i]=uint8_t(0xA0+i);in.packet_serial=0x2A;std::vector<uint8_t> logical;std::string err;assert(newera::login::BuildLogicalC1(in,logical,err));assert(logical.size()==50&&logical[0]==0xC1&&logical[1]==in.packet_serial&&logical[2]==0xF1);newera::crypto::PacketCryptoSM enc,dec;newera::crypto::PacketCryptoSM::Keys ek,dk;MakeKeys(ek,3);MakeKeys(dk,43681);enc.SetKeys(ek);dec.SetKeys(dk);std::vector<uint8_t>frame;assert(newera::login::BuildC3(in,enc,frame,err));assert(frame.size()==79&&frame[0]==0xC3&&frame[1]==79); const char* golden="c34f0de31e1a4537821084daef63cf193a06e9118ad4d9ecae551825c3a8e23bc4093c87c61b5c4c8403a55c10254f4f154a8c97036638f7c2a29908798c2300c71cc9fc0a0409050aa2421cd86f53"; for(size_t i=0;i<frame.size();++i){char h[3];snprintf(h,sizeof(h),"%02x",frame[i]);assert(h[0]==golden[i*2]&&h[1]==golden[i*2+1]);}std::vector<uint8_t>payload(56);int n=dec.Decrypt(payload.data(),frame.data()+2,77);assert(n==49);payload.resize(49);std::vector<uint8_t>plain(50,0);plain[0]=0xC1;memcpy(plain.data()+1,payload.data(),49);InverseStreamXor(plain);assert(plain[0]==0xC1&&plain[1]==in.packet_serial&&plain[2]==0xF1&&plain[3]==0x01);
#ifndef _WIN32
int ls=socket(AF_INET,SOCK_STREAM,0);assert(ls>=0);int one=1;setsockopt(ls,SOL_SOCKET,SO_REUSEADDR,&one,sizeof(one));sockaddr_in a{};a.sin_family=AF_INET;a.sin_addr.s_addr=htonl(INADDR_LOOPBACK);a.sin_port=0;assert(bind(ls,(sockaddr*)&a,sizeof(a))==0);assert(listen(ls,1)==0);socklen_t al=sizeof(a);assert(getsockname(ls,(sockaddr*)&a,&al)==0);uint16_t port=ntohs(a.sin_port);std::thread server([&](){int cs=accept(ls,nullptr,nullptr);assert(cs>=0);std::vector<uint8_t>rx(frame.size());ReadAll(cs,rx.data(),rx.size());assert(rx==frame);WriteAll(cs,rx.data(),rx.size());close(cs);});int c=socket(AF_INET,SOCK_STREAM,0);assert(c>=0);sockaddr_in ca{};ca.sin_family=AF_INET;ca.sin_addr.s_addr=htonl(INADDR_LOOPBACK);ca.sin_port=htons(port);assert(connect(c,(sockaddr*)&ca,sizeof(ca))==0);WriteAll(c,frame.data(),frame.size());std::vector<uint8_t>echo(frame.size());ReadAll(c,echo.data(),echo.size());assert(echo==frame);close(c);server.join();close(ls);
#endif
puts("TS-13 F1:01 login request C3 loopback: PASS");return 0;}
