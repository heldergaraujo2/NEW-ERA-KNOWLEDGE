#include "../f1_02_logout_request.h"

#include <cassert>
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
        const int rc = sendMode
            ? send(s, reinterpret_cast<const char*>(p + done), static_cast<int>(n - done), 0)
            : recv(s, reinterpret_cast<char*>(p + done), static_cast<int>(n - done), 0);
#else
        const int rc = sendMode
            ? static_cast<int>(send(s, p + done, n - done, 0))
            : static_cast<int>(recv(s, p + done, n - done, 0));
#endif
        if (rc <= 0) {
            return false;
        }
        done += static_cast<std::size_t>(rc);
    }
    return true;
}

static std::string Hex(const uint8_t* p, std::size_t n) {
    char b[3];
    std::string s;
    for (std::size_t i = 0; i < n; ++i) {
        std::snprintf(b, sizeof(b), "%02x", p[i]);
        s += b;
    }
    return s;
}

int main() {
    newera::crypto::PacketCryptoSM::Keys keys{};
    for (int i = 0; i < 4; ++i) {
        keys.modulus[i] = 65521;
        keys.key[i] = 3;
        keys.xor_[i] = 7;
    }

    newera::crypto::PacketCryptoSM sm;
    sm.SetKeys(keys);

    newera::login::LogoutRequestInput input{0x5A};
    std::vector<uint8_t> logical;
    std::string error;

    assert(newera::login::BuildLogicalC1(input, logical, error));
    assert(logical.size() == 5);
    assert(logical[0] == 0xC1 && logical[1] == 0x05 && logical[2] == 0xF1);
    assert(logical[3] == 0x7A && logical[4] == 0x9C);

    std::vector<uint8_t> frame;
    assert(newera::login::BuildC3(input, sm, frame, error));
    assert(frame.size() == 13);
    assert(frame[0] == 0xC3 && frame[1] == 0x0D);

    newera::crypto::PacketCryptoSM::Keys decryptKeys = keys;
    for (int i = 0; i < 4; ++i) {
        decryptKeys.key[i] = 43681; // 3^-1 mod 65521, deterministic test-key inverse.
    }
    newera::crypto::PacketCryptoSM dec;
    dec.SetKeys(decryptKeys);
    uint8_t decoded[8] = {};
    assert(dec.Decrypt(decoded, frame.data() + 2, 11) == 5);
    assert(std::memcmp(decoded, logical.data(), logical.size()) == 0);

    socket_t listener = socket(AF_INET, SOCK_STREAM, 0);
    assert(listener != kBad);

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

    std::thread server([&]() {
        socket_t peer = accept(listener, nullptr, nullptr);
        assert(peer != kBad);
        uint8_t received[13] = {};
        assert(Xfer(peer, received, sizeof(received), false));
        assert(std::memcmp(received, frame.data(), frame.size()) == 0);
        CloseSocket(peer);
    });

    socket_t client = socket(AF_INET, SOCK_STREAM, 0);
    assert(client != kBad);
    assert(connect(client, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) == 0);
    assert(Xfer(client, frame.data(), frame.size(), true));
    CloseSocket(client);

    server.join();
    CloseSocket(listener);

    std::printf("logical=%s\n", Hex(logical.data(), logical.size()).c_str());
    std::printf("c3=%s\n", Hex(frame.data(), frame.size()).c_str());
    std::puts("TS-14 F1:02 logout request C3 TCP loopback: PASS");
    return 0;
}
