// NEW-ERA — loopback_harness.cpp (1.3-K) — BOTH_MOVE ASIO 0x0007 (localhost)
// Prova end-to-end LOCAL com socket REAL — SOMENTE 127.0.0.1 (hardcode; sem args).
// Protocolo do stub: 5 mensagens [len:u16 LE][frame] (framing = artefato do stub):
//   frame12 (C2 79B), frame13 (C2 29B), move_ok (olc 11B), move_miss (olc 11B),
//   move_trunc (olc 10B — DEVE falhar).
// CLIENT: ws.clear() -> 0x12 -> 0x13 (ws=4) -> move_ok (0x0100: target 77/88,
//   dir=3/90°, x/y INTOCADOS, missed=0) -> move_miss (missed=1) -> snapshot ->
//   move_trunc (FALSE, ws intocado). Single-TU (core 1.3-H..K incluso).
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

    printf("[server] conectado; enviando 5 frames framingados (len16LE)\n");
    if (!SendFramed(c, loopback_mvd4::kFrame12.data(),  loopback_mvd4::kFrame12Len)  ||
        !SendFramed(c, loopback_mvd4::kFrame13.data(),  loopback_mvd4::kFrame13Len)  ||
        !SendFramed(c, loopback_mvd4::kMoveOk.data(),   loopback_mvd4::kMoveOkLen)   ||
        !SendFramed(c, loopback_mvd4::kMoveMiss.data(), loopback_mvd4::kMoveMissLen) ||
        !SendFramed(c, loopback_mvd4::kMoveTrunc.data(),loopback_mvd4::kMoveTrLen)) {
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
        ws.entities.size() != loopback_mvd4::kWsSize) {
        printf("[client] spawn falhou: %s\n", err.c_str()); ::close(fd); return false; }
    printf("[client] spawn OK (ws=%zu)\n", ws.entities.size());

    size_t missed = 0;
    if (!RecvFramed(fd, f) || !mvp::ApplyFrame_PacketMoveD4_Asio(f, ws, err, &missed) ||
        missed != 0) {
        printf("[client] move_ok falhou: %s\n", err.c_str()); ::close(fd); return false; }
    assert(ws.entities.at(loopback_mvd4::kMovedKey).targetX == (uint8_t)loopback_mvd4::kNewTX);
    assert(ws.entities.at(loopback_mvd4::kMovedKey).targetY == (uint8_t)loopback_mvd4::kNewTY);
    assert(ws.entities.at(loopback_mvd4::kMovedKey).dir == (uint8_t)loopback_mvd4::kDir);
    assert(ws.entities.at(loopback_mvd4::kMovedKey).angleDeg == loopback_mvd4::kAngle);
    assert(ws.entities.at(loopback_mvd4::kMovedKey).x == (uint8_t)loopback_mvd4::kXUnchanged);
    assert(ws.entities.at(loopback_mvd4::kMovedKey).y == (uint8_t)loopback_mvd4::kYUnchanged);
    printf("[client] move_ok: 0x%04X target=(%d,%d) dir=%d (%d deg) xy intocados (missed=%zu)\n",
           loopback_mvd4::kMovedKey, loopback_mvd4::kNewTX, loopback_mvd4::kNewTY,
           loopback_mvd4::kDir, loopback_mvd4::kAngle, missed);

    if (!RecvFramed(fd, f) || !mvp::ApplyFrame_PacketMoveD4_Asio(f, ws, err, &missed) ||
        missed != loopback_mvd4::kMissedFinal || ws.entities.size() != loopback_mvd4::kWsSize) {
        printf("[client] move_miss falhou: %s\n", err.c_str()); ::close(fd); return false; }
    printf("[client] move_miss: key inexistente ignorada (missed=%zu, ws segue %zu)\n",
           missed, ws.entities.size());

    const mvp::WorldState snap = ws;
    if (!RecvFramed(fd, f)) { printf("[client] move_trunc ausente\n"); ::close(fd); return false; }
    const bool applied = mvp::ApplyFrame_PacketMoveD4_Asio(f, ws, err);
    if (applied || err.rfind("0xD4:", 0) != 0 || ws.entities.size() != snap.entities.size() ||
        ws.entities.at(loopback_mvd4::kMovedKey).targetX != (uint8_t)loopback_mvd4::kNewTX) {
        printf("[client] move_trunc: applied=%d err='%s'\n", (int)applied, err.c_str());
        ::close(fd); return false; }
    for (const auto& kv : snap.entities)
        if (ws.entities.find(kv.first) == ws.entities.end()) { printf("[client] key perdida!\n"); ::close(fd); return false; }
    printf("[client] move_trunc rejeitado ('%s') — ws intocado\n", err.c_str());

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
    printf("LOOPBACK MOVE_D4_ASIO OK: spawn(4)->both_move(target/dir/angle; xy ok)->miss(1)->trunc sem efeito\n");
    return 0;
}
