#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include "../mvp_login/crypto_des_xex3.h"

int main() {
    using newera::crypto::DesXex3;
    const DesXex3::Key key = {
        0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,
        0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
        0x10,0x21,0x32,0x43,0x54,0x65,0x76,0x87
    };
    const DesXex3::Block plain = {0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF};
    const DesXex3::Block expected = {0x36,0x2D,0xFF,0xED,0x3E,0x46,0x3E,0x12};
    DesXex3::Block cipher{}, recovered{};
    std::string err;
    assert(DesXex3::EncryptBlock(key,plain,cipher,&err));
    assert(cipher==expected);
    assert(DesXex3::DecryptBlock(key,cipher,recovered,&err));
    assert(recovered==plain);
    std::cout<<"TS-28 DES-XEX3 block compatibility: PASS\n";
}
