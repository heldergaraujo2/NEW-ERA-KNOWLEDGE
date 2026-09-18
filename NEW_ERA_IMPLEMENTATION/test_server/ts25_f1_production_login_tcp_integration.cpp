#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <sys/types.h>
#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif
#include "../mvp_login/f1_01_login_request.h"
#include "../mvp_login/f1_01_login_server_rx.h"
#include "../mvp_login/f1_login_server_decision.h"
using namespace newera::login;
static const uint8_t enc1[]={0x12,0x11,0x36,0x00,0x00,0x00,0xd4,0x53,0x09,0x3f,0x01,0x41,0x5e,0xe2,0xe2,0x68,0xd3,0x93,0x2d,0x06,0xdf,0x20,0x5a,0xfc,0x08,0x3f,0x00,0xec,0x5c,0xe2,0xd1,0x37,0xd2,0x93,0xf0,0x92,0xde,0x20,0x86,0x1a,0x08,0x3f,0xd2,0x76,0x5c,0xe2,0xfa,0x41,0xd2,0x93,0x86,0x35,0xde,0x20};
static bool sendall(int s,const std::vector<uint8_t>&v){size_t n=0;while(n<v.size()){int k=send(s,(const char*)v.data()+n,(int)(v.size()-n),0);if(k<=0)return false;n+=k;}return true;}
static bool recvn(int s,std::vector<uint8_t>&v){size_t n=0;while(n<v.size()){int k=recv(s,(char*)v.data()+n,(int)(v.size()-n),0);if(k<=0)return false;n+=k;}return true;}
static int closefd(int s){#ifdef _WIN32 return closesocket(s); #else return close(s); #endif}
int main(){
 PacketCryptoSM sm; std::string err; assert(sm.LoadKeysFromMemory(enc1,sizeof(enc1),err));
 LoginRequestInput in{}; in.id="TestHero"; in.password="P@ss123"; in.tick_count=0x78563412u; std::memcpy(in.version,"12345",5); for(int i=0;i<16;i++)in.protocol_serial[i]=(uint8_t)('A'+i); in.packet_serial=7;
 std::vector<uint8_t> wire; assert(BuildC3(in,sm,wire,err)); assert(wire.size()==79);
 int ls=socket(AF_INET,SOCK_STREAM,0);assert(ls>=0); sockaddr_in a{};a.sin_family=AF_INET;a.sin_addr.s_addr=htonl(INADDR_LOOPBACK);a.sin_port=0;assert(bind(ls,(sockaddr*)&a,sizeof(a))==0);assert(listen(ls,1)==0); socklen_t al=sizeof(a);assert(getsockname(ls,(sockaddr*)&a,&al)==0);uint16_t port=ntohs(a.sin_port);
 std::vector<uint8_t> response(5); bool server_ok=false; std::thread srv([&]{int cs=accept(ls,nullptr,nullptr);assert(cs>=0);std::vector<uint8_t> rx(79);assert(recvn(cs,rx));std::string de;DecodedLoginRequest d;assert(DecodeF101(rx.data(),rx.size(),sm,d,de));ServerLoginPolicy p;p.expected_version="12345";p.expected_protocol_serial=std::string("ABCDEFGHIJKLMNOP",16);assert(ValidateServerLoginRequest(d,p,de));uint8_t out[5];assert(BuildLoginResultC1(JoinServerResult::Success,out));response.assign(out,out+5);server_ok=send(cs,(const char*)out,5,0)==5;closefd(cs);});
 int cs=socket(AF_INET,SOCK_STREAM,0);assert(cs>=0);a.sin_port=htons(port);assert(connect(cs,(sockaddr*)&a,sizeof(a))==0);assert(sendall(cs,wire));assert(recvn(cs,response));assert(response==std::vector<uint8_t>({0xc1,0x05,0xf1,0x01,0x01}));closefd(cs);srv.join();closefd(ls);assert(server_ok);std::cout<<"TS-25 F1 production login server TCP integration: PASS\\n";return 0;}
