// NEW-ERA — loopback_harness.cpp (1.3-J) — POSITION UPDATE 0x15 (localhost)
// Prova end-to-end LOCAL com socket REAL — SOMENTE 127.0.0.1 (hardcode; sem args).
// Protocolo do stub: 6 mensagens [len:u16 LE][frame] (framing = artefato do stub):
//   frame12 (C2 79B), frame13 (C2 29B), move_ok (C1 7B), move_miss (C1 7B),
//   move_trunc (C1 6B — DEVE falhar), delete14 (C1 10B).
// CLIENT: ws.clear() -> 0x12 -> 0x13 (ws=4) -> move_ok (0x0100 -> 77/88,
//   missed=0) -> move_miss (missed=1) -> snapshot -> move_trunc (FALSE, ws
//   intocado) -> delete14 (ws=2). Final: moved pos 77/88, missed==1, size==2.
// Single-TU: inclui o core (bloco 1.3-H/1.3-I/1.3-J) — sem espelhos ODR.
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

    printf("[server] conectado; enviando 6 frames framingados (len16LE)\n");
    if (!SendFramed(c, loopback_mv15::kFrame12.data(),  loopback_mv15::kFrame12Len)  ||
        !SendFramed(c, loopback_mv15::kFrame13.data(),  loopback_mv15::kFrame13Len)  ||
        !SendFramed(c, loopback_mv15::kMoveOk.data(),   loopback_mv15::kMoveOkLen)   ||
        !SendFramed(c, loopback_mv15::kMoveMiss.data(), loopback_mv15::kMoveMissLen) ||
        !SendFramed(c, loopback_mv15::kMoveTrunc.data(),loopback_mv15::kMoveTrLen)   ||
        !SendFramed(c, loopback_mv15::kDelete14.data(), loopback_mv15::kDel14Len)) {
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

    if (!RecvFramed(fd, f) || !mvp::ApplyFrame_C2_12_Characters(f, ws, err) ||
        !RecvFramed(fd, f) || !mvp::ApplyFrame_C2_13_Monsters(f, ws, err) ||
        ws.entities.size() != 4) {
        printf("[client] spawn falhou: %s\n", err.c_str()); ::close(fd); return false; }
    printf("[client] spawn OK (ws=4)\n");

    size_t missed = 0;
    if (!RecvFramed(fd, f) || !mvp::ApplyFrame_PositionUpdate_C1(f, ws, err, &missed) ||
        missed != 0) {
        printf("[client] move_ok falhou: %s\n", err.c_str()); ::close(fd); return false; }
    assert(ws.entities.at(loopback_mv15::kMovedKey).x == (uint8_t)loopback_mv15::kNewX);
    assert(ws.entities.at(loopback_mv15::kMovedKey).y == (uint8_t)loopback_mv15::kNewY);
    assert(ws.entities.at(loopback_mv15::kMovedKey).targetX == (uint8_t)loopback_mv15::kNewX);
    assert(ws.entities.at(loopback_mv15::kMovedKey).targetY == (uint8_t)loopback_mv15::kNewY);
    printf("[client] move_ok: 0x%04X -> (%d,%d) target=idem (missed=%zu)\n",
           loopback_mv15::kMovedKey, loopback_mv15::kNewX, loopback_mv15::kNewY, missed);

    if (!RecvFramed(fd, f) || !mvp::ApplyFrame_PositionUpdate_C1(f, ws, err, &missed) ||
        missed != loopback_mv15::kMissedFinal || ws.entities.size() != 4) {
        printf("[client] move_miss falhou: %s\n", err.c_str()); ::close(fd); return false; }
    printf("[client] move_miss: key inexistente ignorada (missed=%zu, ws segue 4)\n", missed);

    const mvp::WorldState snap = ws;
    if (!RecvFramed(fd, f)) { printf("[client] move_trunc ausente\n"); ::close(fd); return false; }
    const bool applied = mvp::ApplyFrame_PositionUpdate_C1(f, ws, err);
    if (applied || err.rfind("0x15:", 0) != 0 || ws.entities.size() != snap.entities.size() ||
        ws.entities.at(loopback_mv15::kMovedKey).x != (uint8_t)loopback_mv15::kNewX) {
        printf("[client] move_trunc: applied=%d err='%s'\n", (int)applied, err.c_str());
        ::close(fd); return false; }
    for (const auto& kv : snap.entities)
        if (ws.entities.find(kv.first) == ws.entities.end()) { printf("[client] key perdida!\n"); ::close(fd); return false; }
    printf("[client] move_trunc rejeitado ('%s') — ws intocado\n", err.c_str());

    if (!RecvFramed(fd, f) || !mvp::ApplyFrame_DeleteEntities_C1(f, ws, err) ||
        ws.entities.size() != loopback_mv15::kWsFinalSize) {
        printf("[client] delete falhou: %s\n", err.c_str()); ::close(fd); return false; }
    printf("[client] delete OK (ws=%zu)\n", ws.entities.size());

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
    printf("LOOPBACK MOVE_15 OK: spawn(4)->move(77/88)->miss(1)->trunc sem efeito->delete(2)\n");
    return 0;
}
