// NEW-ERA — loopback_harness.cpp (1.3-L) — BOTH_POSITION ASIO 0x0006 (localhost)
// Prova end-to-end LOCAL com socket REAL — SOMENTE 127.0.0.1 (hardcode; sem args).
// Protocolo do stub: 6 mensagens [len:u16 LE][frame] (framing = artefato do stub):
//   frame12 (C2 79B), frame13 (C2 29B), both_ok (olc 13B), pkt15_ok (C1 7B),
//   both_miss (olc 13B), both_trunc (olc 12B — DEVE falhar).
// CLIENT: ws.clear() -> 0x12 -> 0x13 (ws=4) -> both_ok (0x0100: x/y/target
//   77/88, dir intocado, missed=0) -> snapshot -> pkt15_ok (0x15 clássico:
//   EQUIVALÊNCIA — estado idêntico campo a campo) -> both_miss (missed=1) ->
//   snapshot2 -> both_trunc (FALSE, ws intocado). Single-TU (core 1.3-H..L).
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

// comparação CAMPO-A-CAMPO (EntityRecord não tem operator==)
static bool SameEntity(const mvp::EntityRecord& a, const mvp::EntityRecord& b) {
    if (a.key != b.key || a.kind != b.kind) return false;
    if (a.x != b.x || a.y != b.y) return false;
    if (a.targetX != b.targetX || a.targetY != b.targetY) return false;
    if (a.dir != b.dir || a.angleDeg != b.angleDeg) return false;
    if (a.pk != b.pk || a.classByte != b.classByte || a.type != b.type) return false;
    if (std::memcmp(a.id, b.id, sizeof(a.id)) != 0) return false;
    return a.buffs == b.buffs;
}
static bool SameWorld(const mvp::WorldState& A, const mvp::WorldState& B) {
    if (A.entities.size() != B.entities.size()) return false;
    for (const auto& kv : A.entities) {
        auto it = B.entities.find(kv.first);
        if (it == B.entities.end() || !SameEntity(kv.second, it->second)) return false;
    }
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

    printf("[server] conectado; enviando 6 frames framingados (len16LE)\n");
    if (!SendFramed(c, loopback_bothpos::kFrame12.data(),  loopback_bothpos::kFrame12Len)  ||
        !SendFramed(c, loopback_bothpos::kFrame13.data(),  loopback_bothpos::kFrame13Len)  ||
        !SendFramed(c, loopback_bothpos::kBothOk.data(),   loopback_bothpos::kBothOkLen)   ||
        !SendFramed(c, loopback_bothpos::kPkt15Ok.data(),  loopback_bothpos::kPkt15Len)    ||
        !SendFramed(c, loopback_bothpos::kBothMiss.data(), loopback_bothpos::kBothMissLen) ||
        !SendFramed(c, loopback_bothpos::kBothTrunc.data(),loopback_bothpos::kBothTrLen)) {
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
        ws.entities.size() != loopback_bothpos::kWsSize) {
        printf("[client] spawn falhou: %s\n", err.c_str()); ::close(fd); return false; }
    printf("[client] spawn OK (ws=%zu)\n", ws.entities.size());

    size_t missed = 0;
    if (!RecvFramed(fd, f) || !mvp::ApplyFrame_BOTH_POSITION_Asio(f, ws, err, &missed) ||
        missed != 0) {
        printf("[client] both_ok falhou: %s\n", err.c_str()); ::close(fd); return false; }
    assert(ws.entities.at(loopback_bothpos::kMovedKey).x == (uint8_t)loopback_bothpos::kNewX);
    assert(ws.entities.at(loopback_bothpos::kMovedKey).y == (uint8_t)loopback_bothpos::kNewY);
    assert(ws.entities.at(loopback_bothpos::kMovedKey).targetX == (uint8_t)loopback_bothpos::kNewX);
    assert(ws.entities.at(loopback_bothpos::kMovedKey).targetY == (uint8_t)loopback_bothpos::kNewY);
    printf("[client] both_ok: 0x%04X x/y/target=(%d,%d) (missed=%zu)\n",
           loopback_bothpos::kMovedKey, loopback_bothpos::kNewX, loopback_bothpos::kNewY, missed);

    // EQUIVALÊNCIA: 0x15 clássico sobre o mesmo estado => estado idêntico
    const mvp::WorldState afterBoth = ws;
    size_t missed15 = 0;
    if (!RecvFramed(fd, f) || !mvp::ApplyFrame_PositionUpdate_C1(f, ws, err, &missed15) ||
        missed15 != 0 || !SameWorld(ws, afterBoth)) {
        printf("[client] pkt15_ok/equivalencia falhou: %s\n", err.c_str()); ::close(fd); return false; }
    printf("[client] pkt15_ok: equivalencia 0x15 == 0x0006 (estado idêntico campo a campo)\n");

    if (!RecvFramed(fd, f) || !mvp::ApplyFrame_BOTH_POSITION_Asio(f, ws, err, &missed) ||
        missed != loopback_bothpos::kMissedFinal || ws.entities.size() != loopback_bothpos::kWsSize) {
        printf("[client] both_miss falhou: %s\n", err.c_str()); ::close(fd); return false; }
    printf("[client] both_miss: key inexistente ignorada (missed=%zu, ws segue %zu)\n",
           missed, ws.entities.size());

    const mvp::WorldState snap2 = ws;
    if (!RecvFramed(fd, f)) { printf("[client] both_trunc ausente\n"); ::close(fd); return false; }
    const bool applied = mvp::ApplyFrame_BOTH_POSITION_Asio(f, ws, err);
    if (applied || err.rfind("0x0006:", 0) != 0 || !SameWorld(ws, snap2)) {
        printf("[client] both_trunc: applied=%d err='%s'\n", (int)applied, err.c_str());
        ::close(fd); return false; }
    printf("[client] both_trunc rejeitado ('%s') — ws intocado\n", err.c_str());

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
    printf("LOOPBACK BOTH_POSITION OK: spawn(4)->0x0006(x/y/target)->0x15 equiv->miss(1)->trunc sem efeito\n");
    return 0;
}
