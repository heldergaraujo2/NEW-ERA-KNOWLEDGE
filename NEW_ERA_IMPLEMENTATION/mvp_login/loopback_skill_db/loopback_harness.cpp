// NEW-ERA — loopback_harness.cpp (1.3-O) — SKILL TX C1 0xDB + RESP damage (localhost)
// Prova end-to-end LOCAL com socket REAL — SOMENTE 127.0.0.1 (hardcode; sem args).
// Troca RAW (2 mensagens, SEM framing — fluxo TX->RX):
//   CLIENT: ws c/ dummy 0x0100 -> BuildC1_SkillRequestWire(Type=0x0009,
//           (50,60), serial=1, Count=1, key 0x0101, skillSerial=2)
//           -> envia 12 B crus.
//   SERVER: recv 12 B -> memcmp c/ golden (divergencia => offset + abort)
//           -> responde kRespDamage 10 B (damage 0x11 do golden 1.3-N).
//   CLIENT: ApplyFrame_DamageRx_C1(resp) => 0x0100.lastDamage=0x0123.
// TWO-TU: espelho ODR token-idêntico (NÃO EDITAR sem editar o core) +
// prototypes; link com mvp_login_client.cpp (-DNEWERA_MVP_NO_MAIN).
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

// ---- prototypes (link: mvp_login_client.cpp; SEM default args aqui) ----
bool BuildC1_SkillRequestWire(uint16_t skillType, uint8_t x, uint8_t y,
                              uint8_t serial, uint8_t count,
                              const uint16_t* targetKeys, uint8_t skillSerial,
                              std::vector<uint8_t>& out, std::string& err);
bool ApplyFrame_DamageRx_C1(const std::vector<uint8_t>& frame,
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

    printf("[server] conectado; aguardando request de skill (12 B crus)\n");
    uint8_t buf[32];
    if (!RecvAll(c, buf, loopback_skill::kReqLen)) { ::close(c); return; }
    if (std::memcmp(buf, loopback_skill::kReq.data(), loopback_skill::kReqLen) != 0) {
        for (size_t i = 0; i < loopback_skill::kReqLen; ++i)
            if (buf[i] != loopback_skill::kReq[i]) {
                printf("[server] REQ divergente @off %zu: got 0x%02X espera 0x%02X\n",
                       i, buf[i], loopback_skill::kReq[i]); break; }
        ::close(c); return;
    }
    printf("[server] REQ OK (memcmp golden): C1 0C DB Type=0x%04X (%u,%u) ser=%u cnt=%u key=0x%04X ss=%u\n",
           (unsigned)loopback_skill::kSkillType, loopback_skill::kX, loopback_skill::kY,
           (unsigned)loopback_skill::kSerial, (unsigned)loopback_skill::kCount,
           (unsigned)loopback_skill::kTargetKey, (unsigned)loopback_skill::kSkillSerial);
    if (!SendAll(c, loopback_skill::kRespDamage.data(), loopback_skill::kRespLen)) {
        ::close(c); return; }
    printf("[server] RESP damage 10 B enviado\n");
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

    // ws c/ dummy 0x0100 (alvo do RESP; raw 0x8100 -> &0x7FFF -> 0x0100)
    mvp::WorldState ws; ws.clear();
    mvp::EntityRecord dummy{};
    dummy.key = loopback_skill::kDummyKey; dummy.kind = mvp::EntityKind::Character;
    ws.entities[loopback_skill::kDummyKey] = dummy;

    // TX: builder real do request de skill (macro SendRequestMagicAttack)
    const uint16_t keys[1] = { loopback_skill::kTargetKey };
    std::vector<uint8_t> req; std::string err;
    if (!mvp::BuildC1_SkillRequestWire(loopback_skill::kSkillType, loopback_skill::kX,
                                       loopback_skill::kY, loopback_skill::kSerial,
                                       loopback_skill::kCount, keys,
                                       loopback_skill::kSkillSerial, req, err) ||
        req.size() != loopback_skill::kReqLen || !SendAll(fd, req.data(), req.size())) {
        printf("[client] TX falhou: %s\n", err.c_str()); ::close(fd); return false; }
    printf("[client] TX enviado: %zu B (skill 0x%04X @(%u,%u) -> key 0x%04X)\n",
           req.size(), (unsigned)loopback_skill::kSkillType,
           (unsigned)loopback_skill::kX, (unsigned)loopback_skill::kY,
           (unsigned)loopback_skill::kTargetKey);

    // RX: resposta damage 0x11 -> apply
    uint8_t resp[32];
    if (!RecvAll(fd, resp, loopback_skill::kRespLen)) {
        printf("[client] RESP ausente\n"); ::close(fd); return false; }
    std::vector<uint8_t> respVec(resp, resp + loopback_skill::kRespLen);
    size_t missed = 0;
    if (!mvp::ApplyFrame_DamageRx_C1(respVec, ws, err, &missed) || missed != 0) {
        printf("[client] apply RESP falhou: %s\n", err.c_str()); ::close(fd); return false; }
    assert(ws.entities.at(loopback_skill::kDummyKey).lastDamage == loopback_skill::kLastDamage);
    printf("[client] RESP aplicado: 0x%04X lastDamage=0x%04X (missed=%zu)\n",
           (unsigned)loopback_skill::kDummyKey, (unsigned)loopback_skill::kLastDamage, missed);

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
    printf("LOOPBACK SKILL_DB OK: TX 12 B (memcmp golden) -> RESP damage -> lastDamage=0x0123\n");
    return 0;
}
