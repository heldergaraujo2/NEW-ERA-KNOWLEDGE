// NEW-ERA — loopback_harness.cpp (1.3-M) — DAMAGE RX 0x11 + TÚNEL BOTH_MESSAGE 0x000C (localhost)
// Prova end-to-end LOCAL com socket REAL — SOMENTE 127.0.0.1 (hardcode; sem args).
// Protocolo do stub: 7 mensagens [len:u16 LE][frame] (framing = artefato do stub):
//   frame12 (C2 79B), frame13 (C2 29B), atk_ok (C1 10B), atk_miss (C1 10B),
//   atk_trunc (C1 9B — DEVE falhar), both_ok (olc 16B túnel),
//   both_trunc (olc 15B — DEVE falhar).
// CLIENT: ws.clear() -> 0x12 -> 0x13 (ws=4) -> atk_ok (0x0100 lastDamage=0x0123,
// missed=0) -> atk_miss (missed=1) -> snapshot -> atk_trunc (FALSE, ws intacto)
// -> probe lastDamage=-1 -> both_ok túnel (APLICA: 0x0123, missed2=0) ->
// snapshot2 -> both_trunc (FALSE, ws intocado). TWO-TU: espelho ODR
// token-idêntico (NÃO EDITAR sem editar o core) + link c/ mvp_login_client.cpp.
#include "embedded_vectors.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdint>
#include <atomic>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <future>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

// ---- espelho ODR (cópia token-idêntica do core — não editar sem o core) ----
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
    // 1.3-M (damage RX 0x11): HP/Shield NÃO vêm no viewport (0x12/0x13 sem
    // Life) => opcionais, só populados por pacote futuro de stats; lastDamage
    // espelha c->Hit = Damage (:3188 — todos os caminhos do handler)
    std::optional<int> hp;      // ausente até stats (não recuperado no MVP)
    std::optional<int> shield;  // idem
    int lastDamage = -1;        // último dano recebido (c->Hit :3188)
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

struct DamageEvent {
    uint16_t key = 0;           // pós-máscara &0x7FFF (:2997)
    bool     success = false;   // b15 do raw (:2996)
    uint16_t damage = 0;        // WORD BE (:3002)
    uint16_t shieldDamage = 0;  // WORD BE (:3013)
    uint8_t  damageTypeRaw = 0; // byte cru (:3009)
    int      type = 0;          // &0x3F (:3009)
    bool     doubleEnable = false; // >>6 &1 (:3011)
    bool     comboEnable = false;  // >>7 &1 (:3012)
};

// ---- prototypes (link: mvp_login_client.cpp; SEM default args aqui) ----
bool ApplyFrame_C2_12_Characters(const std::vector<uint8_t>& frame,
                                 WorldState& ws, std::string& err);
bool ApplyFrame_C2_13_Monsters(const std::vector<uint8_t>& frame,
                               WorldState& ws, std::string& err);
bool ParseDamageRxPlain_C1(const std::vector<uint8_t>& frame,
                           DamageEvent& out, std::string& err);
bool ApplyFrame_DamageRx_C1(const std::vector<uint8_t>& frame,
                            WorldState& ws, std::string& err,
                            size_t* missed);
bool ExtractClassicFromBothMessage_Asio(const std::vector<uint8_t>& frame,
                                        std::vector<uint8_t>& outClassic,
                                        std::string& err);
bool ApplyFrame_BOTH_MESSAGE_Tunnel_DamageOnly(const std::vector<uint8_t>& frame,
                                               WorldState& ws, std::string& err,
                                               size_t* missed);
}} // namespace newera::mvp
// ------------------------------ fim do espelho ------------------------------

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

