#include "../f3_13_equipment_charset.h"

#include <array>
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
static constexpr socket_t kBadSocket = INVALID_SOCKET;
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
using socket_t = int;
static constexpr socket_t kBadSocket = -1;
#endif

static void CloseSocket(socket_t s) {
#ifdef _WIN32
    closesocket(s);
#else
    close(s);
#endif
}

static bool Transfer(socket_t s, void* buffer, std::size_t size, bool sendMode) {
    auto* p = static_cast<uint8_t*>(buffer);
    std::size_t done = 0;
    while (done < size) {
#ifdef _WIN32
        int rc = sendMode
            ? send(s, reinterpret_cast<const char*>(p + done), static_cast<int>(size - done), 0)
            : recv(s, reinterpret_cast<char*>(p + done), static_cast<int>(size - done), 0);
#else
        int rc = sendMode
            ? static_cast<int>(send(s, p + done, size - done, 0))
            : static_cast<int>(recv(s, p + done, size - done, 0));
#endif
        if (rc <= 0) return false;
        done += static_cast<std::size_t>(rc);
    }
    return true;
}

int main() {
#ifdef _WIN32
    WSADATA wsa{};
    assert(WSAStartup(MAKEWORD(2, 2), &wsa) == 0);
#endif

    socket_t listener = socket(AF_INET, SOCK_STREAM, 0);
    assert(listener != kBadSocket);

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = 0;
    assert(bind(listener, reinterpret_cast<const sockaddr*>(&address), sizeof(address)) == 0);
    assert(listen(listener, 1) == 0);

    socklen_t addressSize = sizeof(address);
    assert(getsockname(listener, reinterpret_cast<sockaddr*>(&address), &addressSize) == 0);

    std::array<uint8_t, newera::f3_13::kCharSetSize> charset{};
    for (std::size_t i = 0; i < charset.size(); ++i) {
        charset[i] = static_cast<uint8_t>(0xA0 + i);
    }

    const auto golden = newera::f3_13::Build(0x0123, charset);
    assert(golden.size() == 24);
    assert(golden[0] == 0xC1 && golden[1] == 0x18);
    assert(golden[2] == 0xF3 && golden[3] == 0x13);
    assert(golden[4] == 0x01 && golden[5] == 0x23);

    std::thread server([&]() {
        socket_t peer = accept(listener, nullptr, nullptr);
        assert(peer != kBadSocket);

        std::array<uint8_t, newera::f3_13::kFrameSize> received{};
        assert(Transfer(peer, received.data(), received.size(), false));
        assert(std::memcmp(received.data(), golden.data(), golden.size()) == 0);
        assert(Transfer(peer, received.data(), received.size(), true));

        CloseSocket(peer);
    });

    socket_t client = socket(AF_INET, SOCK_STREAM, 0);
    assert(client != kBadSocket);
    assert(connect(client, reinterpret_cast<const sockaddr*>(&address), sizeof(address)) == 0);
    assert(Transfer(client, const_cast<uint8_t*>(golden.data()), golden.size(), true));

    std::array<uint8_t, newera::f3_13::kFrameSize> echoed{};
    assert(Transfer(client, echoed.data(), echoed.size(), false));
    assert(std::memcmp(echoed.data(), golden.data(), golden.size()) == 0);

    newera::f3_13::EquipmentCharSet parsed{};
    std::string error;
    assert(newera::f3_13::Parse(
        std::vector<uint8_t>(echoed.begin(), echoed.end()), parsed, error));
    assert(parsed.index == 0x0123);
    assert(parsed.charset == charset);

    // Negative checks: size and subcode are part of the normative framing.
    auto bad = golden;
    bad[1] = 23;
    assert(!newera::f3_13::Parse(bad, parsed, error));
    bad = golden;
    bad[3] = 0x12;
    assert(!newera::f3_13::Parse(bad, parsed, error));

    CloseSocket(client);
    server.join();
    CloseSocket(listener);

#ifdef _WIN32
    WSACleanup();
#endif

    std::puts("F3:13 TCP loopback: PASS");
    return 0;
}
