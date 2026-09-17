#include "../combat_action_skill_rx.h"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
using socket_t = SOCKET;
static constexpr socket_t BAD = INVALID_SOCKET;
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
using socket_t = int;
static constexpr socket_t BAD = -1;
#endif

static void Close(socket_t s) {
#ifdef _WIN32
    closesocket(s);
#else
    close(s);
#endif
}

static bool Xfer(socket_t s, uint8_t* data, std::size_t size, bool tx) {
    std::size_t done = 0;
    while (done < size) {
#ifdef _WIN32
        const int result = tx
            ? send(s, reinterpret_cast<const char*>(data + done),
                   static_cast<int>(size - done), 0)
            : recv(s, reinterpret_cast<char*>(data + done),
                   static_cast<int>(size - done), 0);
#else
        const ssize_t result = tx
            ? send(s, data + done, size - done, 0)
            : recv(s, data + done, size - done, 0);
#endif
        if (result <= 0) {
            return false;
        }
        done += static_cast<std::size_t>(result);
    }
    return true;
}

static std::vector<uint8_t> ActionGolden() {
    return {0xC1, 0x09, 0x18, 0x01, 0x23, 0x05, 0x78, 0x04, 0x56};
}

static std::vector<uint8_t> SkillGolden() {
    return {0xC1, 0x09, 0x19, 0x00, 0x42, 0x01, 0x23, 0x04, 0x56};
}

int main() {
#ifdef _WIN32
    WSADATA wsa{};
    assert(WSAStartup(MAKEWORD(2, 2), &wsa) == 0);
#endif

    socket_t listener = socket(AF_INET, SOCK_STREAM, 0);
    assert(listener != BAD);

    int reuse = 1;
#ifdef _WIN32
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR,
               reinterpret_cast<const char*>(&reuse), sizeof(reuse));
#else
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
#endif

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = 0;
    assert(bind(listener, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == 0);
    assert(listen(listener, 1) == 0);

#ifdef _WIN32
    int address_length = sizeof(address);
#else
    socklen_t address_length = sizeof(address);
#endif
    assert(getsockname(listener, reinterpret_cast<sockaddr*>(&address), &address_length) == 0);

    const auto action = ActionGolden();
    const auto skill = SkillGolden();

    std::thread server([&]() {
        socket_t peer = accept(listener, nullptr, nullptr);
        assert(peer != BAD);

        uint8_t trigger = 0;
        assert(Xfer(peer, &trigger, 1, false));
        assert(Xfer(peer, const_cast<uint8_t*>(action.data()), action.size(), true));
        assert(Xfer(peer, const_cast<uint8_t*>(skill.data()), skill.size(), true));

        Close(peer);
    });

    socket_t client = socket(AF_INET, SOCK_STREAM, 0);
    assert(client != BAD);
    assert(connect(client, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == 0);

    uint8_t trigger = 0xA5;
    assert(Xfer(client, &trigger, 1, true));

    std::vector<uint8_t> action_wire(action.size());
    std::vector<uint8_t> skill_wire(skill.size());
    assert(Xfer(client, action_wire.data(), action_wire.size(), false));
    assert(Xfer(client, skill_wire.data(), skill_wire.size(), false));

    newera::combat_rx::ActionEvent action_event{};
    newera::combat_rx::SkillEvent skill_event{};
    std::string error;

    assert(newera::combat_rx::ParseActionC1(
        action_wire.data(), action_wire.size(), action_event, error));
    assert(action_event.key == 0x0123);
    assert(action_event.angle == 0x05);
    assert(action_event.action == 0x78);
    assert(action_event.target == 0x0456);

    assert(newera::combat_rx::ParseSkillC1(
        skill_wire.data(), skill_wire.size(), skill_event, error));
    assert(skill_event.magic == 0x0042);
    assert(skill_event.source == 0x0123);
    assert(skill_event.target == 0x0456);

    uint8_t truncated[8]{};
    std::memcpy(truncated, action_wire.data(), sizeof(truncated));
    assert(!newera::combat_rx::ParseActionC1(
        truncated, sizeof(truncated), action_event, error));

    Close(client);
    server.join();
    Close(listener);

#ifdef _WIN32
    WSACleanup();
#endif

    std::puts("TS-8 combat action/skill RX loopback: PASS");
    return 0;
}
