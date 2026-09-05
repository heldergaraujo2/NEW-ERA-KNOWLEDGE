// NEW-ERA — loopback_harness.cpp (1.3-G) — VIEWPORT 0x12 CHARACTER (C2, S->C)
// Prova end-to-end LOCAL com socket REAL — SOMENTE 127.0.0.1 (hardcode; sem args).
// Pacote puramente S->C (spawn push; sem request):
//   SERVER stub: accept -> envia kRespServer (golden C2 79 B; 2 entidades c/
//                strides 36/38) -> close.
//   CLIENT MVP : recvall(79 B exatos) -> ParseViewportCharacterSpawnPlain_C2 ->
//                asserts (count=2; ids "HeroA"/"HeroB"; keys 0x0100/0x0200;
//                dirs 1/2 c/ angulos 0°/45°; PK 3/0 — Path dual :2237/:2271;
//                buffs B=[55 66]; equip first/last preservados) -> close.
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

    printf("[server] conectado; enviando CHAR-SPAWN golden C2 0x12 (%zu B)\n",
           loopback_vp12::kRespSize);
    if (!SendAll(c, loopback_vp12::kRespServer.data(), loopback_vp12::kRespSize)) {
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

    // recebe o frame C2 completo (79 B exatos, como o golden)
    std::vector<uint8_t> pkt(loopback_vp12::kRespSize);
    if (!RecvAll(fd, pkt.data(), pkt.size())) {
        printf("[client] spawn incompleto\n"); ::close(fd); return false;
    }

    // parse + validação (count, ids, keys, dirs/ângulos, PK — Path dual, buffs B)
    std::vector<mvp::SpawnCharacter> ents; std::string err;
    if (!mvp::ParseViewportCharacterSpawnPlain_C2(pkt, ents, err)) {
        printf("[client] parse falhou: %s\n", err.c_str()); ::close(fd); return false;
    }
    if ((int)ents.size() != loopback_vp12::kExpectedCount ||
        ents[0].key != loopback_vp12::kExpectedKeyA ||
        ents[1].key != loopback_vp12::kExpectedKeyB ||
        std::string(ents[0].id) != loopback_vp12::kExpectedIdA ||
        std::string(ents[1].id) != loopback_vp12::kExpectedIdB ||
        (int)ents[0].dir != loopback_vp12::kExpectedDirA ||
        (int)ents[1].dir != loopback_vp12::kExpectedDirB ||
        (int)ents[0].pk != loopback_vp12::kExpectedPkA ||
        (int)ents[1].pk != loopback_vp12::kExpectedPkB ||
        ents[0].angleDeg != loopback_vp12::kExpectedAngA ||
        ents[1].angleDeg != loopback_vp12::kExpectedAngB ||
        !ents[0].buffs.empty() ||
        ents[1].buffs.size() != loopback_vp12::kExpectedBuffsB.size() ||
        std::memcmp(ents[1].buffs.data(), loopback_vp12::kExpectedBuffsB.data(),
                    loopback_vp12::kExpectedBuffsB.size()) != 0 ||
        ents[0].equipment[0] != 0x00 || ents[0].equipment[16] != 0x10 ||
        ents[1].equipment[0] != 0xAA || ents[1].equipment[16] != 0xAA) {
        printf("[client] parse divergente do expected_parse\n"); ::close(fd); return false;
    }
    printf("[client] char-spawn OK: count=%zu A(%s key=0x%04X dir=%d pk=%d) "
           "B(%s key=0x%04X dir=%d pk=%d buffs=%zuB equip OK)\n",
           ents.size(), ents[0].id, ents[0].key, ents[0].dir, ents[0].pk,
           ents[1].id, ents[1].key, ents[1].dir, ents[1].pk, ents[1].buffs.size());

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
    printf("LOOPBACK VIEWPORT_12 OK: C2-79B -> 2 SpawnCharacter (strides 36/38; PK/dir; buffs)\n");
    return 0;
}
