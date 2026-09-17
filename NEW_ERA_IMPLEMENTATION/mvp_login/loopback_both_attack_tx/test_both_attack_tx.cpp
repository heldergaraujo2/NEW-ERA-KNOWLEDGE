#include "../both_attack_tx.h"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

static void Expect(const std::vector<uint8_t>& got, const std::vector<uint8_t>& want) { assert(got == want); }

int main() {
    using namespace newera::both_attack;
    std::vector<uint8_t> out;
    std::string error;

    assert(BuildAttack1(0x0101, 0x78, 0x03, out, error));
    Expect(out, {0x08,0x00,0x07,0x00,0x00,0x00,0xC1,0x07,0x11,0x01,0x01,0x78,0x03});

    assert(BuildAttack2(0x0009, 0x32, 0x3C, 0x01, 0x01, out, error));
    Expect(out, {0x09,0x00,0x09,0x00,0x00,0x00,0xC3,0x09,0xDB,0x00,0x01,0x09,0x32,0x01,0x3C});

    assert(!BuildAttack1(0, 0x78, 0, out, error));
    assert(!BuildAttack1(1, 0x78, 8, out, error));
    assert(!BuildAttack2(0x0109, 1, 2, 1, 0, out, error));
    assert(!BuildAttack2(0x0109, 1, 2, 0, 1, out, error));
    assert(!BuildAttack2(0x0109, 1, 2, 51, 1, out, error));

    std::cout << "1.3-RP2 BOTH_ATTACK1/2 core golden+guards: PASS\n";
    return 0;
}
