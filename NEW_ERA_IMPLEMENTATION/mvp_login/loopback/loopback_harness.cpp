// NEW-ERA — loopback_harness.cpp (1.0-G)
// Prova end-to-end LOCAL do MVP de login com socket REAL — SOMENTE 127.0.0.1
// (hardcode; sem argumentos de host/porta; sem rede externa).
//   SERVER stub: envia R1 (golden) -> recebe T1 -> memcmp vs kT1_expected
//                (diff no 1º offset divergente) -> envia R2 (golden).
//   CLIENT MVP : decode R1 (pipeline real, Dec2) -> builder T1 REAL (Enc1)
//                -> envia -> decode R2 (Value=1 esperado).
// Integração: linka com mvp_login_client.cpp (main() sob NEWERA_MVP_NO_MAIN).
#include "embedded_vectors.h"
#include "mvp_login_client.cpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <atomic>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <future>
#include <string>
#include <thread>
#include <vector>

using namespace newera;
static const uint8_t kVer[5] = { 1, 2, 3, 4, 5 };   // inputs T1 (1.0-F)

static bool SendAll(int fd, const uint8_t* p, size_t n) {
    size_t off = 0;
    while (off < n) {
        ssize_t w = ::send(fd, p + off, n - off, 0);
        if (w <= 0) return false;
        off += static_cast<size_t>(w);
    }
    return true;
}
static bool RecvAll(int fd, uint8_t* p, size_t n) {
    size_t off = 0;
    while (off < n) {
        ssize_t r = ::recv(fd, p + off, n - off, 0);
        if (r <= 0) return false;
        off += static_cast<size_t>(r);
    }
    return true;
}

// ---------------- SERVER STUB (thread) ----------------
static std::atomic<bool> g_serverOk{ false };

static void ServerThread(std::promise<int> portPromise) {
    int ls = ::socket(AF_INET, SOCK_STREAM, 0);
    if (ls < 0) { portPromise.set_value(-1); return; }
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);   // SOMENTE 127.0.0.1
    addr.sin_port = 0;                               // porta efêmera
    if (::bind(ls, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0 ||
        ::listen(ls, 1) != 0) { portPromise.set_value(-1); ::close(ls); return; }
    socklen_t alen = sizeof(addr);
    ::getsockname(ls, reinterpret_cast<sockaddr*>(&addr), &alen);
    portPromise.set_value(ntohs(addr.sin_port));

    int c = ::accept(ls, nullptr, nullptr);
    ::close(ls);
    if (c < 0) return;

    printf("[server] conectado; enviando R1 (%zu B)\n", loopback::kR1Size);
    if (!SendAll(c, loopback::kR1.data(), loopback::kR1Size)) { ::close(c); return; }

    // T1 do cliente: [0xC3][size] + (size-2)
    uint8_t hdr[2];
    if (!RecvAll(c, hdr, 2) || hdr[0] != 0xC3) { printf("[server] header T1 invalido\n"); ::close(c); return; }
    const size_t rest = static_cast<size_t>(hdr[1]) - 2;
    std::vector<uint8_t> pkt(static_cast<size_t>(hdr[1]));
    pkt[0] = hdr[0]; pkt[1] = hdr[1];
    if (rest && !RecvAll(c, pkt.data() + 2, rest)) { printf("[server] T1 incompleto\n"); ::close(c); return; }
    printf("[server] recebeu T1 (%zu B)\n", pkt.size());

    if (pkt.size() != loopback::kT1Size ||
        std::memcmp(pkt.data(), loopback::kT1_expected.data(), loopback::kT1Size) != 0) {
        size_t n = pkt.size() < loopback::kT1Size ? pkt.size() : loopback::kT1Size;
        size_t diff = n;
        for (size_t i = 0; i < n; ++i) if (pkt[i] != loopback::kT1_expected[i]) { diff = i; break; }
        printf("[server] T1 DIVERGENTE do golden (tam %zu vs %zu; 1º offset divergente: %zu)\n",
               pkt.size(), loopback::kT1Size, diff);
        ::close(c);
        return;
    }
    printf("[server] matched T1_expected (memcmp OK)\n");

    printf("[server] enviando R2 (%zu B)\n", loopback::kR2Size);
    if (!SendAll(c, loopback::kR2.data(), loopback::kR2Size)) { ::close(c); return; }
    ::close(c);
    g_serverOk = true;
}

