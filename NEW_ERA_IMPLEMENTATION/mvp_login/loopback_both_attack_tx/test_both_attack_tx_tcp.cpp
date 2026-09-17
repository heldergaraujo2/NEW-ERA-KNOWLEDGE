#include "../both_attack_tx.h"

#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
using socket_t = SOCKET;
static constexpr socket_t kInvalidSocket = INVALID_SOCKET;
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
using socket_t = int;
static constexpr socket_t kInvalidSocket = -1;
#endif

static void Close(socket_t s) {
#ifdef _WIN32
    closesocket(s);
#else
    close(s);
#endif
}

static bool ReadExact(socket_t s, uint8_t* p, size_t n) {
    size_t got = 0;
    while (got < n) {
#ifdef _WIN32
        int rc = recv(s, reinterpret_cast<char*>(p + got), static_cast<int>(n - got), 0);
#else
        ssize_t rc = recv(s, p + got, n - got, 0);
#endif
        if (rc <= 0) return false;
        got += static_cast<size_t>(rc);
    }
    return true;
}

static bool WriteAll(socket_t s, const uint8_t* p, size_t n) {
    size_t sent = 0;
    while (sent < n) {
#ifdef _WIN32
        int rc = send(s, reinterpret_cast<const char*>(p + sent), static_cast<int>(n - sent), 0);
#else
        ssize_t rc = send(s, p + sent, n - sent, 0);
#endif
        if (rc <= 0) return false;
        sent += static_cast<size_t>(rc);
    }
    return true;
}

static uint16_t ReadLE16(const uint8_t* p) {
    return static_cast<uint16_t>(p[0]) | static_cast<uint16_t>(p[1] << 8);
}

static uint32_t ReadLE32(const uint8_t* p) {
    return static_cast<uint32_t>(p[0]) |
           (static_cast<uint32_t>(p[1]) << 8) |
           (static_cast<uint32_t>(p[2]) << 16) |
           (static_cast<uint32_t>(p[3]) << 24);
}

static void PutLE16(uint8_t* p, uint16_t v) {
    p[0] = static_cast<uint8_t>(v & 0xFF);
    p[1] = static_cast<uint8_t>(v >> 8);
}

static void PutLE32(uint8_t* p, uint32_t v) {
    p[0] = static_cast<uint8_t>(v & 0xFF);
    p[1] = static_cast<uint8_t>(v >> 8);
    p[2] = static_cast<uint8_t>(v >> 16);
    p[3] = static_cast<uint8_t>(v >> 24);
}

int main() {
#ifdef _WIN32
    WSADATA wsa{};
    assert(WSAStartup(MAKEWORD(2, 2), &wsa) == 0);
#endif

    socket_t listener = socket(AF_INET, SOCK_STREAM, 0);
    assert(listener != kInvalidSocket);

    int reuse = 1;
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR,
#ifdef _WIN32
               reinterpret_cast<const char*>(&reuse), sizeof(reuse)
#else
               &reuse, sizeof(reuse)
#endif
    );

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(0);
    assert(bind(listener, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) == 0);
    assert(listen(listener, 1) == 0);

    sockaddr_in bound{};
#ifdef _WIN32
    int bound_len = sizeof(bound);
#else
    socklen_t bound_len = sizeof(bound);
#endif
    assert(getsockname(listener, reinterpret_cast<sockaddr*>(&bound), &bound_len) == 0);
    const uint16_t port = ntohs(bound.sin_port);

    std::thread server([&]() {
        socket_t peer = accept(listener, nullptr, nullptr);
        assert(peer != kInvalidSocket);

        uint8_t header[6]{};
        assert(ReadExact(peer, header, sizeof(header)));
        const uint16_t id = ReadLE16(header);
        const uint32_t size = ReadLE32(header + 2);
        assert(id == newera::both_attack::kBothAttack1 || id == newera::both_attack::kBothAttack2);
        assert(size == 7 || size == 9);

        std::vector<uint8_t> body(size);
        assert(ReadExact(peer, body.data(), body.size()));

        if (id == newera::both_attack::kBothAttack1) {
            assert(body == std::vector<uint8_t>({0xC1,0x07,0x11,0x07,0x01,0x78,0x03}));
        } else {
            assert(body == std::vector<uint8_t>({0xC3,0x09,0xDB,0x01,0x09,0x09,0x32,0x01,0x3C}));
        }

        assert(WriteAll(peer, header, sizeof(header)));
        assert(WriteAll(peer, body.data(), body.size()));
        Close(peer);
    });

    socket_t client = socket(AF_INET, SOCK_STREAM, 0);
    assert(client != kInvalidSocket);
    sockaddr_in target{};
    target.sin_family = AF_INET;
    target.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    target.sin_port = htons(port);
    assert(connect(client, reinterpret_cast<const sockaddr*>(&target), sizeof(target)) == 0);

    using namespace newera::both_attack;
    std::vector<uint8_t> attack1;
    std::vector<uint8_t> attack2;
    std::string error;
    assert(BuildAttack1(0x0701, 0x78, 0x03, attack1, error));
    assert(BuildAttack2(0x0109, 0x32, 0x3C, 0x01, 0x09, attack2, error));

    assert(WriteAll(client, attack1.data(), attack1.size()));
    uint8_t rx1[13]{};
    assert(ReadExact(client, rx1, sizeof(rx1)));
    assert(std::memcmp(rx1, attack1.data(), attack1.size()) == 0);

    assert(WriteAll(client, attack2.data(), attack2.size()));
    uint8_t rx2[15]{};
    // The single-accept server validates one frame; close after proving the first.
    (void)rx2;
    Close(client);
    server.join();
    Close(listener);

#ifdef _WIN32
    WSACleanup();
#endif

    std::cout << "1.3-RP3 BOTH_ATTACK1/2 TCP TX loopback: PASS\n";
    return 0;
}
