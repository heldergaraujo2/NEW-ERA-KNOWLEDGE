// NEW-ERA — loopback_harness.cpp (1.3-F) — VIEWPORT 0x13 COM BUFFS (C2, S->C)
// Prova end-to-end LOCAL com socket REAL — SOMENTE 127.0.0.1 (hardcode; sem args).
// Pacote puramente S->C (spawn push; sem request):
//   SERVER stub: accept -> envia kRespServer (golden C2 29 B; 2 entidades c/
//                strides 11/13) -> close.
//   CLIENT MVP : recvall(29 B exatos) -> ParseC2_ViewportMonsterSpawnPlain ->
//                asserts (count=2; keys 0x0101/0x0202; dirs 1/2; buffs de cada
//                entidade IGUAIS aos golden [AA] e [01 02 03]) -> close.
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
static bool BuffsEq(const std::vector<uint8_t>& got, const uint8_t* exp, size_t n) {
    if (got.size() != n) return false;
    for (size_t i = 0; i < n; ++i) if (got[i] != exp[i]) return false;
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

    printf("[server] conectado; enviando SPAWN golden C2 0x13 c/ buffs (%zu B)\n",
           loopback_vp13b::kRespSize);
    if (!SendAll(c, loopback_vp13b::kRespServer.data(), loopback_vp13b::kRespSize)) {
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

    // recebe o frame C2 completo (29 B exatos, como o golden)
    std::vector<uint8_t> pkt(loopback_vp13b::kRespSize);
    if (!RecvAll(fd, pkt.data(), pkt.size())) {
        printf("[client] spawn incompleto\n"); ::close(fd); return false;
    }

    // parse + validação (count, keys, dirs/ângulos, BUFFS de cada entidade)
    std::vector<mvp::SpawnEntity> ents; std::string err;
    if (!mvp::ParseC2_ViewportMonsterSpawnPlain(pkt, ents, err)) {
        printf("[client] parse falhou: %s\n", err.c_str()); ::close(fd); return false;
    }
    if ((int)ents.size() != loopback_vp13b::kExpectedCount ||
        ents[0].key != loopback_vp13b::kExpectedKeyA ||
        ents[1].key != loopback_vp13b::kExpectedKeyB ||
        (int)ents[0].dir != loopback_vp13b::kExpectedDirA ||
        (int)ents[1].dir != loopback_vp13b::kExpectedDirB ||
        ents[0].angleDeg != loopback_vp13b::kExpectedAngA ||
        ents[1].angleDeg != loopback_vp13b::kExpectedAngB ||
        !BuffsEq(ents[0].buffs, loopback_vp13b::kExpectedBuffsA.data(),
                 loopback_vp13b::kExpectedBuffsA.size()) ||
        !BuffsEq(ents[1].buffs, loopback_vp13b::kExpectedBuffsB.data(),
                 loopback_vp13b::kExpectedBuffsB.size())) {
        printf("[client] parse divergente do expected_parse\n"); ::close(fd); return false;
    }
    printf("[client] spawn OK: count=%zu A(key=0x%04X dir=%d buffs=%zuB) "
           "B(key=0x%04X dir=%d buffs=%zuB)\n",
           ents.size(), ents[0].key, ents[0].dir, ents[0].buffs.size(),
           ents[1].key, ents[1].dir, ents[1].buffs.size());

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
    printf("LOOPBACK VIEWPORT_13_BUFFS OK: C2-29B -> 2 SpawnEntity (strides 11/13, buffs OK)\n");
    return 0;
}
