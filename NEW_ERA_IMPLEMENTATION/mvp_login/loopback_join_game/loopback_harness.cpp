// NEW-ERA — loopback_harness.cpp (1.3-C) — JOIN GAME — ASIO TX + CLASSIC C1 F3:03 RX
// Prova end-to-end LOCAL com socket REAL — SOMENTE 127.0.0.1 (hardcode; sem args).
//   SERVER stub (GS modern bridge): recebe REQ olc::net 16 B ([id:u16=0x000B]
//                [size:u32=10][ID10]) -> memcmp vs kReqExpected -> envia RESP
//                golden C1 66 B (PRECEIVE_JOIN_MAP_SERVER; espelho da ponte
//                SocketManagerModern.cpp :122-:131 que traduz BOTH_CONNECT_
//                JOIN_GAME -> classico F3:03) -> close.
//   CLIENT MVP : envia REQ do builder WIRE REAL (BuildAsio_BOTH_CONNECT_
//                JOIN_GAME_Request) -> recebe C1 (hdr 2 B -> resto) -> decode
//                (C1 sem crypto) -> ParseC1_F3_03_JoinMapServerResponsePlain
//                com asserts (map/x/y/angle/gold/exp) .
#include "embedded_vectors.h"
#include "mvp_login_client.cpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <atomic>
#include <cstdio>
#include <cstring>
#include <future>
#include <string>
#include <thread>
#include <vector>

using namespace newera;

static bool SendAll(int fd, const uint8_t* p, size_t n) {
    size_t off = 0;
    while (off < n) { ssize_t w = ::send(fd, p + off, n - off, 0);
                      if (w <= 0) return false; off += (size_t)w; }
    return true;
}
static bool RecvAll(int fd, uint8_t* p, size_t n) {
    size_t off = 0;
    while (off < n) { ssize_t r = ::recv(fd, p + off, n - off, 0);
                      if (r <= 0) return false; off += (size_t)r; }
    return true;
}

// ---------------- SERVER STUB (GS modern bridge) ----------------
static std::atomic<bool> g_serverOk{ false };

static void ServerThread(std::promise<int> portPromise) {
    int ls = ::socket(AF_INET, SOCK_STREAM, 0);
    if (ls < 0) { portPromise.set_value(-1); return; }
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);   // SOMENTE 127.0.0.1
    addr.sin_port = 0;                               // porta efêmera
    if (::bind(ls, (sockaddr*)&addr, sizeof(addr)) != 0 || ::listen(ls, 1) != 0) {
        portPromise.set_value(-1); ::close(ls); return;
    }
    socklen_t alen = sizeof(addr);
    ::getsockname(ls, (sockaddr*)&addr, &alen);
    portPromise.set_value(ntohs(addr.sin_port));

    int c = ::accept(ls, nullptr, nullptr);
    ::close(ls);
    if (c < 0) return;

    // 1) recebe o frame olc::net do client (16 B exatos — NÃO é C1)
    uint8_t buf[16];
    if (!RecvAll(c, buf, sizeof buf)) { printf("[server] request incompleto\n"); ::close(c); return; }
    if (std::memcmp(buf, loopback_join::kReqExpected.data(), loopback_join::kReqSize) != 0) {
        size_t diff = loopback_join::kReqSize;
        for (size_t i = 0; i < loopback_join::kReqSize; ++i)
            if (buf[i] != loopback_join::kReqExpected[i]) { diff = i; break; }
        printf("[server] REQUEST ASIO DIVERGENTE do golden (1o offset: %zu)\n", diff);
        ::close(c);
        return;
    }
    printf("[server] matched REQ asio golden 16 B (id=0x000B, memcmp OK)\n");

    // 2) envia o RESP classico C1 66 B (golden)
    printf("[server] enviando RESP golden C1 F3:03 (%zu B)\n", loopback_join::kRespSize);
    if (!SendAll(c, loopback_join::kRespServer.data(), loopback_join::kRespSize)) {
        ::close(c); return;
    }
    ::close(c);
    g_serverOk = true;
}

