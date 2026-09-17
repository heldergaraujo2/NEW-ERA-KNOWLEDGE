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

static void LE16(uint8_t* p, uint16_t v) { p[0] = static_cast<uint8_t>(v); p[1] = static_cast<uint8_t>(v >> 8); }
static void LE32(uint8_t* p, uint32_t v) {
    p[0] = static_cast<uint8_t>(v); p[1] = static_cast<uint8_t>(v >> 8);
    p[2] = static_cast<uint8_t>(v >> 16); p[3] = static_cast<uint8_t>(v >> 24);
}
static uint16_t R16(const uint8_t* p) { return static_cast<uint16_t>(p[0] | (p[1] << 8)); }
static uint32_t R32(const uint8_t* p) { return static_cast<uint32_t>(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24)); }

static std::vector<uint8_t> Envelope(const std::vector<uint8_t>& classic) {
    std::vector<uint8_t> out(6 + classic.size());
    LE16(out.data(), 0x000C);
    LE32(out.data() + 2, static_cast<uint32_t>(classic.size()));
    std::memcpy(out.data() + 6, classic.data(), classic.size());
    return out;
}

static void AssertEnvelope(const std::vector<uint8_t>& frame) {
    assert(frame.size() >= 6);
    assert(R16(frame.data()) == 0x000C);
    assert(R32(frame.data() + 2) == frame.size() - 6);
    const uint8_t* body = frame.data() + 6;
    assert(body[0] == 0xC1);
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
    const auto f313 = newera::f3_13::Build(0x0123, charset);
    const auto scripted = Envelope(f313);
    AssertEnvelope(scripted);
    assert(scripted.size() == 30);
    assert(scripted[6] == 0xC1 && scripted[7] == 0x18 && scripted[8] == 0xF3 && scripted[9] == 0x13);

    std::thread server([&]() {
        socket_t peer = accept(listener, nullptr, nullptr);
        assert(peer != kBad);

        // Consume one client packet to model TS-2's trigger point.
        std::array<uint8_t, 6> triggerHdr{};
        assert(Xfer(peer, triggerHdr.data(), triggerHdr.size(), false));
        const uint32_t triggerSize = R32(triggerHdr.data() + 2);
        assert(R16(triggerHdr.data()) == 0x000C);
        std::vector<uint8_t> trigger(triggerSize);
        if (!trigger.empty()) assert(Xfer(peer, trigger.data(), trigger.size(), false));

        // TS-5 scripted sequence: existing F3 family responses plus the new F3:13 closure.
        for (int i = 0; i < 3; ++i) assert(Xfer(peer, const_cast<uint8_t*>(scripted.data()), scripted.size(), true));
        CloseSocket(peer);
    });

    socket_t client = socket(AF_INET, SOCK_STREAM, 0);
    assert(client != kBad);
    assert(connect(client, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) == 0);
    std::vector<uint8_t> trigger = Envelope(std::vector<uint8_t>{0xC1, 0x04, 0xF1, 0x00});
    assert(Xfer(client, trigger.data(), trigger.size(), true));

    for (int i = 0; i < 3; ++i) {
        std::array<uint8_t, 6> hdr{};
        assert(Xfer(client, hdr.data(), hdr.size(), false));
        assert(R16(hdr.data()) == 0x000C);
        assert(R32(hdr.data() + 2) == 24);
        std::vector<uint8_t> body(24);
        assert(Xfer(client, body.data(), body.size(), false));
        assert(body == f313);
        newera::f3_13::EquipmentCharSet parsed{};
        std::string error;
        assert(newera::f3_13::Parse(body, parsed, error));
        assert(parsed.index == 0x0123);
        assert(parsed.charset == charset);
    }

    CloseSocket(client);
    server.join();
    CloseSocket(listener);
#ifdef _WIN32
    WSACleanup();
#endif
    std::puts("TS-5 scripted F3:13 sequence: PASS");
    return 0;
}
