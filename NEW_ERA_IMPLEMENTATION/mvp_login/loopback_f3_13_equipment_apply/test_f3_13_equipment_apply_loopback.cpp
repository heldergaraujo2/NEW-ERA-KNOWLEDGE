#include "../f3_13_equipment_apply.h"

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

namespace {
void SendAll(int fd, const std::vector<uint8_t>& bytes) {
    std::size_t done = 0;
    while (done < bytes.size()) {
        const auto n = send(fd, bytes.data() + done, bytes.size() - done, 0);
        assert(n > 0);
        done += static_cast<std::size_t>(n);
    }
}

void ReceiveAll(int fd, std::vector<uint8_t>& bytes) {
    std::size_t done = 0;
    while (done < bytes.size()) {
        const auto n = recv(fd, bytes.data() + done, bytes.size() - done, 0);
        assert(n > 0);
        done += static_cast<std::size_t>(n);
    }
}
} // namespace

int main() {
    const std::array<uint8_t, 18> charset = {
        0x23, 0x34, 0x10, 0x20, 0x30, 0x00, 0x39, 0x00,
        0x00, 0xA8, 0xF1, 0x10, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00};

    std::vector<uint8_t> frame = {0xC1, 0x18, 0xF3, 0x13, 0x01, 0x23};
    frame.insert(frame.end(), charset.begin(), charset.end());
    assert(frame.size() == 24);

    int listener = socket(AF_INET, SOCK_STREAM, 0);
    assert(listener >= 0);
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = 0;
    assert(bind(listener, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == 0);
    socklen_t address_size = sizeof(address);
    assert(getsockname(listener, reinterpret_cast<sockaddr*>(&address), &address_size) == 0);
    assert(listen(listener, 1) == 0);

    std::thread server([&] {
        const int client = accept(listener, nullptr, nullptr);
        assert(client >= 0);
        std::vector<uint8_t> received(frame.size());
        ReceiveAll(client, received);
        assert(received == frame);
        SendAll(client, frame);
        close(client);
    });

    const int client = socket(AF_INET, SOCK_STREAM, 0);
    assert(client >= 0);
    assert(connect(client, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == 0);
    SendAll(client, frame);
    std::vector<uint8_t> echoed(frame.size());
    ReceiveAll(client, echoed);
    close(client);
    server.join();
    close(listener);
    assert(echoed == frame);

    std::array<uint8_t, 18> e{};
    std::copy(echoed.begin() + 6, echoed.end(), e.begin());

    const newera::equipment_apply::ModelBases bases = {
        1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000,
        9000, 9100, 9200, 9300, 9400};
    newera::equipment_apply::State state;
    std::string error;
    assert(newera::equipment_apply::Apply(e, bases, 2, state, error));

    assert(state.weapon0 == 1000 + 0x123);
    assert(state.weapon1 == 1000 + 0x234);
    assert(state.body[0].type == 4000 + 1);
    assert(state.body[0].level == 9);
    assert(state.body[0].option1 == 1);
    assert(state.body[0].ext_option == 1);
    assert(state.extend_state == 1);

    // Regression: all five armor 0x1FF sentinels fall back to class body models,
    // and both weapon 0xFFF sentinels clear the weapon slots.
    auto sentinel = e;
    sentinel[0] = 0xFF;
    sentinel[1] = 0xFF;
    sentinel[2] = 0xFF;
    sentinel[3] = 0xFF;
    sentinel[4] = 0xFF;
    sentinel[8] = 0xF8;
    sentinel[11] = 0xF0;
    sentinel[12] = 0xFF;
    sentinel[13] = 0xFF;
    sentinel[14] = 0xFF;

    newera::equipment_apply::State fallback_state;
    assert(newera::equipment_apply::Apply(sentinel, bases, 2, fallback_state, error));
    assert(fallback_state.weapon0 == -1);
    assert(fallback_state.weapon1 == -1);
    const std::array<int, 5> expected_fallback = {9002, 9102, 9202, 9302, 9402};
    for (int i = 0; i < 5; ++i) {
        assert(fallback_state.body[i].type == expected_fallback[i]);
    }

    std::cout << "TS-9 F3:13 equipment/model apply loopback: PASS\n";
    return 0;
}
