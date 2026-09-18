// NEW-ERA — F1 server login decision layer
// Protocol evidence: 0C.9-B / 0C.9-C. This is a compatible reconstruction,
// not a claim of original database/authentication implementation.
#pragma once
#include <stdint.h>
#include <string>
#include "f1_01_login_server_rx.h"
namespace newera { namespace login {
enum class JoinServerResult : uint8_t { PasswordWrong=0, Success=1, AccountMissing=2, AlreadyConnected=3, ServerFull=4 };
struct ServerLoginPolicy { std::string expected_version; std::string expected_protocol_serial; bool require_nonzero_packet_serial=true; };
inline bool ValidateServerLoginRequest(const DecodedLoginRequest& r,const ServerLoginPolicy& p,std::string& error){
 std::string v(reinterpret_cast<const char*>(r.version),5); std::string s(reinterpret_cast<const char*>(r.protocol_serial),16);
 if(r.id.empty()){error="empty account";return false;} if(r.password.empty()){error="empty password";return false;}
 if(p.require_nonzero_packet_serial&&r.packet_serial==0){error="packet serial is zero";return false;}
 if(!p.expected_version.empty()&&v!=p.expected_version){error="version mismatch";return false;}
 if(!p.expected_protocol_serial.empty()&&s!=p.expected_protocol_serial){error="protocol serial mismatch";return false;}
 return true;
}
inline uint8_t MapJoinServerResult(JoinServerResult r){return static_cast<uint8_t>(r);}
inline bool BuildLoginResultC1(JoinServerResult r,uint8_t out[5]){if(!out)return false;out[0]=0xC1;out[1]=0x05;out[2]=0xF1;out[3]=0x01;out[4]=MapJoinServerResult(r);return true;}
inline bool BuildVersionFailureC1(uint8_t out[5]){if(!out)return false;out[0]=0xC1;out[1]=0x05;out[2]=0xF1;out[3]=0x01;out[4]=0x06;return true;}
}} // namespace newera::login
