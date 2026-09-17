#include "../mvp_login/f3_13_equipment_charset.h"

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
static constexpr socket_t kBad = INVALID_SOCKET;
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
using socket_t = int;
static constexpr socket_t kBad = -1;
#endif

static void CloseSocket(socket_t s) {
#ifdef _WIN32
    closesocket(s);
#else
    close(s);
#endif
}

static bool Xfer(socket_t s, uint8_t* p, std::size_t n, bool sendMode) {
    std::size_t done = 0;
    while (done < n) {
#ifdef _WIN32
        int rc = sendMode ? send(s, reinterpret_cast<const char*>(p + done), static_cast<int>(n - done), 0)
                          : recv(s, reinterpret_cast<char*>(p + done), static_cast<int>(n - done), 0);
#else
        int rc = sendMode ? static_cast<int>(send(s, p + done, n - done, 0))
                          : static_cast<int>(recv(s, p + done, n - done, 0));
#endif
        if (rc <= 0) return false;
        done += static_cast<std::size_t>(rc);
    }
    return true;
}

static void LE16(uint8_t* p, uint16_t v) {
    p[0] = static_cast<uint8_t>(v);
    p[1] = static_cast<uint8_t>(v >> 8);
}
static void LE32(uint8_t* p, uint32_t v) {
    p[0] = static_cast<uint8_t>(v);
    p[1] = static_cast<uint8_t>(v >> 8);
    p[2] = static_cast<uint8_t>(v >> 16);
    p[3] = static_cast<uint8_t>(v >> 24);
}
static uint16_t RLE16(const uint8_t* p) { return static_cast<uint16_t>(p[0] | (p[1] << 8)); }
static uint32_t RLE32(const uint8_t* p) { return static_cast<uint32_t>(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24)); }

static std::vector<uint8_t> WrapBoth(const std::vector<uint8_t>& classic) {
    std::vector<uint8_t> out(6 + classic.size());
    LE16(out.data(), 0x000C);
    LE32(out.data() + 2, static_cast<uint32_t>(classic.size()));
    std::memcpy(out.data() + 6, classic.data(), classic.size());
    return out;
}

int main() {
#ifdef _WIN32
    WSADATA wsa{};
    assert(WSAStartup(MAKEWORD(2, 2), &wsa) == 0);
#endif

    socket_t listener = socket(AF_INET, SOCK_STREAM, 0);
    assert(listener != kBad);
    int yes = 1;
#ifdef _WIN32
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&yes), sizeof(yes));
#else
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
#endif
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = 0;
    assert(bind(listener, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) == 0);
    assert(listen(listener, 1) == 0);
#ifdef _WIN32
    int addrLen = sizeof(addr);
#else
    socklen_t addrLen = sizeof(addr);
#endif
    assert(getsockname(listener, reinterpret_cast<sockaddr*>(&addr), &addrLen) == 0);

    std::array<uint8_t, newera::f3_13::kCharSetSize> charset{};
    for (std::size_t i = 0; i < charset.size(); ++i) charset[i] = static_cast<uint8_t>(0xA0 + i);
    const auto classic = newera::f3_13::Build(0x0123, charset);
    const auto modern = WrapBoth(classic);
    assert(modern.size() == 30);
    assert(RLE16(modern.data()) == 0x000C);
    assert(RLE32(modern.data() + 2) == 24);
    assert(std::equal(classic.begin(), classic.end(), modern.begin() + 6));

    std::thread server([&]() {
        socket_t peer = accept(listener, nullptr, nullptr);
        assert(peer != kBad);
        std::array<uint8_t, 6> hdr{};
        assert(Xfer(peer, hdr.data(), hdr.size(), false));
        assert(RLE16(hdr.data()) == 0x000C);
        assert(RLE32(hdr.data() + 2) == 24);
        std::vector<uint8_t> body(24);
        assert(Xfer(peer, body.data(), body.size(), false));
        assert(body == classic);
        assert(Xfer(peer, modern.data(), modern.size(), true));
        CloseSocket(peer);
    });

    socket_t client = socket(AF_INET, SOCK_STREAM, 0);
    assert(client != kBad);
    assert(connect(client, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) == 0);
    auto tx = modern;
    assert(Xfer(client, tx.data(), tx.size(), true));

    std::array<uint8_t, 6> rxHdr{};
    assert(Xfer(client, rxHdr.data(), rxHdr.size(), false));
    assert(RLE16(rxHdr.data()) == 0x000C);
    assert(RLE32(rxHdr.data() + 2) == 24);
    std::vector<uint8_t> rxBody(24);
    assert(Xfer(client, rxBody.data(), rxBody.size(), false));
    assert(rxBody == classic);

    newera::f3_13::EquipmentCharSet parsed{};
    std::string error;
    assert(newera::f3_13::Parse(rxBody, parsed, error));
    assert(parsed.index == 0x0123);
    assert(parsed.charset == charset);

    // Regression checks: BOTH_MESSAGE id/size and the inner C1 frame remain distinct layers.
    auto bad = modern;
    bad[0] = 0x0D;
    assert(RLE16(bad.data()) != 0x000C);
    bad = modern;
    bad[2] = 23;
    assert(RLE32(bad.data() + 2) != 24);
    bad.assign(classic.begin(), classic.end());
    bad[1] = 23;
    assert(!newera::f3_13::Parse(bad, parsed, error));

    CloseSocket(client);
    server.join();
    CloseSocket(listener);
#ifdef _WIN32
    WSACleanup();
#endif
    std::puts("TS-4 F3:13 BOTH_MESSAGE loopback: PASS");
    return 0;
}
