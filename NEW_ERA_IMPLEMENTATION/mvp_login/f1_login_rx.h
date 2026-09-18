#pragma once
#include <cstddef>
#include <cstdint>
#include <string>

namespace newera::login_rx {

inline bool ValidateServerVersion(const uint8_t packet_version[5], const uint8_t expected_client_version[5], std::string& error) {
    for (std::size_t i=0;i<5;++i) {
        const uint8_t expected_wire=static_cast<uint8_t>(expected_client_version[i]-static_cast<uint8_t>(i+1));
        if(packet_version[i]!=expected_wire){ error="F1:00: version mismatch at byte "+std::to_string(i); return false; }
    }
    return true;
}

struct LoginResult { uint8_t value=0; };
inline bool ParseLoginResultC1(const uint8_t* packet,std::size_t size,LoginResult& out,std::string& error) {
    if(packet==nullptr || size<5){error="F1:01: expected C1 frame of at least 5 bytes";return false;}
    if(packet[0]!=0xC1 || packet[1]!=size || packet[2]!=0xF1 || packet[3]!=0x01){error="F1:01: invalid C1 header";return false;}
    out.value=packet[4]; return true;
}

enum class LoginOutcome:uint8_t { Success,WrongPassword,InvalidAccount,AlreadyConnected,ServerFull,VersionMismatch,GenericFailure };
inline LoginOutcome MapLoginValue(uint8_t value){switch(value){case 0x01:case 0x20:return LoginOutcome::Success;case 0x00:return LoginOutcome::WrongPassword;case 0x02:return LoginOutcome::InvalidAccount;case 0x03:return LoginOutcome::AlreadyConnected;case 0x04:return LoginOutcome::ServerFull;case 0x06:return LoginOutcome::VersionMismatch;default:return LoginOutcome::GenericFailure;}}
inline const char* OutcomeName(LoginOutcome o){switch(o){case LoginOutcome::Success:return "success";case LoginOutcome::WrongPassword:return "wrong_password";case LoginOutcome::InvalidAccount:return "invalid_account";case LoginOutcome::AlreadyConnected:return "already_connected";case LoginOutcome::ServerFull:return "server_full";case LoginOutcome::VersionMismatch:return "version_mismatch";default:return "generic_failure";}}
} // namespace newera::login_rx
