// NEW-ERA — loopback_harness.cpp (1.3-E) — VIEWPORT SPAWN — C2 head 0x13 (S->C)
// Prova end-to-end LOCAL com socket REAL — SOMENTE 127.0.0.1 (hardcode; sem args).
// Pacote puramente S->C (spawn push; sem request):
//   SERVER stub: accept -> envia kRespServer (golden C2 15 B) -> close.
//   CLIENT MVP : recvall(15 B exatos) -> ParseC2_ViewportMonsterSpawnPlain ->
//                asserts (count/key/type/x/y/dir/angle/flags/buffs) -> close.
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

    printf("[server] conectado; enviando SPAWN golden C2 0x13 (%zu B)\n",
           loopback_vp13::kRespSize);
    if (!SendAll(c, loopback_vp13::kRespServer.data(), loopback_vp13::kRespSize)) {
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

    // recebe o frame C2 completo (15 B exatos, como o golden)
    std::vector<uint8_t> pkt(loopback_vp13::kRespSize);
    if (!RecvAll(fd, pkt.data(), pkt.size())) {
        printf("[client] spawn incompleto\n"); ::close(fd); return false;
    }

    // parse + validação do expected_parse
    std::vector<mvp::SpawnEntity> ents; std::string err;
    if (!mvp::ParseC2_ViewportMonsterSpawnPlain(pkt, ents, err)) {
        printf("[client] parse falhou: %s\n", err.c_str()); ::close(fd); return false;
    }
    if ((int)ents.size() != loopback_vp13::kExpectedCount ||
        ents[0].key       != loopback_vp13::kExpectedKey ||
        ents[0].type      != loopback_vp13::kExpectedType ||
        (int)ents[0].x    != loopback_vp13::kExpectedX ||
        (int)ents[0].y    != loopback_vp13::kExpectedY ||
        (int)ents[0].dir  != loopback_vp13::kExpectedDir ||
        ents[0].angleDeg  != loopback_vp13::kExpectedAngle ||
        ents[0].createFlag || ents[0].teleportFlag || ents[0].myMob ||
        !ents[0].buffs.empty()) {
        printf("[client] parse divergente do expected_parse\n"); ::close(fd); return false;
    }
    printf("[client] spawn OK: count=%zu key=0x%04X type=0x%04X x=%u y=%u dir=%u (%d deg)\n",
           ents.size(), ents[0].key, ents[0].type, ents[0].x, ents[0].y, ents[0].dir,
           ents[0].angleDeg);

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
    printf("LOOPBACK VIEWPORT_13 OK: SPAWN-C2-15B -> SpawnEntity(count/key/type/x/y/dir)\n");
    return 0;
}