// comparação CAMPO-A-CAMPO (inclui hp/shield/lastDamage do 1.3-M)
static bool SameEntity(const mvp::EntityRecord& a, const mvp::EntityRecord& b) {
    if (a.key != b.key || a.kind != b.kind) return false;
    if (a.x != b.x || a.y != b.y) return false;
    if (a.targetX != b.targetX || a.targetY != b.targetY) return false;
    if (a.dir != b.dir || a.angleDeg != b.angleDeg) return false;
    if (a.pk != b.pk || a.classByte != b.classByte || a.type != b.type) return false;
    if (std::memcmp(a.id, b.id, sizeof(a.id)) != 0) return false;
    if (a.buffs != b.buffs) return false;
    if (a.hp != b.hp || a.shield != b.shield) return false;
    return a.lastDamage == b.lastDamage;
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

    printf("[server] conectado; enviando 7 frames framingados (len16LE)\n");
    if (!SendFramed(c, loopback_dmg::kFrame12.data(),  loopback_dmg::kFrame12Len)  ||
        !SendFramed(c, loopback_dmg::kFrame13.data(),  loopback_dmg::kFrame13Len)  ||
        !SendFramed(c, loopback_dmg::kAtkOk.data(),    loopback_dmg::kAtkOkLen)    ||
        !SendFramed(c, loopback_dmg::kAtkMiss.data(),  loopback_dmg::kAtkMissLen)  ||
        !SendFramed(c, loopback_dmg::kAtkTrunc.data(), loopback_dmg::kAtkTrLen)    ||
        !SendFramed(c, loopback_dmg::kBothOk.data(),   loopback_dmg::kBothOkLen)   ||
        !SendFramed(c, loopback_dmg::kBothTrunc.data(),loopback_dmg::kBothTrLen)) {
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
        ws.entities.size() != loopback_dmg::kWsSize) {
        printf("[client] spawn falhou: %s\n", err.c_str()); ::close(fd); return false; }
    printf("[client] spawn OK (ws=%zu)\n", ws.entities.size());

    size_t missed = 0;
    if (!RecvFramed(fd, f) || !mvp::ApplyFrame_DamageRx_C1(f, ws, err, &missed) ||
        missed != 0) {
        printf("[client] atk_ok falhou: %s\n", err.c_str()); ::close(fd); return false; }
    assert(ws.entities.at(loopback_dmg::kKey).lastDamage == loopback_dmg::kLastDamage);
    printf("[client] atk_ok: 0x%04X lastDamage=0x%04X (c->Hit :3188; missed=%zu)\n",
           loopback_dmg::kKey, (unsigned)loopback_dmg::kLastDamage, missed);

    if (!RecvFramed(fd, f) || !mvp::ApplyFrame_DamageRx_C1(f, ws, err, &missed) ||
        missed != loopback_dmg::kMissedClassic || ws.entities.size() != loopback_dmg::kWsSize) {
        printf("[client] atk_miss falhou: %s\n", err.c_str()); ::close(fd); return false; }
    printf("[client] atk_miss: key inexistente ignorada (missed=%zu, ws segue %zu)\n",
           missed, ws.entities.size());

    const mvp::WorldState snap = ws;
    if (!RecvFramed(fd, f)) { printf("[client] atk_trunc ausente\n"); ::close(fd); return false; }
    const bool applied = mvp::ApplyFrame_DamageRx_C1(f, ws, err, &missed);
    if (applied || err.rfind("0x11:", 0) != 0 || !SameWorld(ws, snap)) {
        printf("[client] atk_trunc: applied=%d err='%s'\n", (int)applied, err.c_str());
        ::close(fd); return false; }
    printf("[client] atk_trunc rejeitado ('%s') — ws intocado\n", err.c_str());

    // túnel: probe distingue apply real (lastDamage resetado antes)
    ws.entities.at(loopback_dmg::kKey).lastDamage = -1;          // PROBE de teste
    size_t missed2 = 0;
    if (!RecvFramed(fd, f) ||
        !mvp::ApplyFrame_BOTH_MESSAGE_Tunnel_DamageOnly(f, ws, err, &missed2) ||
        missed2 != loopback_dmg::kMissedBoth ||
        ws.entities.at(loopback_dmg::kKey).lastDamage != loopback_dmg::kLastDamage) {
        printf("[client] both_ok/tunel falhou: %s\n", err.c_str()); ::close(fd); return false; }
    printf("[client] both_ok: tunel 0x000C APLICOU inner 0x11 (lastDamage=0x%04X, missed2=%zu)\n",
           (unsigned)loopback_dmg::kLastDamage, missed2);

    const mvp::WorldState snap2 = ws;
    if (!RecvFramed(fd, f)) { printf("[client] both_trunc ausente\n"); ::close(fd); return false; }
    const bool applied2 = mvp::ApplyFrame_BOTH_MESSAGE_Tunnel_DamageOnly(f, ws, err, &missed2);
    if (applied2 || err.rfind("0x000C:", 0) != 0 || !SameWorld(ws, snap2)) {
        printf("[client] both_trunc: applied=%d err='%s'\n", (int)applied2, err.c_str());
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
    printf("LOOPBACK DAMAGE_11 OK: spawn(4)->0x11(lastDamage)->miss(1)->trunc sem efeito->tunel 0x000C aplica->tunel trunc sem efeito\n");
    return 0;
}
