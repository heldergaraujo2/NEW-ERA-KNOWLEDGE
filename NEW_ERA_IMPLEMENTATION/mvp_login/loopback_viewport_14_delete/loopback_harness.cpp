// NEW-ERA — loopback_harness.cpp (1.3-I) — CICLO SPAWN->DELETE (localhost)
// Prova end-to-end LOCAL com socket REAL — SOMENTE 127.0.0.1 (hardcode; sem args).
// Protocolo do stub: 4 mensagens [len:u16 LE][frame] (framing = artefato do stub).
//   SERVER: (len+frame12 C2 79B) (len+frame13 C2 29B) (len+del14 C1 10B)
//           (len+del14Trunc C1 9B) -> close.
//   CLIENT: ws.clear() -> Apply 0x12 (true) -> Apply 0x13 (true; ws=4; snapshot)
//           -> Apply del14 (true; ws=2; keys 0x0200/0x0202; removidas ausentes;
//              snapshot2) -> Apply del14Trunc (FALSE; ws == snapshot2) -> exit 0.
// Single-TU: inclui o core (bloco 1.3-H/1.3-I) — sem espelhos ODR.
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
static bool SendFramed(int fd, const uint8_t* frame, size_t n) {
    uint8_t pre[2] = { (uint8_t)(n & 0xFF), (uint8_t)((n >> 8) & 0xFF) };  // u16 LE
    return SendAll(fd, pre, 2) && SendAll(fd, frame, n);
}
static bool RecvFramed(int fd, std::vector<uint8_t>& frame) {
    uint8_t pre[2];
    if (!RecvAll(fd, pre, 2)) return false;
    const size_t len = (size_t)pre[0] | ((size_t)pre[1] << 8);
    frame.resize(len);
    return len ? RecvAll(fd, frame.data(), len) : true;
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

    printf("[server] conectado; enviando 4 frames framingados (len16LE)\n");
    if (!SendFramed(c, loopback_vp14::kFrame12.data(), loopback_vp14::kFrame12Len) ||
        !SendFramed(c, loopback_vp14::kFrame13.data(), loopback_vp14::kFrame13Len) ||
        !SendFramed(c, loopback_vp14::kDel14.data(), loopback_vp14::kDel14Len) ||
        !SendFramed(c, loopback_vp14::kDel14Trunc.data(), loopback_vp14::kDel14TrLen)) {
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

    mvp::WorldState ws; ws.clear(); std::string err;
    std::vector<uint8_t> f;

    // 1) spawn chars
    if (!RecvFramed(fd, f) || !mvp::ApplyFrame_C2_12_Characters(f, ws, err)) {
        printf("[client] frame12 falhou: %s\n", err.c_str()); ::close(fd); return false; }
    printf("[client] frame12 OK (ws=%zu)\n", ws.entities.size());
    // 2) spawn monsters + snapshot (size=4)
    if (!RecvFramed(fd, f) || !mvp::ApplyFrame_C2_13_Monsters(f, ws, err)) {
        printf("[client] frame13 falhou: %s\n", err.c_str()); ::close(fd); return false; }
    assert(ws.entities.size() == 4);
    const mvp::WorldState snap = ws;
    printf("[client] frame13 OK (ws=4) — snapshot tirado\n");
    // 3) delete válido -> ws=2
    if (!RecvFramed(fd, f) || !mvp::ApplyFrame_DeleteEntities_C1(f, ws, err)) {
        printf("[client] del14 falhou: %s\n", err.c_str()); ::close(fd); return false; }
    assert(ws.entities.size() == loopback_vp14::kExpectedTotal);
    for (uint16_t k : loopback_vp14::kExpectedKeys)
        if (!ws.entities.count(k)) { printf("[client] key 0x%04X ausente\n", k); ::close(fd); return false; }
    for (uint16_t k : loopback_vp14::kRemovedKeys)
        if (ws.entities.count(k)) { printf("[client] key 0x%04X nao removida\n", k); ::close(fd); return false; }
    const mvp::WorldState snap2 = ws;
    printf("[client] del14 OK (ws=2; 0x0100/0x0101 removidas; 0x9999->0x1999 ignorada)\n");
    // 4) delete truncado -> false e ws == snapshot2
    if (!RecvFramed(fd, f)) { printf("[client] del14t ausente\n"); ::close(fd); return false; }
    const bool applied = mvp::ApplyFrame_DeleteEntities_C1(f, ws, err);
    if (applied || err.rfind("0x14:", 0) != 0 || ws.entities.size() != snap2.entities.size()) {
        printf("[client] del14t: applied=%d err='%s' size=%zu\n",
               (int)applied, err.c_str(), ws.entities.size()); ::close(fd); return false; }
    for (const auto& kv : snap2.entities)
        if (ws.entities.find(kv.first) == ws.entities.end()) { printf("[client] key perdida!\n"); ::close(fd); return false; }
    printf("[client] del14_trunc rejeitado ('%s') — ws intocado (%zu entidades)\n",
           err.c_str(), ws.entities.size());

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
    printf("LOOPBACK VIEWPORT_14 OK: spawn(4)->delete(2)->trunc sem efeito\n");
    return 0;
}
