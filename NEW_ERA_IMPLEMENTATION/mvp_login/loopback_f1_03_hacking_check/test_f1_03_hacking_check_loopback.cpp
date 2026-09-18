#include "../f1_03_hacking_check_request.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <string>
#include <thread>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using socket_t=int;
static void CloseSocket(socket_t s){close(s);}
static bool Xfer(socket_t s,uint8_t*p,size_t n,bool sendMode){size_t d=0;while(d<n){ssize_t r=sendMode?send(s,p+d,n-d,0):recv(s,p+d,n-d,0);if(r<=0)return false;d+=size_t(r);}return true;}
static std::string Hex(const uint8_t*p,size_t n){char b[3];std::string s;for(size_t i=0;i<n;++i){snprintf(b,3,"%02x",p[i]);s+=b;}return s;}
int main(){
 newera::crypto::PacketCryptoSM::Keys ek{}; for(int i=0;i<4;++i){ek.modulus[i]=65521;ek.key[i]=3;ek.xor_[i]=7;} newera::crypto::PacketCryptoSM enc;enc.SetKeys(ek);
 newera::login::HackingCheckInput in{0x12,0x34}; std::vector<uint8_t> logical,frame; std::string err;
 assert(newera::login::BuildLogicalC1(in,logical,err)); assert(logical.size()==6); assert(logical[0]==0xC1&&logical[1]==0x06&&logical[2]==0xF1); assert(logical[3]==0x7B&&logical[4]==0xD5&&logical[5]==0x53);
 assert(newera::login::BuildC3(in,enc,frame,err)); assert(frame.size()==13&&frame[0]==0xC3&&frame[1]==0x0D);
 newera::crypto::PacketCryptoSM::Keys dk=ek;for(int i=0;i<4;++i)dk.key[i]=43681;newera::crypto::PacketCryptoSM dec;dec.SetKeys(dk);uint8_t plain[8]={};assert(dec.Decrypt(plain,frame.data()+2,11)==6);assert(memcmp(plain,logical.data(),6)==0);
 socket_t l=socket(AF_INET,SOCK_STREAM,0);assert(l>=0);sockaddr_in a{};a.sin_family=AF_INET;a.sin_addr.s_addr=htonl(INADDR_LOOPBACK);a.sin_port=0;assert(bind(l,(sockaddr*)&a,sizeof(a))==0);assert(listen(l,1)==0);socklen_t al=sizeof(a);assert(getsockname(l,(sockaddr*)&a,&al)==0);
 std::thread server([&](){socket_t p=accept(l,nullptr,nullptr);assert(p>=0);uint8_t rx[13]{};assert(Xfer(p,rx,13,false));assert(memcmp(rx,frame.data(),13)==0);CloseSocket(p);});
 socket_t c=socket(AF_INET,SOCK_STREAM,0);assert(c>=0);assert(connect(c,(sockaddr*)&a,sizeof(a))==0);assert(Xfer(c,frame.data(),frame.size(),true));CloseSocket(c);server.join();CloseSocket(l);
 printf("logical=%s\n",Hex(logical.data(),logical.size()).c_str());printf("c3=%s\n",Hex(frame.data(),frame.size()).c_str());puts("TS-15 F1:03 hacking-check C3 TCP loopback: PASS"); return 0;
}