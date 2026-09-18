#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include "../f1_login_server_decision.h"
using namespace newera::login;
int main(){
 ServerLoginPolicy p; p.expected_version="12345"; p.expected_protocol_serial=std::string("ABCDEFGHIJKLMNOP",16);
 DecodedLoginRequest r; r.id="TestHero"; r.password="P@ss123"; r.packet_serial=7; std::memcpy(r.version,"12345",5); std::memcpy(r.protocol_serial,"ABCDEFGHIJKLMNOP",16);
 std::string e; assert(ValidateServerLoginRequest(r,p,e));
 uint8_t out[5]; for(auto x:{JoinServerResult::PasswordWrong,JoinServerResult::Success,JoinServerResult::AccountMissing,JoinServerResult::AlreadyConnected,JoinServerResult::ServerFull}){assert(BuildLoginResultC1(x,out));assert(out[0]==0xC1&&out[1]==5&&out[2]==0xF1&&out[3]==1&&out[4]==MapJoinServerResult(x));}
 assert(BuildVersionFailureC1(out)); assert(out[4]==6);
 DecodedLoginRequest bad=r; std::memcpy(bad.version,"99999",5); assert(!ValidateServerLoginRequest(bad,p,e)); bad=r; bad.packet_serial=0; assert(!ValidateServerLoginRequest(bad,p,e)); bad=r; bad.id.clear(); assert(!ValidateServerLoginRequest(bad,p,e));
 std::cout<<"TS-24 F1 server login decision/result mapping: PASS\\n"; return 0;
}
