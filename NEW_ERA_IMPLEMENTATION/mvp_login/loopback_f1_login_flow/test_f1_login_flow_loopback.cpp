#include "../f1_login_flow.h"
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#ifndef _WIN32
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif
static void R(int s,uint8_t*p,size_t n){while(n){int x=recv(s,(char*)p,n,0);assert(x>0);p+=x;n-=size_t(x);}}
static void W(int s,const uint8_t*p,size_t n){while(n){int x=send(s,(const char*)p,n,0);assert(x>0);p+=x;n-=size_t(x);}}
static void Keys(newera::crypto::PacketCryptoSM::Keys&k,uint32_t q){for(int i=0;i<4;++i){k.modulus[i]=65521;k.key[i]=q;k.xor_[i]=7;}}
int main(){
 const uint8_t v[5]={0x35,0x30,0x32,0x41,0x42};
 const uint8_t hs[12]={0xC1,0x0C,0xF1,0x00,0x01,0x12,0x34,0x34,0x2E,0x2F,0x3D,0x3D};
 const uint8_t result[5]={0xC1,0x05,0xF1,0x01,0x01};
 newera::login::LoginRequestInput in;in.id="PLAYER";in.password="SECRET";in.tick_count=0x12345678;std::memcpy(in.version,v,5);for(int i=0;i<16;++i)in.protocol_serial[i]=uint8_t(0xA0+i);in.packet_serial=0x2A;
 newera::crypto::PacketCryptoSM enc;newera::crypto::PacketCryptoSM::Keys ek;Keys(ek,3);enc.SetKeys(ek);
 int ls=socket(AF_INET,SOCK_STREAM,0);assert(ls>=0);int one=1;setsockopt(ls,SOL_SOCKET,SO_REUSEADDR,&one,sizeof(one));sockaddr_in a{};a.sin_family=AF_INET;a.sin_addr.s_addr=htonl(INADDR_LOOPBACK);a.sin_port=0;assert(bind(ls,(sockaddr*)&a,sizeof(a))==0);assert(listen(ls,1)==0);socklen_t al=sizeof(a);assert(getsockname(ls,(sockaddr*)&a,&al)==0);uint16_t port=ntohs(a.sin_port);
 std::vector<uint8_t> rx;
 std::thread server([&](){int s=accept(ls,nullptr,nullptr);assert(s>=0);W(s,hs,12);uint8_t h[2];R(s,h,2);assert(h[0]==0xC3&&h[1]==79);rx.resize(79);rx[0]=h[0];rx[1]=h[1];R(s,rx.data()+2,77);W(s,result,5);close(s);});
 int c=socket(AF_INET,SOCK_STREAM,0);assert(c>=0);sockaddr_in ca{};ca.sin_family=AF_INET;ca.sin_addr.s_addr=htonl(INADDR_LOOPBACK);ca.sin_port=htons(port);assert(connect(c,(sockaddr*)&ca,sizeof(ca))==0);
 newera::login::LoginFlow flow(v);flow.Start();uint8_t got_hs[12];R(c,got_hs,12);assert(flow.AcceptServerVersion(got_hs,12));std::vector<uint8_t> req;std::string err;assert(flow.BuildLoginRequest(in,enc,req,err));assert(req.size()==79&&req[0]==0xC3&&req[1]==79);W(c,req.data(),req.size());uint8_t got_result[5];R(c,got_result,5);assert(flow.AcceptLoginResult(got_result,5));assert(flow.state()==newera::login::SessionState::LoggedIn&&flow.hero_key()==0x1234);close(c);server.join();close(ls);
 std::cout<<"TS-18 F1 end-to-end flow loopback: PASS\n";
}
