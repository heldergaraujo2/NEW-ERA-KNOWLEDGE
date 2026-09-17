#include "../mvp_login/f3_13_equipment_charset.h"
#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <thread>
#include <vector>
#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib,"Ws2_32.lib")
using socket_t=SOCKET; static constexpr socket_t BAD=INVALID_SOCKET;
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
using socket_t=int; static constexpr socket_t BAD=-1;
#endif
static void Close(socket_t s){
#ifdef _WIN32
closesocket(s);
#else
close(s);
#endif
}
static bool Xfer(socket_t s,uint8_t* p,size_t n,bool tx){size_t d=0;while(d<n){
#ifdef _WIN32
int r=tx?send(s,reinterpret_cast<const char*>(p+d),(int)(n-d),0):recv(s,reinterpret_cast<char*>(p+d),(int)(n-d),0);
#else
ssize_t r=tx?send(s,p+d,n-d,0):recv(s,p+d,n-d,0);
#endif
if(r<=0)return false;d+=(size_t)r;}return true;}
static void W16(uint8_t* p,uint16_t v){p[0]=(uint8_t)v;p[1]=(uint8_t)(v>>8);}
static void W32(uint8_t* p,uint32_t v){p[0]=(uint8_t)v;p[1]=(uint8_t)(v>>8);p[2]=(uint8_t)(v>>16);p[3]=(uint8_t)(v>>24);}
static uint16_t R16(const uint8_t* p){return(uint16_t)(p[0]|(p[1]<<8));}
static uint32_t R32(const uint8_t* p){return(uint32_t)(p[0]|(p[1]<<8)|(p[2]<<16)|(p[3]<<24));}
static std::vector<uint8_t> Env(const std::vector<uint8_t>& body){std::vector<uint8_t> v(6+body.size());W16(v.data(),0x000C);W32(v.data()+2,(uint32_t)body.size());std::memcpy(v.data()+6,body.data(),body.size());return v;}
int main(){
#ifdef _WIN32
WSADATA w{};assert(WSAStartup(MAKEWORD(2,2),&w)==0);
#endif
socket_t ls=socket(AF_INET,SOCK_STREAM,0);assert(ls!=BAD);int yes=1;
#ifdef _WIN32
setsockopt(ls,SOL_SOCKET,SO_REUSEADDR,(const char*)&yes,sizeof(yes));
#else
setsockopt(ls,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes));
#endif
sockaddr_in a{};a.sin_family=AF_INET;a.sin_addr.s_addr=htonl(INADDR_LOOPBACK);a.sin_port=0;assert(bind(ls,(sockaddr*)&a,sizeof(a))==0);assert(listen(ls,2)==0);
#ifdef _WIN32
int al=sizeof(a);
#else
socklen_t al=sizeof(a);
#endif
assert(getsockname(ls,(sockaddr*)&a,&al)==0);
std::array<uint8_t,newera::f3_13::kCharSetSize> cs{};for(size_t i=0;i<cs.size();++i)cs[i]=(uint8_t)(0xA0+i);
const auto inner=newera::f3_13::Build(0x0123,cs);const auto wire=Env(inner);assert(wire.size()==30);assert(R16(wire.data())==0x000C);assert(R32(wire.data()+2)==24);
std::thread server([&](){for(int n=0;n<2;++n){socket_t p=accept(ls,nullptr,nullptr);assert(p!=BAD);uint8_t h[6]{};assert(Xfer(p,h,6,false));assert(R16(h)==0x000C);assert(R32(h+2)==4);uint8_t trigger[4]{};assert(Xfer(p,trigger,4,false));auto out=wire;assert(Xfer(p,out.data(),out.size(),true));Close(p);}});
for(int n=0;n<2;++n){socket_t c=socket(AF_INET,SOCK_STREAM,0);assert(c!=BAD);assert(connect(c,(sockaddr*)&a,sizeof(a))==0);auto trigger=Env({0xC1,0x04,0xF1,0x00});assert(Xfer(c,trigger.data(),trigger.size(),true));uint8_t h[6]{};assert(Xfer(c,h,6,false));assert(R16(h)==0x000C);assert(R32(h+2)==24);std::vector<uint8_t> body(24);assert(Xfer(c,body.data(),body.size(),false));assert(body==inner);newera::f3_13::EquipmentCharSet parsed{};std::string err;assert(newera::f3_13::Parse(body,parsed,err));assert(parsed.index==0x0123);assert(parsed.charset==cs);Close(c);}
server.join();Close(ls);
#ifdef _WIN32
WSACleanup();
#endif
std::puts("TS-6 F3:13 multi-accept loopback: PASS");return 0;}
