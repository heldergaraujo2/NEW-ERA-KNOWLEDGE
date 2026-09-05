// NEW-ERA — loopback_harness.cpp (1.2-A3) — F3:0x30 Option — WIRE-REAL C1
// Prova end-to-end LOCAL com socket REAL — SOMENTE 127.0.0.1 (hardcode; sem args).
//   SERVER stub: envia kRespServer (golden C1 34 B) -> recebe request C1 PLAIN
//                (wire real 34 B) -> memcmp vs kReqExpected -> close.
//   CLIENT MVP : decodifica response C1 (sem decrypt) -> ParseC1_F3_30 com
//                asserts (hotKey0/gameOption/qwer) -> envia request do builder
//                WIRE REAL (BuildC1_F3_30_OptionRequestWire).
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

    printf("[server] conectado; enviando RESP golden C1 (%zu B)\n", loopback_f3o::kRespSize);
    if (!SendAll(c, loopback_f3o::kRespServer.data(), loopback_f3o::kRespSize)) { ::close(c); return; }

    uint8_t hdr[2];
    if (!RecvAll(c, hdr, 2) || hdr[0] != 0xC1) { printf("[server] header invalido (espera C1 wire real)\n"); ::close(c); return; }
    const size_t rest = (size_t)hdr[1] - 2;
    std::vector<uint8_t> pkt((size_t)hdr[1]);
    pkt[0] = hdr[0]; pkt[1] = hdr[1];
    if (rest && !RecvAll(c, pkt.data() + 2, rest)) { printf("[server] request incompleto\n"); ::close(c); return; }

    if (pkt.size() != loopback_f3o::kReqSize ||
        std::memcmp(pkt.data(), loopback_f3o::kReqExpected.data(), loopback_f3o::kReqSize) != 0) {
        size_t n = pkt.size() < loopback_f3o::kReqSize ? pkt.size() : loopback_f3o::kReqSize;
        size_t diff = n;
        for (size_t i = 0; i < n; ++i) if (pkt[i] != loopback_f3o::kReqExpected[i]) { diff = i; break; }
        printf("[server] REQUEST DIVERGENTE do golden (tam %zu vs %zu; 1º offset: %zu)\n",
               pkt.size(), loopback_f3o::kReqSize, diff);
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

    // 1) response golden C1 -> decode (sem decrypt) + parse option
    std::vector<uint8_t> resp(loopback_f3o::kRespSize);
    if (!RecvAll(fd, resp.data(), resp.size())) { ::close(fd); return false; }
    crypto::PacketCryptoSM smAny;  // C1: sem crypto
    const uint8_t ver[5] = { 1, 2, 3, 4, 5 };
    mvp::ParsedMvp out; std::string err; std::vector<uint8_t> plainC1;
    if (!mvp::DecodeAndParseMvpPacket(resp, smAny, ver, out, err, /*streamXored=*/false, &plainC1) ||
        out.head != 0xF3 || out.sub != 0x30) {
        printf("[client] decode RESP falhou\n"); ::close(fd); return false;
    }
    mvp::ParsedOption opt; std::string perr;
    if (!mvp::ParseC1_F3_30_OptionResponsePlain(plainC1, opt, perr) ||
        opt.hotKeys[0] != loopback_f3o::kExpectedHotKey0 ||
        opt.gameOption != loopback_f3o::kExpectedGameOption ||
        opt.qwerLevel  != loopback_f3o::kExpectedQwerLevel) {
        printf("[client] parse divergente do expected_parse\n"); ::close(fd); return false;
    }
    int empties = 0;
    for (int i = 1; i < 10; ++i) if (opt.hotKeys[i] == 0xFFFF) ++empties;
    if (empties != loopback_f3o::kExpectedEmptyHotKeys) { ::close(fd); return false; }
    printf("[client] decoded RESP C1: hotKey0=0x%04x gameOption=0x%02x qwer=%d\n",
           opt.hotKeys[0], opt.gameOption, opt.qwerLevel);

    // 2) request C->S com o builder WIRE REAL (option[30] idêntica ao golden)
    std::array<uint8_t, 30> option{};
    option[0] = 0x12; option[1] = 0x34;
    for (int i = 1; i < 10; ++i) { option[2*i] = 0xFF; option[2*i+1] = 0xFF; }
    option[20] = 0xA5; option[21] = 'Q'; option[22] = 'W'; option[23] = 'E';
    option[24] = 0x01; option[25] = 0x02;
    const int32_t q = 100; std::memcpy(&option[26], &q, 4);
    auto req = mvp::BuildC1_F3_30_OptionRequestWire(option);
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
    printf("LOOPBACK F3:30 WIRE OK: RESP-C1->parse(hotKey/opt/qwer)->REQ-C1-wire(match)\n");
    return 0;
}
