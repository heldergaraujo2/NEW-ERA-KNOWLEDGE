// NEW-ERA — loopback_harness.cpp (1.1-E) — F3:0x30 Option Data
// Prova end-to-end LOCAL com socket REAL — SOMENTE 127.0.0.1 (hardcode; sem args).
//   SERVER stub: envia kRespServer (golden) -> recebe request C3 -> memcmp vs
//                kReqExpected (diff no 1º offset divergente) -> close.
//   CLIENT MVP : decodifica response (streamXored=false) -> ParseC1_F3_30 com
//                asserts (hotKey0/0xFFFF/gameOption/qwer) -> envia request do
//                builder REAL (BuildC3_F3_30_OptionRequestEncrypted, Enc1).
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

    printf("[server] conectado; enviando RESP golden (%zu B)\n", loopback_f3o::kRespSize);
    if (!SendAll(c, loopback_f3o::kRespServer.data(), loopback_f3o::kRespSize)) { ::close(c); return; }

    uint8_t hdr[2];
    if (!RecvAll(c, hdr, 2) || hdr[0] != 0xC3) { printf("[server] header invalido\n"); ::close(c); return; }
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

    std::string err;
    crypto::PacketCryptoSM smRx;   // Dec2
    if (!smRx.LoadKeysFromFile("NEW_ERA_IMPLEMENTATION/mvp_login/keys/Dec2.dat", &err, /*type=*/1) &&
        !smRx.LoadKeysFromFile("keys/Dec2.dat", &err, /*type=*/1)) {
        printf("[client] erro chaves RX: %s\n", err.c_str()); ::close(fd); return false;
    }

    // 1) response golden -> decode + parse option
    std::vector<uint8_t> resp(loopback_f3o::kRespSize);
    if (!RecvAll(fd, resp.data(), resp.size())) { ::close(fd); return false; }
    const uint8_t ver[5] = { 1, 2, 3, 4, 5 };
    mvp::ParsedMvp out; std::vector<uint8_t> plainC1;
    if (!mvp::DecodeAndParseMvpPacket(resp, smRx, ver, out, err, /*streamXored=*/false, &plainC1) ||
        out.head != 0xF3 || out.sub != 0x30) {
        printf("[client] decode RESP falhou: %s\n", err.c_str()); ::close(fd); return false;
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
    if (empties != loopback_f3o::kExpectedEmptyHotKeys) {
        printf("[client] hotKeys vazias != esperado\n"); ::close(fd); return false;
    }
    printf("[client] decoded RESP: hotKey0=0x%04x gameOption=0x%02x qwer=%d (%d hotkeys vazias)\n",
           opt.hotKeys[0], opt.gameOption, opt.qwerLevel, empties);

    // 2) request C->S com o builder REAL (1ª chamada => serial 0x01 = golden)
    uint8_t option[30] = { 0 };
    option[0] = 0x12; option[1] = 0x34;
    for (int i = 1; i < 10; ++i) { option[2*i] = 0xFF; option[2*i+1] = 0xFF; }
    option[20] = 0xA5; option[21] = 'Q'; option[22] = 'W'; option[23] = 'E';
    option[24] = 0x01; option[25] = 0x02;
    const int32_t q = 100; std::memcpy(&option[26], &q, 4);
    auto req = mvp::BuildC3_F3_30_OptionRequestEncrypted(option, &err);
    if (req.empty()) { printf("[client] builder falhou: %s\n", err.c_str()); ::close(fd); return false; }
    printf("[client] sent REQ (%zu B)\n", req.size());
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
    printf("LOOPBACK F3:30 OK: RESP->parse(hotKey/opt/qwer)->REQ(match)\n");
    return 0;
}
