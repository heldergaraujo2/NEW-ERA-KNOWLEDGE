// NEW-ERA — loopback_harness.cpp (1.1-C) — F3:0x00 Character List
// Prova end-to-end LOCAL com socket REAL — SOMENTE 127.0.0.1 (hardcode; sem args).
//   SERVER stub: envia kRespServer (golden) -> recebe request C3 ->
//                memcmp vs kReqExpected (diff no 1º offset divergente) -> close.
//   CLIENT MVP : decodifica+parseia a char list (DecodeAndParseMvpPacket com
//                streamXored=false + ParseC1_F3_00_CharacterListPlain; asserts
//                vs expected_parse) -> envia request do builder REAL
//                (BuildC3_F3_00_RequestCharListEncrypted, Enc1) -> close.
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

    printf("[server] conectado; enviando RESP golden (%zu B)\n", loopback_f3::kRespSize);
    if (!SendAll(c, loopback_f3::kRespServer.data(), loopback_f3::kRespSize)) { ::close(c); return; }

    uint8_t hdr[2];
    if (!RecvAll(c, hdr, 2) || hdr[0] != 0xC3) { printf("[server] header invalido\n"); ::close(c); return; }
    const size_t rest = (size_t)hdr[1] - 2;
    std::vector<uint8_t> pkt((size_t)hdr[1]);
    pkt[0] = hdr[0]; pkt[1] = hdr[1];
    if (rest && !RecvAll(c, pkt.data() + 2, rest)) { printf("[server] request incompleto\n"); ::close(c); return; }

    if (pkt.size() != loopback_f3::kReqSize ||
        std::memcmp(pkt.data(), loopback_f3::kReqExpected.data(), loopback_f3::kReqSize) != 0) {
        size_t n = pkt.size() < loopback_f3::kReqSize ? pkt.size() : loopback_f3::kReqSize;
        size_t diff = n;
        for (size_t i = 0; i < n; ++i) if (pkt[i] != loopback_f3::kReqExpected[i]) { diff = i; break; }
        printf("[server] REQUEST DIVERGENTE do golden (tam %zu vs %zu; 1º offset: %zu)\n",
               pkt.size(), loopback_f3::kReqSize, diff);
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
    const char* p1 = "NEW_ERA_IMPLEMENTATION/mvp_login/keys/Dec2.dat";
    const char* p2 = "keys/Dec2.dat";
    if (!smRx.LoadKeysFromFile(p1, &err, /*type=*/1) && !smRx.LoadKeysFromFile(p2, &err, /*type=*/1)) {
        printf("[client] erro chaves RX: %s\n", err.c_str()); ::close(fd); return false;
    }

    // 1) response golden -> decode + parse char list
    std::vector<uint8_t> resp(loopback_f3::kRespSize);
    if (!RecvAll(fd, resp.data(), resp.size())) { ::close(fd); return false; }
    const uint8_t ver[5] = { 1, 2, 3, 4, 5 };
    mvp::ParsedMvp out; std::vector<uint8_t> plainC1;
    if (!mvp::DecodeAndParseMvpPacket(resp, smRx, ver, out, err, /*streamXored=*/false, &plainC1) ||
        out.head != 0xF3 || out.sub != 0x00) {
        printf("[client] decode RESP falhou: %s\n", err.c_str()); ::close(fd); return false;
    }
    mvp::ParsedCharList list; std::string perr;
    if (!mvp::ParseC1_F3_00_CharacterListPlain(plainC1, list, perr)) {
        printf("[client] parse char list falhou: %s\n", perr.c_str()); ::close(fd); return false;
    }
    if (list.hdr.count != loopback_f3::kExpectedCount || list.hdr.maxClass != loopback_f3::kExpectedMaxClass ||
        list.chars.empty() || std::string(list.chars[0].id) != loopback_f3::kExpectedName ||
        list.chars[0].level != loopback_f3::kExpectedLevel) {
        printf("[client] parse divergente do expected_parse\n"); ::close(fd); return false;
    }
    printf("[client] decoded RESP: count=%d maxClass=%d char0=%s level=%u\n",
           list.hdr.count, list.hdr.maxClass, list.chars[0].id, list.chars[0].level);

    // 2) request C->S com o builder REAL (1ª chamada => serial 0x01 = golden)
    auto req = mvp::BuildC3_F3_00_RequestCharListEncrypted(&err);
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
    printf("LOOPBACK F3:00 OK: RESP->parse(count/name/level)->REQ(match)\n");
    return 0;
}
