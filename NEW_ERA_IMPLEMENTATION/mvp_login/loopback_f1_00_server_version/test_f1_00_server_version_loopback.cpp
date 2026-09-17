#include "../f1_00_server_version.h"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
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
        const int n = tx ? send(s, reinterpret_cast<const char*>(data + done), static_cast<int>(size - done), 0)
                         : recv(s, reinterpret_cast<char*>(data + done), static_cast<int>(size - done), 0);
#else
        const ssize_t n = tx ? send(s, data + done, size - done, 0)
                             : recv(s, data + done, size - done, 0);
#endif
        if (n <= 0) return false;
        done += static_cast<std::size_t>(n);
    }
    return true;
}

int main() {
#ifdef _WIN32
    WSADATA wsa{};
    assert(WSAStartup(MAKEWORD(2, 2), &wsa) == 0);
#endif

    const uint8_t version[5] = {'5','0','2','A','B'};
    std::vector<uint8_t> golden;
    assert(newera::login_rx::BuildServerVersionC1(0x01, 0x1234, version, golden));
    const std::vector<uint8_t> expected = {
        0xC1,0x0C,0xF1,0x00,0x01,0x12,0x34,0x35,0x30,0x32,0x41,0x42};
    assert(golden == expected);

    socket_t listener = socket(AF_INET, SOCK_STREAM, 0);
    assert(listener != BAD);
    int reuse = 1;
#ifdef _WIN32
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&reuse), sizeof(reuse));
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

    std::thread server([&]() {
        socket_t peer = accept(listener, nullptr, nullptr);
        assert(peer != BAD);
        std::vector<uint8_t> received(golden.size());
        assert(Xfer(peer, received.data(), received.size(), false));
        assert(received == golden);
        assert(Xfer(peer, received.data(), received.size(), true));
        Close(peer);
    });

    socket_t client = socket(AF_INET, SOCK_STREAM, 0);
    assert(client != BAD);
    assert(connect(client, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == 0);
    assert(Xfer(client, golden.data(), golden.size(), true));

    std::vector<uint8_t> echoed(golden.size());
    assert(Xfer(client, echoed.data(), echoed.size(), false));
    assert(echoed == expected);

    newera::login_rx::ServerVersion parsed{};
    std::string error;
    assert(newera::login_rx::ParseServerVersionC1(echoed.data(), echoed.size(), parsed, error));
    assert(parsed.result == 0x01);
    assert(parsed.hero_key == 0x1234);
    assert(std::memcmp(parsed.version, version, 5) == 0);

    uint8_t truncated[11]{};
    std::memcpy(truncated, echoed.data(), sizeof(truncated));
    assert(!newera::login_rx::ParseServerVersionC1(truncated, sizeof(truncated), parsed, error));

    Close(client);
    server.join();
    Close(listener);

#ifdef _WIN32
    WSACleanup();
#endif

    std::puts("1.3-T F1:00 ServerVersion TCP loopback: PASS");
    return 0;
}
