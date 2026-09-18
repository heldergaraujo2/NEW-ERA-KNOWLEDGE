#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include "f1_login_session.h"
#include "f1_01_login_request.h"
namespace newera::login {
class LoginFlow {
public:
 explicit LoginFlow(const uint8_t expected_version[5]):session_(expected_version){}
 void Start(){session_.Start();}
 bool AcceptServerVersion(const uint8_t*p,std::size_t n){return session_.ConsumeHandshake(p,n);}
 bool BuildLoginRequest(const LoginRequestInput& in,crypto::PacketCryptoSM& sm,std::vector<uint8_t>& out,std::string& error){
   if(session_.state()!=SessionState::AwaitingResult){error="login request before valid F1:00";return false;}
   return BuildC3(in,sm,out,error);
 }
 bool AcceptLoginResult(const uint8_t*p,std::size_t n){return session_.ConsumeLoginResult(p,n);}
 SessionState state()const{return session_.state();}
 uint16_t hero_key()const{return session_.hero_key();}
private: LoginSession session_;
};
}