// ---------------- CLIENT MVP ----------------
static bool ClientThread(int port) {
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) return false;
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);   // SOMENTE 127.0.0.1
    addr.sin_port = htons((uint16_t)port);
    if (::connect(fd, (sockaddr*)&addr, sizeof(addr)) != 0) { ::close(fd); return false; }

    // 1) envia REQ olc::net 16 B com o builder WIRE REAL (id do golden)
    auto req = mvp::BuildAsio_BOTH_CONNECT_JOIN_GAME_Request(loopback_join::kExpectedId);
    if (req.empty()) { printf("[client] builder falhou (bounds)\n"); ::close(fd); return false; }
    printf("[client] sent REQ asio wire-real (%zu B)\n", req.size());
    if (!SendAll(fd, req.data(), req.size())) { ::close(fd); return false; }

    // 2) recebe RESP C1: hdr 2 B ([C1][size]) -> resto
    uint8_t hdr[2];
    if (!RecvAll(fd, hdr, 2) || hdr[0] != 0xC1 || hdr[1] != loopback_join::kRespSize) {
        printf("[client] header C1 invalido\n"); ::close(fd); return false;
    }
    std::vector<uint8_t> resp(loopback_join::kRespSize);
    resp[0] = hdr[0]; resp[1] = hdr[1];
    if (!RecvAll(fd, resp.data() + 2, loopback_join::kRespSize - 2)) {
        printf("[client] response incompleto\n"); ::close(fd); return false;
    }

    // 3) decode C1 (sem crypto; streamXored=false) + parse F3:03
    crypto::PacketCryptoSM smAny;  // C1: sem crypto (check comentado :873-:880)
    const uint8_t ver[5] = { 1, 2, 3, 4, 5 };
    mvp::ParsedMvp out; std::string err; std::vector<uint8_t> plainC1;
    if (!mvp::DecodeAndParseMvpPacket(resp, smAny, ver, out, err, /*streamXored=*/false, &plainC1) ||
        out.head != 0xF3 || out.sub != 0x03) {
        printf("[client] decode RESP falhou: %s\n", err.c_str()); ::close(fd); return false;
    }
    mvp::ParsedJoinMapServer jm; std::string perr;
    if (!mvp::ParseC1_F3_03_JoinMapServerResponsePlain(plainC1, jm, perr) ||
        (int)jm.map != loopback_join::kExpectedMap ||
        (int)jm.x   != loopback_join::kExpectedX ||
        (int)jm.y   != loopback_join::kExpectedY ||
        jm.angleDeg != loopback_join::kExpectedAngleDeg ||
        jm.gold     != loopback_join::kExpectedGold ||
        jm.exp      != loopback_join::kExpectedExp) {
        printf("[client] parse divergente do expected_parse (%s)\n", perr.c_str());
        ::close(fd); return false;
    }
    printf("[client] parsed RESP C1 F3:03: map=%u x=%u y=%u angle=%ddeg gold=%u exp=0x%016llX\n",
           jm.map, jm.x, jm.y, jm.angleDeg, jm.gold, (unsigned long long)jm.exp);

    ::close(fd);
    return true;
}

int main() {
    std::promise<int> portPromise;
    auto portFuture = portPromise.get_future();
    std::thread server(ServerThread, std::move(portPromise));
    const int port = portFuture.get();
    if (port < 0) { server.join(); printf("FALHA: bind/listen loopback\n"); return 2; }
    printf("[harness] server stub em 127.0.0.1:%d (porta efemera)\n", port);

    bool cli = ClientThread(port);
    server.join();
    if (!cli)        { printf("FALHA: client\n"); return 3; }
    if (!g_serverOk) { printf("FALHA: server\n"); return 4; }
    printf("LOOPBACK JOIN_GAME OK: REQ-asio-16B(match)->RESP-C1-66B(map/x/y/angle/gold/exp)\n");
    return 0;
}
