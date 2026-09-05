// NEW-ERA — loopback_harness.cpp (1.3-A) — F3:0x06 Add Point — WIRE-REAL C1
// Prova end-to-end LOCAL com socket REAL — SOMENTE 127.0.0.1 (hardcode; sem args).
//   SERVER stub: envia kRespServer (golden C1 11 B) -> recebe request C1 PLAIN
//                (wire real 5 B) -> memcmp vs kReqExpected -> close.
//   CLIENT MVP : decodifica response C1 (sem decrypt) -> ParseC1_F3_06 com
//                asserts (ok/stat/max/shield/skill) -> envia request do builder
//                WIRE REAL (BuildC1_F3_06_AddPointRequestWire).
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

// ---------------- SERVER STUB ----------------
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

    printf("[server] conectado; enviando RESP golden C1 (%zu B)\n", loopback_f3a::kRespSize);
    if (!SendAll(c, loopback_f3a::kRespServer.data(), loopback_f3a::kRespSize)) { ::close(c); return; }

    uint8_t hdr[2];
    if (!RecvAll(c, hdr, 2) || hdr[0] != 0xC1) { printf("[server] header invalido (espera C1 wire real)\n"); ::close(c); return; }
    const size_t rest = (size_t)hdr[1] - 2;
    std::vector<uint8_t> pkt((size_t)hdr[1]);
    pkt[0] = hdr[0]; pkt[1] = hdr[1];
    if (rest && !RecvAll(c, pkt.data() + 2, rest)) { printf("[server] request incompleto\n"); ::close(c); return; }

    if (pkt.size() != loopback_f3a::kReqSize ||
        std::memcmp(pkt.data(), loopback_f3a::kReqExpected.data(), loopback_f3a::kReqSize) != 0) {
        size_t n = pkt.size() < loopback_f3a::kReqSize ? pkt.size() : loopback_f3a::kReqSize;
        size_t diff = n;
        for (size_t i = 0; i < n; ++i) if (pkt[i] != loopback_f3a::kReqExpected[i]) { diff = i; break; }
        printf("[server] REQUEST DIVERGENTE do golden (tam %zu vs %zu; 1º offset: %zu)\n",
               pkt.size(), loopback_f3a::kReqSize, diff);
        ::close(c);
        return;
    }
    printf("[server] matched REQ golden (memcmp OK)\n");
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

    // 1) response golden C1 -> decode (sem decrypt) + parse add point
    std::vector<uint8_t> resp(loopback_f3a::kRespSize);
    if (!RecvAll(fd, resp.data(), resp.size())) { ::close(fd); return false; }
    crypto::PacketCryptoSM smAny;  // C1: sem crypto
    const uint8_t ver[5] = { 1, 2, 3, 4, 5 };
    mvp::ParsedMvp out; std::string err; std::vector<uint8_t> plainC1;
    if (!mvp::DecodeAndParseMvpPacket(resp, smAny, ver, out, err, /*streamXored=*/false, &plainC1) ||
        out.head != 0xF3 || out.sub != 0x06) {
        printf("[client] decode RESP falhou\n"); ::close(fd); return false;
    }
    mvp::ParsedAddPoint ap; std::string perr;
    if (!mvp::ParseC1_F3_06_AddPointResponsePlain(plainC1, ap, perr) ||
        !ap.ok || (int)ap.statId != loopback_f3a::kExpectedStat ||
        ap.max != loopback_f3a::kExpectedMax ||
        ap.shieldMax != loopback_f3a::kExpectedShield ||
        ap.skillManaMax != loopback_f3a::kExpectedSkillMana) {
        printf("[client] parse divergente do expected_parse\n"); ::close(fd); return false;
    }
    printf("[client] decoded RESP C1: ok=%d stat=%u max=%u shield=%u skillMana=%u\n",
           ap.ok, ap.statId, ap.max, ap.shieldMax, ap.skillManaMax);

    // 2) request C->S com o builder WIRE REAL (pointType do golden)
    auto req = mvp::BuildC1_F3_06_AddPointRequestWire(static_cast<uint8_t>(loopback_f3a::kExpectedStat));
    if (req.empty()) { ::close(fd); return false; }
    printf("[client] sent REQ wire-real C1 (%zu B)\n", req.size());
    if (!SendAll(fd, req.data(), req.size())) { ::close(fd); return false; }

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
    printf("LOOPBACK F3:06 WIRE OK: RESP-C1(ok/stat/max)->REQ-C1-wire(match)\n");
    return 0;
}
