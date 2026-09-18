#include "../f1_login_rx.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
using socket_t=SOCKET;
static void close_socket(socket_t s){closesocket(s);}
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
using socket_t=int;
static void close_socket(socket_t s){close(s);}
#endif
static bool send_all(socket_t s,const uint8_t*p,size_t n){while(n){int k=send(s,reinterpret_cast<const char*>(p),(int)n,0);if(k<=0)return false;p+=k;n-=k;}return true;}
static bool recv_all(socket_t s,uint8_t*p,size_t n){while(n){int k=recv(s,reinterpret_cast<char*>(p),(int)n,0);if(k<=0)return false;p+=k;n-=k;}return true;}
int main(){
#ifdef _WIN32
WSADATA w{};assert(WSAStartup(MAKEWORD(2,2),&w)==0);
#endif
const uint8_t client_v[5]={0x35,0x30,0x32,0x41,0x42};
const uint8_t handshake[12]={0xC1,0x0C,0xF1,0x00,0x01,0x12,0x34,0x34,0x2e,0x2f,0x3d,0x3d};
std::string err;assert(newera::login_rx::ValidateServerVersion(handshake+7,client_v,err));
uint8_t bad[5];std::memcpy(bad,handshake+7,5);bad[2]^=1;assert(!newera::login_rx::ValidateServerVersion(bad,client_v,err));
const uint8_t result[5]={0xC1,0x05,0xF1,0x01,0x01};newera::login_rx::LoginResult lr{};assert(newera::login_rx::ParseLoginResultC1(result,5,lr,err));assert(lr.value==1);assert(newera::login_rx::MapLoginValue(lr.value)==newera::login_rx::LoginOutcome::Success);
const uint8_t alt[5]={0xC1,0x05,0xF1,0x01,0x20};assert(newera::login_rx::ParseLoginResultC1(alt,5,lr,err));assert(newera::login_rx::MapLoginValue(lr.value)==newera::login_rx::LoginOutcome::Success);
for(uint8_t v:{uint8_t(0),uint8_t(2),uint8_t(3),uint8_t(4),uint8_t(6),uint8_t(7),uint8_t(0x40)}){uint8_t f[5]={0xC1,0x05,0xF1,0x01,v};assert(newera::login_rx::ParseLoginResultC1(f,5,lr,err));}
uint8_t trunc[4]={0xC1,0x04,0xF1,0x01};assert(!newera::login_rx::ParseLoginResultC1(trunc,4,lr,err));
int ls=socket(AF_INET,SOCK_STREAM,0);assert(ls>=0);int one=1;setsockopt(ls,SOL_SOCKET,SO_REUSEADDR,reinterpret_cast<const char*>(&one),sizeof(one));sockaddr_in a{};a.sin_family=AF_INET;a.sin_addr.s_addr=htonl(INADDR_LOOPBACK);a.sin_port=0;assert(bind(ls,reinterpret_cast<sockaddr*>(&a),sizeof(a))==0);assert(listen(ls,1)==0);socklen_t alen=sizeof(a);assert(getsockname(ls,reinterpret_cast<sockaddr*>(&a),&alen)==0);uint16_t port=ntohs(a.sin_port);
std::thread server([&]{socket_t s=accept(ls,nullptr,nullptr);assert(s>=0);assert(send_all(s,result,5));close_socket(s);});
socket_t c=socket(AF_INET,SOCK_STREAM,0);assert(c>=0);sockaddr_in ca{};ca.sin_family=AF_INET;ca.sin_addr.s_addr=htonl(INADDR_LOOPBACK);ca.sin_port=htons(port);assert(connect(c,reinterpret_cast<sockaddr*>(&ca),sizeof(ca))==0);uint8_t got[5]{};assert(recv_all(c,got,5));close_socket(c);server.join();close_socket(ls);assert(newera::login_rx::ParseLoginResultC1(got,5,lr,err)&&lr.value==1);
std::cout<<"TS-16 F1:00 validation + F1:01 result RX TCP loopback: PASS\n";
#ifdef _WIN32
WSACleanup();
#endif
}
