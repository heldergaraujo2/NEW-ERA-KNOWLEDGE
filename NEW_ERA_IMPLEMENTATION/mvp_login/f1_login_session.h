#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include "f1_login_rx.h"
namespace newera::login {
enum class SessionState:uint8_t{Disconnected,AwaitingHandshake,AwaitingResult,LoggedIn,Rejected};
class LoginSession{public:explicit LoginSession(const uint8_t v[5]){for(std::size_t i=0;i<5;++i)expected_[i]=v[i];}void Start(){state_=SessionState::AwaitingHandshake;}SessionState state()const{return state_;}uint16_t hero_key()const{return hero_key_;}uint8_t result_value()const{return result_;}const std::string& error()const{return error_;}
bool ConsumeHandshake(const uint8_t*p,std::size_t n){if(state_!=SessionState::AwaitingHandshake)return Fail("unexpected F1:00 state");if(!p||n!=12||p[0]!=0xC1||p[1]!=0x0C||p[2]!=0xF1||p[3]!=0x00)return Fail("invalid F1:00");if(!login_rx::ValidateServerVersion(p+7,expected_,error_)){state_=SessionState::Rejected;return false;}result_=p[4];if(result_!=1){state_=SessionState::Rejected;error_="F1:00 server rejected";return false;}hero_key_=uint16_t((uint16_t(p[5])<<8)|p[6]);state_=SessionState::AwaitingResult;return true;}
bool ConsumeLoginResult(const uint8_t*p,std::size_t n){if(state_!=SessionState::AwaitingResult)return Fail("unexpected F1:01 state");login_rx::LoginResult r{};if(!login_rx::ParseLoginResultC1(p,n,r,error_)){state_=SessionState::Rejected;return false;}result_=r.value;if(login_rx::MapLoginValue(r.value)==login_rx::LoginOutcome::Success){state_=SessionState::LoggedIn;return true;}state_=SessionState::Rejected;error_=std::string("F1:01 login rejected: ")+login_rx::OutcomeName(login_rx::MapLoginValue(r.value));return false;}
private:bool Fail(const std::string&e){error_=e;state_=SessionState::Rejected;return false;}uint8_t expected_[5]{};SessionState state_=SessionState::Disconnected;uint16_t hero_key_=0;uint8_t result_=0;std::string error_;};}
