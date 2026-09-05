// NEW-ERA — loopback_harness.cpp (1.3-H2B) — WORLDTICK 0x12+0x13+trunc (localhost)
// Prova end-to-end LOCAL com socket REAL — SOMENTE 127.0.0.1 (hardcode; sem args).
// Protocolo do stub: 3 mensagens, cada uma [len:u16 LE][frame C2].
//   SERVER: accept -> (len+frame12) (len+frame13) (len+frame13Trunc) -> close.
//   CLIENT: i=1 ApplyFrame_C2_12_Characters (true; 2 chars)
//           i=2 ApplyFrame_C2_13_Monsters  (true; 2 monsters) + SNAPSHOT deep
//           i=3 ApplyFrame_C2_13_Monsters  (FALSE; ws inalterado vs snapshot)
//           final: 4 entidades, 2/2 por kind, keys 0x0100/0x0200/0x0101/0x0202.
// BUILD TWO-TU (por comando): linka com ../mvp_login_client.cpp. O espelho
// abaixo (EntityKind/EntityRecord/WorldState) DEVE permanecer token-idêntico
// ao bloco 1.3-H do core (ODR); Apply* são prototypes only.
#include "embedded_vectors.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <atomic>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <future>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

// ---- ESPELHO ODR do bloco 1.3-H (não editar sem editar o core) ----
namespace newera { namespace mvp {

enum class EntityKind : uint8_t { Character = 0, Monster = 1 };

struct EntityRecord {
    uint16_t    key = 0;      // 0..0x7FFF (:2177/:2605)
    EntityKind  kind = EntityKind::Monster;
    uint8_t     x = 0, y = 0;
    uint8_t     targetX = 0, targetY = 0;   // provados nos dois pacotes
    uint8_t     dir = 0;                    // Path>>4 (:2271/:2637)
    int         angleDeg = 0;               // ((dir-1)*45)
    uint8_t     pk = 0;        // só Character (Path&0xF :2237)
    char        id[11] = {};   // só Character (ID[10]+NUL :2353-:2354)
    uint8_t     classByte = 0; // só Character (:2235; pose=&0x07 :2240)
    uint16_t    type = 0;      // só Monster (10 bits :2597)
    std::vector<uint8_t> buffs;             // s_BuffEffectState :2361/:2614
};

struct WorldState {
    std::unordered_map<uint16_t, EntityRecord> entities;

    size_t countByKind(EntityKind k) const {   // counters derivados (sem cache)
        size_t n = 0;
        for (const auto& kv : entities) if (kv.second.kind == k) ++n;
        return n;
    }
    void clear() { entities.clear(); }
};

bool ApplyFrame_C2_12_Characters(const std::vector<uint8_t>& frame,
                                 WorldState& ws, std::string& err);
bool ApplyFrame_C2_13_Monsters(const std::vector<uint8_t>& frame,
                               WorldState& ws, std::string& err);

} } // namespace newera::mvp

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

    printf("[server] conectado; enviando 3 frames framingados (len16LE)\n");
    if (!SendFramed(c, loopback_wtick::kFrame12.data(), loopback_wtick::kFrame12Len) ||
        !SendFramed(c, loopback_wtick::kFrame13.data(), loopback_wtick::kFrame13Len) ||
        !SendFramed(c, loopback_wtick::kFrame13Trunc.data(), loopback_wtick::kFrameTrLen)) {
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

    mvp::WorldState ws; std::string err;
    mvp::WorldState snap;                             // snapshot pós-frame-2
    for (int i = 1; i <= 3; ++i) {
        uint8_t pre[2];
        if (!RecvAll(fd, pre, 2)) { printf("[client] len ausente (i=%d)\n", i); ::close(fd); return false; }
        const size_t len = (size_t)pre[0] | ((size_t)pre[1] << 8);   // u16 LE
        std::vector<uint8_t> frame(len);
        if (len && !RecvAll(fd, frame.data(), len)) { printf("[client] frame incompleto (i=%d)\n", i); ::close(fd); return false; }

        if (i == 1) {
            if (!mvp::ApplyFrame_C2_12_Characters(frame, ws, err)) {
                printf("[client] frame12 falhou: %s\n", err.c_str()); ::close(fd); return false; }
            printf("[client] frame12 OK: %zu chars (ws=%zu)\n",
                   ws.countByKind(mvp::EntityKind::Character), ws.entities.size());
        } else if (i == 2) {
            if (!mvp::ApplyFrame_C2_13_Monsters(frame, ws, err)) {
                printf("[client] frame13 falhou: %s\n", err.c_str()); ::close(fd); return false; }
            snap = ws;                                // deep copy (size+keys+...)
            printf("[client] frame13 OK: %zu monsters (ws=%zu) — snapshot tirado\n",
                   ws.countByKind(mvp::EntityKind::Monster), ws.entities.size());
        } else {
            const bool applied = mvp::ApplyFrame_C2_13_Monsters(frame, ws, err);
            if (applied) { printf("[client] frame13_trunc ACEITO indevidamente\n"); ::close(fd); return false; }
            if (err.rfind("0x13:", 0) != 0) { printf("[client] erro inesperado: %s\n", err.c_str()); ::close(fd); return false; }
            if (ws.entities.size() != snap.entities.size()) { printf("[client] ws mudou de tamanho!\n"); ::close(fd); return false; }
            for (const auto& kv : snap.entities)
                if (ws.entities.find(kv.first) == ws.entities.end()) { printf("[client] key perdida!\n"); ::close(fd); return false; }
            printf("[client] frame13_trunc rejeitado ('%s') — ws intocado (%zu entidades)\n",
                   err.c_str(), ws.entities.size());
        }
    }

    // ---------- validação final ----------
    assert(ws.entities.size() == loopback_wtick::kExpectedTotal);
    assert(ws.countByKind(mvp::EntityKind::Character) == loopback_wtick::kExpectedChars);
    assert(ws.countByKind(mvp::EntityKind::Monster) == loopback_wtick::kExpectedMonst);
    for (uint16_t k : loopback_wtick::kExpectedKeys)
        if (!ws.entities.count(k)) { printf("[client] key 0x%04X ausente\n", k); ::close(fd); return false; }

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
    printf("LOOPBACK WORLDTICK OK: 3 frames(len16LE) -> ws=4 (2 chars/2 monsters); trunc sem efeito\n");
    return 0;
}