// ---------------- CLIENT MVP (thread) ----------------
static bool LoadSm(crypto::PacketCryptoSM& sm, const char* p1, const char* p2, int type, std::string& err) {
    if (sm.LoadKeysFromFile(p1, &err, type)) return true;
    return sm.LoadKeysFromFile(p2, &err, type);
}

static bool ClientThread(int port) {
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) return false;
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);   // SOMENTE 127.0.0.1
    addr.sin_port = htons(static_cast<uint16_t>(port));
    if (::connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0) { ::close(fd); return false; }

    std::string err;
    crypto::PacketCryptoSM smRx;   // Dec2
    if (!LoadSm(smRx, "NEW_ERA_IMPLEMENTATION/mvp_login/keys/Dec2.dat", "keys/Dec2.dat", 1, err)) {
        printf("[client] erro chaves RX: %s\n", err.c_str()); ::close(fd); return false;
    }
    crypto::PacketCryptoSM smTx;   // Enc1
    if (!LoadSm(smTx, "NEW_ERA_IMPLEMENTATION/mvp_login/keys/Enc1.dat", "keys/Enc1.dat", 0, err)) {
        printf("[client] erro chaves TX: %s\n", err.c_str()); ::close(fd); return false;
    }

    // Pacote 1: R1 (join handshake, stream-Xor) — decode com pipeline real
    std::vector<uint8_t> r1(loopback::kR1Size);
    if (!RecvAll(fd, r1.data(), r1.size())) { ::close(fd); return false; }
    mvp::ParsedMvp out1;
    if (!mvp::DecodeAndParseMvpPacket(r1, smRx, kVer, out1, err, /*streamXored=*/true) ||
        !out1.isJoin || !out1.join.ok || out1.join.result != 0x01 || out1.join.heroKey != 0x1234 || !out1.versionOk) {
        printf("[client] decode R1 FALHOU: %s\n", err.c_str()); ::close(fd); return false;
    }
    printf("[client] decoded R1: F1:00 result=01 heroKey=0x1234 versionOk=1 serial=%u\n", out1.serial);

    // T1 com o builder REAL (mesmos inputs do golden; 1ª chamada => serialPkt=0x01)
    uint8_t serial16[16]; for (int i = 0; i < 16; ++i) serial16[i] = static_cast<uint8_t>(i);
    auto t1 = mvp::BuildC3_F1_01_LoginRequestEncrypted("111", "111", 0x11223344u, kVer, serial16, smTx, &err);
    if (t1.empty()) { printf("[client] builder T1 FALHOU: %s\n", err.c_str()); ::close(fd); return false; }
    printf("[client] sent T1 (%zu B)\n", t1.size());
    if (!SendAll(fd, t1.data(), t1.size())) { ::close(fd); return false; }

    // Pacote 2: R2 (login result, estilo GS real — sem stream-Xor)
    std::vector<uint8_t> r2(loopback::kR2Size);
    if (!RecvAll(fd, r2.data(), r2.size())) { ::close(fd); return false; }
    mvp::ParsedMvp out2;
    if (!mvp::DecodeAndParseMvpPacket(r2, smRx, kVer, out2, err, /*streamXored=*/false) ||
        !out2.isLoginResult || out2.loginValue != 0x01) {
        printf("[client] decode R2 FALHOU (value=%d): %s\n", out2.loginValue, err.c_str()); ::close(fd); return false;
    }
    printf("[client] decoded R2: F1:01 Value=%d (login OK)\n", out2.loginValue);
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
    if (!cli)            { printf("FALHA: client\n");  return 3; }
    if (!g_serverOk)     { printf("FALHA: server\n");  return 4; }
    printf("LOOPBACK OK: R1->T1(match)->R2(Value=1)\n");
    return 0;
}
