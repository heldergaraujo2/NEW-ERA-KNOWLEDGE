// NEW-ERA TS-19 — real TCP F1 login-flow integration fixture.
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include "../mvp_login/f1_login_flow.h"
static bool SendAll(int s,const uint8_t*p,size_t n){size_t k=0;while(k<n){ssize_t r=send(s,p+k,n-k,0);if(r<=0)return false;k+=size_t(r);}return true;}
static bool RecvAll(int s,uint8_t*p,size_t n){size_t k=0;while(k<n){ssize_t r=recv(s,p+k,n-k,0);if(r<=0)return false;k+=size_t(r);}return true;}
int main(){
 const uint8_t version[5]={0x35,0x30,0x32,0x41,0x42};
 const uint16_t hero=0x1234;
 const uint8_t handshake[12]={0xC1,0x0C,0xF1,0x00,0x01,0x12,0x34,0x34,0x2E,0x2F,0x3D,0x3D};
 const uint8_t result[5]={0xC1,0x05,0xF1,0x01,0x01};
 int ls=socket(AF_INET,SOCK_STREAM,0);if(ls<0)return 2;int one=1;setsockopt(ls,SOL_SOCKET,SO_REUSEADDR,&one,sizeof(one));
 sockaddr_in a{};a.sin_family=AF_INET;a.sin_addr.s_addr=htonl(INADDR_LOOPBACK);a.sin_port=0;
 if(bind(ls,(sockaddr*)&a,sizeof(a))<0||listen(ls,1)<0)return 2;socklen_t alen=sizeof(a);getsockname(ls,(sockaddr*)&a,&alen);uint16_t port=ntohs(a.sin_port);
 std::thread server([&]{int s=accept(ls,nullptr,nullptr);if(s<0)return;if(!SendAll(s,handshake,sizeof(handshake))){close(s);return;}uint8_t h[2];if(!RecvAll(s,h,2)){close(s);return;}if(h[0]!=0xC3||h[1]!=0x4F){close(s);return;}std::vector<uint8_t> body(77);if(!RecvAll(s,body.data(),body.size())){close(s);return;}SendAll(s,result,sizeof(result));close(s);});
 int c=socket(AF_INET,SOCK_STREAM,0);if(c<0)return 2;sockaddr_in ca{};ca.sin_family=AF_INET;ca.sin_addr.s_addr=htonl(INADDR_LOOPBACK);ca.sin_port=htons(port);if(connect(c,(sockaddr*)&ca,sizeof(ca))<0)return 2;
 newera::login::LoginFlow flow(version);flow.Start();uint8_t rxh[12];if(!RecvAll(c,rxh,sizeof(rxh)))return 2;
 if(!flow.AcceptServerVersion(rxh,sizeof(rxh))||flow.state()!=newera::login::SessionState::AwaitingResult||flow.hero_key()!=hero)return 3;
 newera::login::LoginRequestInput in{};in.id="PLAYER";in.password="SECRET";in.tick_count=0x12345678;std::memcpy(in.version,version,5);for(int i=0;i<16;i++)in.protocol_serial[i]=uint8_t(0xA0+i);in.packet_serial=0x2A;
 newera::crypto::PacketCryptoSM sm;newera::crypto::PacketCryptoSM::Keys k{};for(int i=0;i<4;i++){k.modulus[i]=65521;k.key[i]=3;k.xor_[i]=7;}sm.SetKeys(k);
 std::vector<uint8_t> c3;std::string err;if(!flow.BuildLoginRequest(in,sm,c3,err)||c3.size()!=79||c3[0]!=0xC3||c3[1]!=0x4F)return 4;if(!SendAll(c,c3.data(),c3.size()))return 5;
 uint8_t rr[5];if(!RecvAll(c,rr,sizeof(rr)))return 6;if(!flow.AcceptLoginResult(rr,sizeof(rr))||flow.state()!=newera::login::SessionState::LoggedIn)return 7;
 close(c);close(ls);server.join();std::cout<<"TS-19 F1 login flow real TCP integration: PASS\n";return 0;
}
