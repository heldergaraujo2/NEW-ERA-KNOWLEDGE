#include <cassert>
#include <cstdint>
#include <cstdio>
#include <fstream>
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
#include "../f1_01_login_request.h"

static const uint8_t kEnc1[54]={0x12,0x11,0x36,0x00,0x00,0x00,0xd4,0x53,0x09,0x3f,0x01,0x41,0x5e,0xe2,0xe2,0x68,0xd3,0x93,0x2d,0x06,0xdf,0x20,0x5a,0xfc,0x08,0x3f,0x00,0xec,0x5c,0xe2,0xd1,0x37,0xd2,0x93,0xf0,0x92,0xde,0x20,0x86,0x1a,0x08,0x3f,0xd2,0x76,0x5c,0xe2,0xfa,0x41,0xd2,0x93,0x86,0x35,0xde,0x20};
static bool xfer(socket_t s,uint8_t* p,size_t n){while(n){int k=recv(s,reinterpret_cast<char*>(p),static_cast<int>(n),0);if(k<=0)return false;p+=k;n-=static_cast<size_t>(k);}return true;}
static bool sendall(socket_t s,const uint8_t* p,size_t n){while(n){int k=send(s,reinterpret_cast<const char*>(p),static_cast<int>(n),0);if(k<=0)return false;p+=k;n-=static_cast<size_t>(k);}return true;}
int main(){
#ifdef _WIN32
WSADATA w{};assert(WSAStartup(MAKEWORD(2,2),&w)==0);
#endif
const char* path="/tmp/newera_ts22_Enc1.dat";{std::ofstream f(path,std::ios::binary);assert(f.write(reinterpret_cast<const char*>(kEnc1),54));}
newera::crypto::PacketCryptoSM sm;std::string err;assert(sm.LoadKeysFromFile(path,&err,0));std::remove(path);
newera::login::LoginRequestInput in;in.id="PLAYER";in.password="SECRET";in.tick_count=0x12345678;const uint8_t v[5]={0x35,0x30,0x32,0x41,0x42};for(int i=0;i<5;i++)in.version[i]=v[i];for(int i=0;i<16;i++)in.protocol_serial[i]=uint8_t(0xa0+i);in.packet_serial=0x2a;
std::vector<uint8_t> frame;assert(newera::login::BuildC3(in,sm,frame,err));assert(frame.size()==79);
const std::string golden="c34f1fac23f486f52295d0daefbe0868fac969b3b604d9ecead643f6163f566f2c093ccd111749d3502568e4102542337e4d96cf03b064f7c293bc61e5caf8e00a15c9fc2e397d52c62f214b046f53";
socket_t listener=socket(AF_INET,SOCK_STREAM,0);assert(listener>=0);
int one=1;setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,reinterpret_cast<char*>(&one),sizeof(one));sockaddr_in a{};a.sin_family=AF_INET;a.sin_addr.s_addr=htonl(INADDR_LOOPBACK);a.sin_port=0;assert(bind(listener,reinterpret_cast<sockaddr*>(&a),sizeof(a))==0);assert(listen(listener,1)==0);socklen_t alen=sizeof(a);assert(getsockname(listener,reinterpret_cast<sockaddr*>(&a),&alen)==0);uint16_t port=ntohs(a.sin_port);
std::thread server([&]{socket_t c=accept(listener,nullptr,nullptr);assert(c>=0);std::vector<uint8_t> got(frame.size());assert(xfer(c,got.data(),got.size()));assert(got==frame);assert(sendall(c,got.data(),got.size()));close_socket(c);});
socket_t c=socket(AF_INET,SOCK_STREAM,0);assert(c>=0);sockaddr_in dst{};dst.sin_family=AF_INET;dst.sin_addr.s_addr=htonl(INADDR_LOOPBACK);dst.sin_port=htons(port);assert(connect(c,reinterpret_cast<sockaddr*>(&dst),sizeof(dst))==0);assert(sendall(c,frame.data(),frame.size()));std::vector<uint8_t> echo(frame.size());assert(xfer(c,echo.data(),echo.size()));assert(echo==frame);close_socket(c);server.join();close_socket(listener);
std::cout<<"TS-22 F1:01 production Enc1 C3 TCP loopback: PASS\n";
#ifdef _WIN32
WSACleanup();
#endif
return 0;
}