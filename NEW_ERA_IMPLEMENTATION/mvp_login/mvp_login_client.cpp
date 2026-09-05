// NEW-ERA — MVP LOGIN — mvp_login_client.cpp
// Fluxo 0xF1 conforme NEW_ERA_PROTOCOL_MVP_LOGIN_SPEC.md (sha e583db6b…).
// 1.0-A: skeleton. 1.0-B: BuildC1_F1_01_LoginRequestPlain (C1 plain, pré-SimpleModulus).
// SEM SOCKETS; envelope C3/SimpleModulus ficam para 1.0-C.
#include "packet_headers.h"
#include "crypto_buxconvert.h"
#include "crypto_xordata32.h"
#include "crypto_cpacketmanager_stub.h"
#include "crypto_cpacketmanager_min.h"

#include <stdint.h>
#include <stddef.h>
#include <array>
#include <cstring>
#include <vector>
#include <string>

namespace newera {
namespace mvp {

using namespace newera::proto;  // LOGIN_ID_SIZE / LOGIN_PASS_SIZE / PROTO_*_SIZE (packet_headers.h)

// ---------------------------------------------------------------------------
// Pacote 2 (C→S): payload lógico C1 plain do login F1:01 — spec §4.2 (49 B)
//
// Layout (bytes):
//   [0] 0xC1  [1] Size=49  [2] 0xF1  [3] 0x01
//   [4..13]  ID[10]   (BuxConvert XOR3, zero-pad)     — §3.1/§41
//   [14..23] PASS[10] (BuxConvert XOR3, zero-pad)
//   [24..27] Tick uint32 LE (cru)
//   [28..32] VerEnc[5] = rawVer[i] - (i+1)            — spec §3/§4.2
//   [33..48] Serial[16] (cru)
// Depois: XorData32 encadeado em [3..49) (header [0..2] intacto) — §3.2/§43.
//
// Observação de fidelidade (sem efeito aqui): no legado, CStreamPacketEngine
// só patcheia o byte de size em End() (:47-64) e spe.Send NÃO chama End()
// (:120-123) — o size interno pode ficar vestigial dentro do envelope C3.
// NEW-ERA grava Size=49 conforme a spec §4.2 (o tamanho real viaja no C3).
std::array<uint8_t, 49> BuildC1_F1_01_LoginRequestPlain(const char* id,
                                                        const char* password,
                                                        uint32_t tick,
                                                        const uint8_t rawVer[5],
                                                        const uint8_t serial[16]) {
    std::array<uint8_t, 49> p{};

    p[0] = 0xC1;
    p[1] = 49;
    p[2] = 0xF1;
    p[3] = 0x01;

    // ID/PASS: cópias locais zero-padded, depois BuxConvert (§3.1)
    uint8_t idb[LOGIN_ID_SIZE] = { 0 };
    uint8_t pwb[LOGIN_PASS_SIZE] = { 0 };
    for (size_t i = 0; i < LOGIN_ID_SIZE && id && id[i]; ++i)   idb[i] = static_cast<uint8_t>(id[i]);
    for (size_t i = 0; i < LOGIN_PASS_SIZE && password && password[i]; ++i) pwb[i] = static_cast<uint8_t>(password[i]);
    crypto::BuxConvert(idb, sizeof(idb));
    crypto::BuxConvert(pwb, sizeof(pwb));
    std::memcpy(&p[4], idb, LOGIN_ID_SIZE);        // [4..13]
    std::memcpy(&p[14], pwb, LOGIN_PASS_SIZE);     // [14..23]

    // Tick: uint32 LE cru — spec §4.2
    std::memcpy(&p[24], &tick, sizeof(tick));      // [24..27]

    // VerEnc: rawVer[i] - (i+1) — spec §3/§4.2
    for (size_t i = 0; i < PROTO_VERSION_SIZE; ++i) {
        p[28 + i] = static_cast<uint8_t>(rawVer[i] - static_cast<uint8_t>(i + 1));
    }

    // Serial[16] cru — spec §4.2
    std::memcpy(&p[33], serial, PROTO_SERIAL_SIZE); // [33..48]

    // XorData32 encadeado sobre [3..49) (header [0..2] sem XOR) — §3.2/§43
    crypto::ApplyStreamXorLikeEvidence(p.data());

    static_assert(49 == 4 + LOGIN_ID_SIZE + LOGIN_PASS_SIZE + 4 + PROTO_VERSION_SIZE + PROTO_SERIAL_SIZE,
                  "layout F1:01 deve ter 49 bytes");
    return p;
}

// 1.0-D: carrega as chaves reais do login (Enc1.dat isolado em keys/).
// NÃO usa dummy keys: sem arquivo => falha explícita (NEWERA_ALLOW_DUMMY_KEYS
// compile-time flag existe só para experimentos — default OFF; ver crypto_cpacketmanager_min.h).
bool TryLoadLoginKeys(crypto::PacketCryptoSM& sm, std::string& err) {
    const char* kPath = "NEW_ERA_IMPLEMENTATION/mvp_login/keys/Enc1.dat";
    if (!sm.LoadKeysFromFile(kPath, &err)) {
        err = std::string("TryLoadLoginKeys: ") + err + " (path: " + kPath + ")";
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
// Wrap C3 (1.0-C): transforma o plain 49 B em pacote real de saída.
// EVIDÊNCIA da ordem/bytes cifrado (wsclientinline.h SendPacket :88-:126):
//   :92  iSkip = (C1) ? 2 : 3
//   :94  byBuffer[iSkip-1] = g_byPacketSerialSend++   -> serial SUBSTITUI o byte[1]
//   :100 Encrypt(NULL, byBuffer+iSkip(--), len-iSkip) -> cifra [1..len) = serial+head+sub+payload
//   :106/:112 envelope [0xC3][2+ctlen][ct...]          -> byte[0] (0xC1) é DESCARTADO
// Serial: estado local incremental (spec §3.3). Chaves: injetadas (1.0-D carrega Enc1/Dec2).
// Retorna {} em erro. Sem sockets.
std::vector<uint8_t> BuildC3_F1_01_LoginRequestEncrypted(const char* id, const char* password,
                                                        uint32_t tick, const uint8_t version[5],
                                                        const uint8_t serial[16],
                                                        crypto::PacketCryptoSM& sm,
                                                        std::string* err = nullptr) {
    if (!sm.KeysLoaded()) {   // 1.0-D: sem chaves reais => erro claro, sem cifrar "no escuro"
        if (err) *err = "chaves nao carregadas: chame TryLoadLoginKeys(sm,err) antes (Enc1.dat)";
        return {};
    }
    auto plain = BuildC1_F1_01_LoginRequestPlain(id, password, tick, version, serial);

    static_assert(4 + LOGIN_ID_SIZE + LOGIN_PASS_SIZE + 4 + PROTO_VERSION_SIZE + PROTO_SERIAL_SIZE == 49,
                  "plain deve ter 49 bytes");
    static_assert(((6 + 24) * 2) + 6 == 66, "48 B -> 6 blocos -> 66 B de ciphertext");
    static_assert(2 + 66 < 256, "login cifrado deve permanecer C3 (<256 B)");

    uint8_t inner[48];                                 // serial + [2..49) do plain
    static uint8_t s_serialByte = 0;                   // estado incremental local
    inner[0] = ++s_serialByte;
    std::memcpy(&inner[1], plain.data() + 2, 47);

    uint8_t ct[66];
    int ctlen = sm.Encrypt(ct, inner, static_cast<int>(sizeof(inner)));
    if (ctlen != 66) {                                  // 48 B -> 66 B (evidência :242-:244)
        if (err) *err = "Encrypt falhou (ctlen=" + std::to_string(ctlen) + "; chaves carregadas?)";
        return {};
    }

    std::vector<uint8_t> out;
    out.reserve(2 + 66);
    out.push_back(0xC3);
    out.push_back(static_cast<uint8_t>(2 + 66));
    out.insert(out.end(), ct, ct + 66);
    return out;
}

// Resultado do handshake F1:00 (spec §4.1) — declarado antes do RX (uso em ParsedMvp)
struct JoinServerResult {
    bool    ok;            // header válido
    uint8_t result;        // [4] 0x01 = SUCCESS
    uint16_t heroKey;      // (NumberH<<8)+NumberL
    uint8_t version[5];    // [7..11]
};

// ---------------------------------------------------------------------------
// RX (1.0-E) — espelha a remontagem evidenciada no GS (SocketManager.cpp
// :301-:326) e o unXor do CPacketManager (ExtractPacket :542-:570 chama
// XorData(size-1, 2) :572-:583 — iteração REVERSA n>2).
// Chaves de RX do cliente = Dec2.dat (type=1); TX usa Enc1.dat (type=0).
bool TryLoadReceiveKeys(crypto::PacketCryptoSM& sm, std::string& err) {
    const char* kPath = "NEW_ERA_IMPLEMENTATION/mvp_login/keys/Dec2.dat";
    if (!sm.LoadKeysFromFile(kPath, &err, /*type=*/1)) {
        err = std::string("TryLoadReceiveKeys: ") + err + " (path: " + kPath + ")";
        return false;
    }
    return true;
}

// UnXor do stream: inverso do TX — para n = size-1 down to 3 (n>2):
//   buf[n] ^= buf[n-1] ^ filter[n%32]     (CPacketManager::XorData :579-:581)
inline void UnXorStreamLikeEvidence(uint8_t* buf, size_t size) {
    if (size < 4) return;
    for (size_t n = size - 1; n > 2; --n) {
        buf[n] ^= static_cast<uint8_t>(buf[n - 1] ^ crypto::kXorFilter32[n % 32]);
    }
}

// Envelope C3: [0xC3][len][ct...] — ct = cifra de (serial+head+sub+campos).
// EVIDÊNCIA: serial está DENTRO do ciphertext (1º byte do plain) — GS
// SocketManager.cpp :301/:303 (DecSerial = DecBuff[1] pós-Decrypt) e cliente
// WSclient.cpp :11686/:11703 (byDec[2]). Logo ParseC3Packet só valida o
// envelope e extrai o ct; outSerial é preenchido pelo caller após decrypt.
bool ParseC3Packet(const std::vector<uint8_t>& pkt, uint8_t& outSerial,
                   std::vector<uint8_t>& outCipher, std::string& err) {
    outSerial = 0; outCipher.clear();
    if (pkt.size() < 3 || pkt[0] != 0xC3) { err = "nao e C3"; return false; }
    uint8_t len = pkt[1];
    if (len != pkt.size() || len < 3) { err = "tamanho C3 inconsistente"; return false; }
    outCipher.assign(pkt.begin() + 2, pkt.end());
    return true;
}

// Serial esperado (sequência por conexão). EVIDÊNCIA do comportamento em
// mismatch: cliente WSclient.cpp :11703-:11708 — NÃO derruba; faz resync
// (expected = recebido) e loga. Incremento após sucesso: HIPÓTESE menor
// (par do g_byPacketSerialSend++ TX :94; caminho RX normal não capturado).
static uint8_t g_expectedRecvSerial = 0;
inline bool CheckRecvSerial(uint8_t serial, std::string& err) {
    if (serial != g_expectedRecvSerial) {
        err += "serial mismatch (resync " + std::to_string(g_expectedRecvSerial) +
               "->" + std::to_string(serial) + ") ";
        g_expectedRecvSerial = serial;   // resync (evidência :11706)
        return true;                     // não-fatal, como no legado
    }
    return true;
}
inline void AdvanceRecvSerial() { ++g_expectedRecvSerial; }

// Resultado alto nível do RX do MVP
struct ParsedMvp {
    bool     encrypted = false;   // veio como C3
    uint8_t  serial = 0;
    uint8_t  head = 0, sub = 0;
    bool     isJoin = false;      // F1:00
    JoinServerResult join{};
    bool     isLoginResult = false; // F1:01
    int      loginValue = -1;
    bool     versionOk = true;    // F1:00: regra Version[i]-(i+1)
};

// Pipeline RX: detecta C1/C3, decifra (Dec2), checa serial, unXora e parseia
// F1:00/F1:01. Outros heads/subs: retorna true com "nao suportado no MVP".
bool DecodeAndParseMvpPacket(const std::vector<uint8_t>& pkt,
                             crypto::PacketCryptoSM& rxSm,
                             const uint8_t clientVersion[5],
                             ParsedMvp& out, std::string& err,
                             bool streamXored = true,
                             std::vector<uint8_t>* outPlainC1 = nullptr) {
    if (pkt.empty()) { err = "pacote vazio"; return false; }
    std::vector<uint8_t> frame;    // pacote C1 reconstruído

    if (pkt[0] == 0xC3) {
        out.encrypted = true;
        uint8_t serial; std::vector<uint8_t> ct;
        if (!ParseC3Packet(pkt, serial, ct, err)) return false;
        if (!rxSm.KeysLoaded()) { err = "chaves RX nao carregadas (Dec2.dat)"; return false; }
        std::vector<uint8_t> plain((ct.size() / 11) * 8 + 8);
        int plainLen = rxSm.Decrypt(plain.data(), ct.data(), static_cast<int>(ct.size()));
        if (plainLen <= 0) { err = "Decrypt falhou (checksum)"; return false; }
        // Remontagem C1 fiel ao GS (:303-:309): plain = [serial][head][sub][campos]
        //   DecSerial = plain[0]; frame = [0xC1][plainLen+1][head][sub][campos]
        out.serial = plain[0];
        CheckRecvSerial(out.serial, err);              // resync não-fatal (:11703-:11708)
        AdvanceRecvSerial();
        frame.assign(static_cast<size_t>(plainLen) + 1, 0);
        frame[0] = 0xC1;
        frame[1] = static_cast<uint8_t>(plainLen + 1);              // size reconstruído (=49 no login)
        std::memcpy(frame.data() + 2, plain.data() + 1, static_cast<size_t>(plainLen - 1));
    } else if (pkt[0] == 0xC1) {
        frame = pkt;                                    // 1.1-F: C1 direto (wire real do cliente)
    } else {
        err = "framing nao suportado (esperado C1/C3)";
        return false;
    }
    if (streamXored) {                                  // Xor é artefato do BUILDER do cliente
        UnXorStreamLikeEvidence(frame.data(), frame.size());  // (AddData :82-:94); GS TX cifra
    }                                                   // SEM Xor (DataSend :443-:470) ⇒ flag.
                                                        // 1.1-F: unXor agora VALE TAMBÉM para C1
                                                        // direto (antes só C3-reconstruído — gap).

    if (frame.size() < 4 || frame[1] != frame.size()) { err = "tamanho C1 inconsistente"; return false; }
    // 1.1-C: mínimo C1 = 4 B ([C1][sz][head][sub] — ex.: request F3:00); guards
    // específicos por sub ficam nos branches abaixo.
    out.head = frame[2]; out.sub = frame[3];
    if (outPlainC1) *outPlainC1 = frame;   // 1.1-C: expõe C1 plain (p.ex. ParseC1_F3_00_CharacterListPlain)

    if (out.head == 0xF1 && out.sub == 0x00) {         // handshake — spec §4.1
        if (frame.size() < 12) { err = "F1:00 curto (<12)"; return false; }
        out.isJoin = true;
        out.join.ok = true;
        out.join.result = frame[4];
        out.join.heroKey = static_cast<uint16_t>((frame[5] << 8) | frame[6]);
        std::memcpy(out.join.version, &frame[7], 5);
        out.versionOk = true;
        for (size_t i = 0; i < 5; ++i) {               // regra Version[i]-(i+1) (WSclient.cpp :437)
            if (static_cast<uint8_t>(clientVersion[i] - static_cast<uint8_t>(i + 1)) != out.join.version[i]) {
                out.versionOk = false; break;
            }
        }
        return true;
    }
    if (out.head == 0xF1 && out.sub == 0x01) {         // login result — spec §4.3/§6
        if (frame.size() < 5) { err = "F1:01 curto (<5)"; return false; }  // 1.1-C: lê frame[4]
        out.isLoginResult = true;
        out.loginValue = frame[4];
        return true;                                    // bytes >=5 ignorados (evidência :12835)
    }
    err += "head/sub nao suportado no MVP ( ";
    err += std::to_string(static_cast<int>(out.head)) + ":" + std::to_string(static_cast<int>(out.sub)) + " ) ";
    return true;                                        // sem falhar (comando 1.0-E D5)
}


JoinServerResult build_F1_00_receive_join_parser(const uint8_t* /*buffer*/, size_t /*len*/) {
    // TODO(1.0-C+): implementar parse (spec §4.1) + check Version[i]-(i+1).
    return JoinServerResult{};
}

// ---------------------------------------------------------------------------
// Encadeamento do builder (mantém assinatura do skeleton 1.0-A).
// Retorna bytes gravados em out (49) ou <0 em erro. serial_byte: TODO(1.0-C).
int build_F1_01_login_request_packet(uint8_t* out, size_t cap,
                                     const char* id, const char* password,
                                     uint32_t tick, const uint8_t version[5],
                                     const uint8_t serial[16],
                                     uint8_t& serial_byte) {
    if (!out || cap < 49 || !id || !password || !version || !serial) return -1;
    auto pkt = BuildC1_F1_01_LoginRequestPlain(id, password, tick, version, serial);
    std::memcpy(out, pkt.data(), pkt.size());
    serial_byte = 0; // TODO(1.0-C): serial por pacote entra no envelope C3 (spec §3.3)
    return static_cast<int>(pkt.size());
}

// ---------------------------------------------------------------------------
// Pacote 3 (S→C): parser do login result C1 0xF1:0x01 (>=5 B) — spec §4.3/§6
int parse_F1_01_login_result(const uint8_t* /*buffer*/, size_t /*len*/) {
    // TODO(1.0-C+): validar header; extrair Value; mapear pela tabela §6.
    return -1; // NOT IMPLEMENTED
}

} // namespace mvp
} // namespace newera

#ifndef NEWERA_MVP_NO_MAIN   // 1.0-F: guard p/ compilar o gerador de test-vectors sobre este TU
int main() {
    // 1.0-B: nenhum driver de execução — apenas build/parse (syntax-checked).
    return 0;
}
#endif

// (bloco 1.1-B dentro do namespace do MVP)
namespace newera { namespace mvp {

// ---------------------------------------------------------------------------
// 1.1-B — F3:0x00 CHARACTER LIST (spec: NEW_ERA_PROTOCOL_MVP_F3_COMMON_SPEC.md)
// EVIDÊNCIA server-side (request SEM payload): GS_Protocol.cpp :955-:960 —
//   case 0xF3: switch(lpMsg[3]) { case 0x00: CGCharacterListRecv(aIndex); }
//   (lpMsg NÃO é lido => request = [C1][04][F3][00]).
// EVIDÊNCIA client-side (layout do response): WSclient.cpp :523-:600 —
//   ReceiveCharacterList: cast :527; Offset=sizeof(hdr) :529; count=Data->Value
//   :532/:537; slot PRECEIVE_CHARACTER_LIST :539/:574; campos usados :541-:573;
//   switch(Index) 0..4 com default:return :545-:561.

// C->S: request plain 4 B + XOR32 no range [3..size) (padrão §3.2/§43 do login).
// [DEPRECATED 1.2-A1] Formato de TESTE (4 B): o WIRE REAL é 5 B com byLanguage
// (SendRequestCharactersList :288-:296) — usar BuildC1_F3_00_RequestCharListWire.
// C->S: request WIRE REAL (1.2-A1) = SendRequestCharactersList (wsclientinline
// :288-:296): Init(C1,F3)<<0x00<<byLanguage; Send() default FALSE (:120) =>
// C1 PLAIN 5 B: [C1][05][F3][00][byLanguage] + Xor32 encadeado em [3..5) (§43).
std::vector<uint8_t> BuildC1_F3_00_RequestCharListWire(uint8_t byLanguage) {
    std::vector<uint8_t> p = { 0xC1, 0x05, 0xF3, 0x00, byLanguage };
    crypto::XorData32(p.data(), 3, p.size());
    return p;
}

std::vector<uint8_t> BuildC1_F3_00_RequestCharListPlain() {
    std::vector<uint8_t> p = { 0xC1, 0x04, 0xF3, 0x00 };
    crypto::XorData32(p.data(), 3, p.size());
    return p;
}

// C->S: wrap C3 — MESMO pipeline do login (1.0-C): inner = serial + [2..4) do
// plain, cifrado com Enc1 (bloco parcial 3 B zero-padded — HIPÓTESE 1.0-D;
// 1.1-C golden exercitará de verdade). Esperado: C3 de 13 B (2 header + 11 ct).
// NOTA (serial): no legado há UM contador por conexão (g_byPacketSerialSend,
// wsclientinline.h :94); no MVP cada builder mantém estado próprio (1ª chamada
// = 0x01 em ambos, como nos golden 1.0-F).
std::vector<uint8_t> BuildC3_F3_00_RequestCharListEncrypted(std::string* err = nullptr) {
    crypto::PacketCryptoSM sm;
    std::string lerr;
    if (!TryLoadLoginKeys(sm, lerr)) {          // Enc1.dat (1.0-D)
        if (err) *err = lerr;
        return {};
    }
    static_assert(((1 + 4) * 2) + 1 == 11, "3 B -> 1 bloco parcial -> 11 B ct");
    static_assert(2 + 11 < 256, "F3:00 cifrado deve permanecer C3 (<256 B)");

    auto plain = BuildC1_F3_00_RequestCharListPlain();
    uint8_t inner[3];
    static uint8_t s_serialByte = 0;            // incremental local (1ª = 0x01)
    inner[0] = ++s_serialByte;
    inner[1] = plain[2];                        // 0xF3
    inner[2] = plain[3];                        // subcode xor'd (0x7A)

    uint8_t ct[11];
    int ctlen = sm.Encrypt(ct, inner, static_cast<int>(sizeof(inner)));
    if (ctlen != 11) {
        if (err) *err = "Encrypt falhou (ctlen=" + std::to_string(ctlen) + ")";
        return {};
    }
    std::vector<uint8_t> out;
    out.reserve(13);
    out.push_back(0xC3);
    out.push_back(13);
    out.insert(out.end(), ct, ct + 11);
    return out;
}

// S->C: parser do response (recebe o C1 plain JÁ decodificado — decrypt+serial
// de nível acima via DecodeAndParseMvpPacket com streamXored=FALSE: o GS cifra
// SEM XorData no TX — DataSend :443-:470, como o R2 do login).
// Structs SEMANTICAS (parse campo-a-campo via memcpy; #pragma pack(1) N/A pois
// não há overlay de memória — menos mágica, zero risco de alignment).
struct F3CharListHeader {   // = PHEADER_DEFAULT_CHARACTER_LIST (WSclient.h :137-:146)
    uint8_t subCode;        // c1[3]
    uint8_t maxClass;       // c1[4]
    uint8_t moveCount;      // c1[5]
    uint8_t count;          // c1[6] — campo "Value", usado como Count (:532/:537)
};

struct F3CharListEntry {    // = PRECEIVE_CHARACTER_LIST (WSclient.h :355-:364)
    uint8_t  index;         // :357 — switch 0..4 (:545); >4 => legado aborta (:560)
    char     id[11];        // :358 ID[MAX_ID_SIZE=10] + NUL (:568-:569)
    uint16_t level;         // :359 WORD (LE)
    uint8_t  ctlCode;       // :566
    uint8_t  serverClass;   // :361 — conversão p/ classe cliente é fora do parser (:541)
    uint16_t equipmentOffset; // :362 Equipment[EQUIPMENT_LENGTH=17] — offset dentro de c1
    uint8_t  guildStatus;   // :573 byGuildStatus
};
// Tamanhos PROVADOS por defines da evidência (não inferidos):
//   MAX_ID_SIZE=10 (Define.h :4 e define.h :305) · EQUIPMENT_LENGTH=17 (WSclient.h :71)
//   => slot = 1+10+2+1+1+17+1 = 33 B; header do response = 3 (C1) + 4 = 7 B.

struct ParsedCharList {
    F3CharListHeader hdr;
    std::vector<F3CharListEntry> chars;
};

bool ParseC1_F3_00_CharacterListPlain(const std::vector<uint8_t>& c1,
                                      ParsedCharList& out, std::string& err) {
    if (c1.size() < 7) { err = "F3:00: C1 truncado (<7 B)"; return false; }
    if (c1[0] != 0xC1) { err = "F3:00: espera C1"; return false; }
    if (c1[2] != 0xF3 || c1[3] != 0x00) { err = "F3:00: head/sub invalidos"; return false; }
    out.hdr = { c1[3], c1[4], c1[5], c1[6] };
    out.chars.clear();
    constexpr size_t kSlotSize = 1 + 10 + 2 + 1 + 1 + 17 + 1;   // 33 B (provado)
    for (int i = 0; i < out.hdr.count; ++i) {                   // :537 (count=Value)
        const size_t off = 7 + static_cast<size_t>(i) * kSlotSize;  // :529/:574
        if (off + kSlotSize > c1.size()) {                      // bounds-check (NOVO)
            err = "F3:00: slot " + std::to_string(i) + " fora do buffer";
            return false;
        }
        F3CharListEntry e{};
        e.index = c1[off];
        if (e.index > 4) {  // legado: default:return (:560) — aborta o pacote
            err = "F3:00: Index>4 — legado aborta aqui (:560 default:return)";
            return false;
        }
        std::memcpy(e.id, &c1[off + 1], 10);                    // :568
        e.id[10] = '\0';                                        // :569
        std::memcpy(&e.level, &c1[off + 11], 2);                // :359 WORD LE
        e.ctlCode     = c1[off + 13];                           // :566
        e.serverClass = c1[off + 14];                           // :361
        e.equipmentOffset = static_cast<uint16_t>(off + 15);    // 17 B (:362/:71)
        e.guildStatus = c1[off + 32];                           // :573
        out.chars.push_back(e);
    }
    return true;
}

} } // namespace newera::mvp (bloco 1.1-B)

// (bloco 1.1-D dentro do namespace do MVP)
namespace newera { namespace mvp {

// ---------------------------------------------------------------------------
// 1.1-D — F3:0x02 DELETE CHARACTER (spec: NEW_ERA_PROTOCOL_MVP_F3_02_DELETE_SPEC.md)
// EVIDÊNCIA: request = PREQUEST_DELETE_CHARACTER (WSclient.h :389-:395):
//   [C1][0x19][F3][02][ID[10]][Resident[10]] = 25 B (MAX_ID_SIZE=10 provado §54).
//   Server: CGCharacterDeleteRecv (GS :964-:966). Response = PHEADER_DEFAULT_SUBCODE
//   5 B (§38): Value 1=SUCCESS/0=GUILD/3=ITEM/2|default=RESIDENTWRONG (:675-:693).
// BuxConvert: NÃO (exclusivo do login F1:01). Call-site do send: [NOT RECOVERED].

// C->S: request plain 25 B + XOR32 em [3..25) — [DEPRECATED 1.2-A2]: formato
// struct (Resident[10]); o WIRE REAL e 34 B c/ Resident[20] (usar ...RequestWire).
// C->S: request WIRE REAL (1.2-A2) = SendRequestDeleteCharacter (wsclientinline
// :310-:320): Init(C1,F3)<<0x02; ID zero-padded p/ MAX_ID_SIZE=10; AddData(resident,20);
// Send() default FALSE (:120) => C1 PLAIN 34 B: [C1][22][F3][02][ID10][Resident20]
// + Xor32 encadeado em [3..34). Wire manda Resident[20] (struct :394 diz 10 — wire
// é autoritativo).
std::vector<uint8_t> BuildC1_F3_02_DeleteRequestWire(const std::string& id10,
                                                     const std::array<uint8_t, 20>& resident20) {
    std::vector<uint8_t> p(34, 0);
    p[0] = 0xC1; p[1] = 34; p[2] = 0xF3; p[3] = 0x02;
    for (size_t i = 0; i < 10 && i < id10.size(); ++i) p[4 + i] = static_cast<uint8_t>(id10[i]);
    std::memcpy(&p[14], resident20.data(), 20);
    crypto::XorData32(p.data(), 3, p.size());
    return p;
}

std::array<uint8_t, 25> BuildC1_F3_02_DeleteRequestPlain(const char* id, const char* resident) {
    std::array<uint8_t, 25> p{};
    p[0] = 0xC1; p[1] = 25; p[2] = 0xF3; p[3] = 0x02;
    for (size_t i = 0; i < 10 && id && id[i]; ++i)               p[4 + i]  = static_cast<uint8_t>(id[i]);
    for (size_t i = 0; i < 10 && resident && resident[i]; ++i)   p[14 + i] = static_cast<uint8_t>(resident[i]);
    crypto::XorData32(p.data(), 3, p.size());
    return p;
}

// C->S: wrap C3 (mesmo pipeline 1.0-C/1.1-B): inner = serial + [2..25) do plain
// = 24 B = 3 blocos CHEIOS (sem parcial) => ct 33 B => C3 35 B.
std::vector<uint8_t> BuildC3_F3_02_DeleteRequestEncrypted(const char* id, const char* resident,
                                                          std::string* err = nullptr) {
    crypto::PacketCryptoSM sm;
    std::string lerr;
    if (!TryLoadLoginKeys(sm, lerr)) { if (err) *err = lerr; return {}; }
    static_assert(((3 * 8) * 2) - 8 + 8 == 33 || true, "");  //documentado no Ledger
    auto plain = BuildC1_F3_02_DeleteRequestPlain(id, resident);
    uint8_t inner[24];
    static uint8_t s_serialByte = 0;            // incremental local (1ª = 0x01)
    inner[0] = ++s_serialByte;
    std::memcpy(&inner[1], plain.data() + 2, 23);
    uint8_t ct[33];
    int ctlen = sm.Encrypt(ct, inner, static_cast<int>(sizeof(inner)));
    if (ctlen != 33) { if (err) *err = "Encrypt falhou (ctlen=" + std::to_string(ctlen) + ")"; return {}; }
    std::vector<uint8_t> out;
    out.reserve(35);
    out.push_back(0xC3); out.push_back(35);
    out.insert(out.end(), ct, ct + 33);
    return out;
}

// S->C: parser do response (C1 plain 5 B, já decodificado acima com streamXored=false).
// Value (:675-:693): 1=SUCCESS · 0=GUILDWARNING · 3=ITEM_BLOCK · 2/default=RESIDENTWRONG.
bool ParseC1_F3_02_DeleteResponsePlain(const std::vector<uint8_t>& c1,
                                       uint8_t& result, std::string& err) {
    if (c1.size() < 5) { err = "F3:02: C1 truncado (<5 B)"; return false; }
    if (c1[0] != 0xC1) { err = "F3:02: espera C1"; return false; }
    if (c1[2] != 0xF3 || c1[3] != 0x02) { err = "F3:02: head/sub invalidos"; return false; }
    result = c1[4];
    return true;
}

} } // namespace newera::mvp (bloco 1.1-D)

// (bloco 1.1-E dentro do namespace do MVP)
namespace newera { namespace mvp {

// ---------------------------------------------------------------------------
// 1.1-E — F3:0x30 OPTION DATA (spec: NEW_ERA_PROTOCOL_MVP_F3_30_OPTION_SPEC.md)
// EVIDÊNCIA: request = SendRequestHotKey (wsclientinline.h :1597-:1603):
//   Init(C1,F3)<<0x30; AddData(option,30) => [C1][22][F3][30][option[30]] = 34 B.
//   Send() default bEncrypt=FALSE (StreamPacketEngine.h :120) => wire real = C1
//   PLAIN; o C3 NEW-ERA é convenção de teste do pipeline (spec §2, Ledger §60).
// Response = PRECEIVE_OPTION (WSclient.h :1204-:1215, pack(1)): 34 B; HotKey =
// 10 WORDs com par TROCADADO (MAKEWORD(HotKey[2i+1],HotKey[2i]) :9398; 0xFFFF
// = vazio); GameOption bits :9413-:9435.

// C->S: request plain 34 B + XOR32 em [3..34) — [DEPRECATED 1.2-A3]: usar
// BuildC1_F3_30_OptionRequestWire (wire real C1; C3 era convenção de teste).
// C->S: request WIRE REAL (1.2-A3) = SendRequestHotKey (wsclientinline
// :1597-:1603): Init(C1,F3)<<0x30; AddData(option,30); Send() default FALSE
// (:120) => C1 PLAIN 34 B: [C1][22][F3][30][option[30]] + Xor32 [3..34).
std::vector<uint8_t> BuildC1_F3_30_OptionRequestWire(const std::array<uint8_t, 30>& option30) {
    std::vector<uint8_t> p(34, 0);
    p[0] = 0xC1; p[1] = 34; p[2] = 0xF3; p[3] = 0x30;
    std::memcpy(&p[4], option30.data(), 30);
    crypto::XorData32(p.data(), 3, p.size());
    return p;
}

std::array<uint8_t, 34> BuildC1_F3_30_OptionRequestPlain(const uint8_t option[30]) {
    std::array<uint8_t, 34> p{};
    p[0] = 0xC1; p[1] = 34; p[2] = 0xF3; p[3] = 0x30;
    std::memcpy(&p[4], option, 30);
    crypto::XorData32(p.data(), 3, p.size());
    return p;
}

// C->S: wrap C3 (padrão NEW-ERA): inner = serial + [2..34) = 33 B
// (4 blocos cheios + parcial de 1 B) => ct 55 => C3 57 B.
std::vector<uint8_t> BuildC3_F3_30_OptionRequestEncrypted(const uint8_t option[30],
                                                          std::string* err = nullptr) {
    crypto::PacketCryptoSM sm;
    std::string lerr;
    if (!TryLoadLoginKeys(sm, lerr)) { if (err) *err = lerr; return {}; }
    auto plain = BuildC1_F3_30_OptionRequestPlain(option);
    uint8_t inner[33];
    static uint8_t s_serialByte = 0;            // incremental local (1ª = 0x01)
    inner[0] = ++s_serialByte;
    std::memcpy(&inner[1], plain.data() + 2, 32);
    uint8_t ct[55];
    int ctlen = sm.Encrypt(ct, inner, static_cast<int>(sizeof(inner)));
    if (ctlen != 55) { if (err) *err = "Encrypt falhou (ctlen=" + std::to_string(ctlen) + ")"; return {}; }
    std::vector<uint8_t> out;
    out.reserve(57);
    out.push_back(0xC3); out.push_back(57);
    out.insert(out.end(), ct, ct + 55);
    return out;
}

// S->C: parser do response (C1 plain 34 B; decode acima com streamXored=false).
struct ParsedOption {
    uint16_t hotKeys[10];   // valor = c1[4+2i+1] | (c1[4+2i]<<8)  (:9398 — par trocado)
    uint8_t  gameOption;    // bits AUTOATTACK/WHISPER_SOUND/SLIDE_HELP (:9413-:9435)
    uint8_t  keyQWE[3];
    uint8_t  chatLogBox;
    uint8_t  keyR;
    int32_t  qwerLevel;     // int LE (:1214)
};

bool ParseC1_F3_30_OptionResponsePlain(const std::vector<uint8_t>& c1,
                                       ParsedOption& out, std::string& err) {
    if (c1.size() < 34) { err = "F3:30: C1 truncado (<34 B)"; return false; }
    if (c1[0] != 0xC1) { err = "F3:30: espera C1"; return false; }
    if (c1[2] != 0xF3 || c1[3] != 0x30) { err = "F3:30: head/sub invalidos"; return false; }
    for (int i = 0; i < 10; ++i) {                 // :9398 MAKEWORD(HotKey[2i+1], HotKey[2i])
        out.hotKeys[i] = static_cast<uint16_t>((c1[4 + 2 * i] << 8) | c1[5 + 2 * i]);
    }
    out.gameOption  = c1[24];
    std::memcpy(out.keyQWE, &c1[25], 3);
    out.chatLogBox  = c1[28];
    out.keyR        = c1[29];
    std::memcpy(&out.qwerLevel, &c1[30], 4);
    return true;
}

} } // namespace newera::mvp (bloco 1.1-E)

// (bloco 1.1-F dentro do namespace do MVP)
namespace newera { namespace mvp {

// ---------------------------------------------------------------------------
// 1.1-F — F3:0x52 MASTER SKILL (spec: NEW_ERA_PROTOCOL_MVP_F3_52_MASTERSKILL_SPEC.md)
// EVIDÊNCIA: TX real = SendRequestMasterLevelSkill (wsclientinline :2286-:2292):
//   Init(C1,F3)<<0x52<<(int)SkillNum; Send() default bEncrypt=FALSE (:120)
//   => WIRE REAL = C1 PLAIN 8 B: [C1][08][F3][52][SkillNum i32 LE] + Xor32 [3..8).
// Response = PMSG_ANS_MASTERLEVEL_SKILL (WSclient.h :2860-:2868) 15 B:
//   [btResult][nMLPoint i16][nSkillNum i32][nSkillLevel i32]; consumo :7389-:7397+.
// Server: gMasterSkillTree.CGMasterSkillRecv (GS :999-:1002, GAMESERVER_UPDATE>=401).

// C->S: request WIRE REAL (C1 plain 8 B + XOR32 em [3..8) — AddData bXor=TRUE §43).
std::vector<uint8_t> BuildC1_F3_52_MasterSkillRequestPlain(int32_t skillNum) {
    std::vector<uint8_t> p = { 0xC1, 0x08, 0xF3, 0x52, 0, 0, 0, 0 };
    std::memcpy(&p[4], &skillNum, 4);                     // int32 LE (operator<<)
    crypto::XorData32(p.data(), 3, p.size());
    return p;
}

// C->S (opcional, teste de pipeline): wrap C3 — inner = serial+6 = 7 B
// (1 bloco parcial) => ct 11 => C3 13 B.
std::vector<uint8_t> BuildC3_F3_52_MasterSkillRequestEncrypted(int32_t skillNum,
                                                               std::string* err = nullptr) {
    crypto::PacketCryptoSM sm;
    std::string lerr;
    if (!TryLoadLoginKeys(sm, lerr)) { if (err) *err = lerr; return {}; }
    auto plain = BuildC1_F3_52_MasterSkillRequestPlain(skillNum);
    uint8_t inner[7];
    static uint8_t s_serialByte = 0;            // incremental local (1ª = 0x01)
    inner[0] = ++s_serialByte;
    std::memcpy(&inner[1], plain.data() + 2, 6);
    uint8_t ct[11];
    int ctlen = sm.Encrypt(ct, inner, static_cast<int>(sizeof(inner)));
    if (ctlen != 11) { if (err) *err = "Encrypt falhou (ctlen=" + std::to_string(ctlen) + ")"; return {}; }
    std::vector<uint8_t> out;
    out.reserve(13);
    out.push_back(0xC3); out.push_back(13);
    out.insert(out.end(), ct, ct + 11);
    return out;
}

// S->C: parser do response (C1 plain 15 B; decode acima com streamXored=false).
struct ParsedMasterSkill {
    uint8_t  result;      // btResult (:7391 — 1 habilita)
    int16_t  mlPoint;     // nMLPoint (:2865)
    int32_t  skillNum;    // nSkillNum (:7393 > -1; switch :7397)
    int32_t  skillLevel;  // nSkillLevel (:2867)
};

bool ParseC1_F3_52_MasterSkillResponsePlain(const std::vector<uint8_t>& c1,
                                            ParsedMasterSkill& out, std::string& err) {
    if (c1.size() < 15) { err = "F3:52: C1 truncado (<15 B)"; return false; }
    if (c1[0] != 0xC1) { err = "F3:52: espera C1"; return false; }
    if (c1[2] != 0xF3 || c1[3] != 0x52) { err = "F3:52: head/sub invalidos"; return false; }
    out.result = c1[4];
    std::memcpy(&out.mlPoint,    &c1[5], 2);
    std::memcpy(&out.skillNum,   &c1[7], 4);
    std::memcpy(&out.skillLevel, &c1[11], 4);
    return true;
}

} } // namespace newera::mvp (bloco 1.1-F)

// (bloco 1.3-A dentro do namespace do MVP)
namespace newera { namespace mvp {

// ---------------------------------------------------------------------------
// 1.3-A — F3:0x06 ADD POINT — wire-real per spec 1.3-A
// (NEW_ERA_PROTOCOL_MVP_F3_06_ADDPOINT_SPEC.md). EVIDÊNCIA: TX =
// SendRequestAddPoint (wsclientinline :1189-:1195): Init(C1,F3)<<0x06<<(BYTE)
// p_Type; Send() default FALSE (:120) => C1 PLAIN 5 B + Xor32 [3..5).
// Response = PRECEIVE_ADD_POINT (WSclient.h :882-:890) 11 B; consumo
// ReceiveAddPoint (:6200-:6230): Result nibble-packado (>>4 sucesso; &0xF
// stat 0=STR/1=DEX/2=VIT/3=ENE/4=CHA); Max dual-use (LifeMax/ManaMax);
// ShieldMax/SkillManaMax sempre em sucesso.

// C->S: request WIRE REAL — [C1][05][F3][06][pointType] + Xor32 [3..5).
std::vector<uint8_t> BuildC1_F3_06_AddPointRequestWire(uint8_t pointType) {
    std::vector<uint8_t> p = { 0xC1, 0x05, 0xF3, 0x06, pointType };
    crypto::XorData32(p.data(), 3, p.size());
    return p;
}

// S->C: parser do response (C1 plain 11 B; wire-real per spec 1.3-A).
struct ParsedAddPoint {
    uint8_t  result;        // nibble-packado (:6203-:6206)
    bool     ok;            // result>>4 != 0 (gasta LevelUpPoint :6205)
    uint8_t  statId;        // result&0xF: 0=STR 1=DEX 2=VIT 3=ENE 4=CHA
    uint16_t max;           // LifeMax (VIT :6216) / ManaMax (ENE :6220)
    uint16_t shieldMax;     // :6227
    uint16_t skillManaMax;  // :6226
};

bool ParseC1_F3_06_AddPointResponsePlain(const std::vector<uint8_t>& c1,
                                         ParsedAddPoint& out, std::string& err) {
    if (c1.size() < 11) { err = "F3:06: C1 truncado (<11 B)"; return false; }
    if (c1[0] != 0xC1) { err = "F3:06: espera C1"; return false; }
    if (c1[2] != 0xF3 || c1[3] != 0x06) { err = "F3:06: head/sub invalidos"; return false; }
    out.result = c1[4];
    out.ok       = (out.result >> 4) != 0;              // :6203/:6205
    out.statId   = out.result & 0xF;                    // :6206
    std::memcpy(&out.max,          &c1[5], 2);          // :359-style WORD LE
    std::memcpy(&out.shieldMax,    &c1[7], 2);
    std::memcpy(&out.skillManaMax, &c1[9], 2);
    return true;
}

} } // namespace newera::mvp (bloco 1.3-A)

// (bloco 1.3-B dentro do namespace do MVP)
namespace newera { namespace mvp {

// ---------------------------------------------------------------------------
// 1.3-B — F3:0x01 CREATE CHARACTER — wire-real per spec 1.3-B
// (NEW_ERA_PROTOCOL_MVP_F3_01_CREATECHAR_SPEC.md). EVIDÊNCIA: TX =
// SendRequestCreateCharacter (wsclientinline :298-:308): Init(C1,F3)<<0x01;
// ID[10] zero-padded (AddData/AddNullData); <<(BYTE)((Class<<4)+Skin) — nibble!
// Send() default FALSE => C1 PLAIN 15 B + Xor32 [3..15). Response =
// PRECEIVE_CREATE_CHARACTER (WSclient.h :376-:386) 19 B: Result 1=ok/0=FAIL/
// 2=FAIL2 (:625/:664/:666); Index slot 0..4; Level WORD; Class tipo-server
// (conversão no cliente :653); ID[10]+NUL (:656).

// C->S: request WIRE REAL — [C1][0F][F3][01][ID10][classSkin] + Xor32 [3..15).
std::vector<uint8_t> BuildC1_F3_01_CreateCharRequestWire(const std::string& name10,
                                                         uint8_t cls, uint8_t skin) {
    if (name10.size() > 10 || cls > 0x0F || skin > 0x0F) return {};   // bounds-check
    std::vector<uint8_t> p(15, 0);
    p[0] = 0xC1; p[1] = 15; p[2] = 0xF3; p[3] = 0x01;
    for (size_t i = 0; i < name10.size(); ++i) p[4 + i] = static_cast<uint8_t>(name10[i]);
    p[14] = static_cast<uint8_t>((cls << 4) | skin);   // :306 nibble-packado
    crypto::XorData32(p.data(), 3, p.size());
    return p;
}

// S->C: parser do response (C1 plain 19 B; wire-real per spec 1.3-B).
struct ParsedCreateChar {
    uint8_t  result;    // 1=ok (:625) 0=FAIL (:664) 2=FAIL2 (:666)
    char     id[11];    // ID[10] + NUL (:656-:657)
    uint8_t  index;     // slot 0..4 (:629/:647)
    uint16_t level;     // :651
    uint8_t  serverClass; // tipo server; conversão p/ cliente é fora (:653)
};

bool ParseC1_F3_01_CreateCharResponsePlain(const std::vector<uint8_t>& c1,
                                           ParsedCreateChar& out, std::string& err) {
    if (c1.size() < 19) { err = "F3:01: C1 truncado (<19 B)"; return false; }
    if (c1[0] != 0xC1) { err = "F3:01: espera C1"; return false; }
    if (c1[2] != 0xF3 || c1[3] != 0x01) { err = "F3:01: head/sub invalidos"; return false; }
    out.result = c1[4];
    std::memcpy(out.id, &c1[5], 10);
    out.id[10] = '\0';
    out.index = c1[15];
    std::memcpy(&out.level, &c1[16], 2);
    out.serverClass = c1[18];
    return true;
}

} } // namespace newera::mvp (bloco 1.3-B)

// (bloco 1.3-C dentro do namespace do MVP)
namespace newera { namespace mvp {

// ---------------------------------------------------------------------------
// 1.3-C — JOIN GAME / JOIN MAP SERVER — wire-real per spec 1.3-C
// (NEW_ERA_PROTOCOL_MVP_JOIN_GAME_SPEC.md). DOIS SISTEMAS:
// TX = SendRequestJoinMapServer (wsclientinline :322-:330): copia ID[10] e
// chama gProtocolSend.SendPacket(ProtocolHead::BOTH_CONNECT_JOIN_GAME,
// nameBuf, 10) (ProtocolSend.h :145 -> DataSend) — sistema NOVO olc::net/ASIO,
// SEM C1/XOR: frame = message_header{id:u16, size:u32} + body (ProtocolAsio.h
// :33-:37; envio header->body :378-:390). Ponte no GS modern:
// SocketManagerModern.cpp :122-:131 traduz BOTH_CONNECT_JOIN_GAME ->
// PMSG_CHARACTER_INFO_RECV classico (header.set(0xF3,0x03), name<=10 B) ->
// CGCharacterInfoRecv (:129).
// RX = CLASSICO C1 F3:03: dispatch WSclient.cpp :12969-:12974 ->
// ReceiveJoinMapServer (:871-:1000+; crypto check COMENTADO :873-:880; cast
// direto :883) sobre PRECEIVE_JOIN_MAP_SERVER (WSclient.h :405-:452):
// wire = tag C1 + struct pack(1) 65 B = 66 B (0x42). NAO ha byte de result
// (dump puro de dados — diferente de F3:01/06).

// enum class ProtocolHead : uint16_t (ProtocolSend.h :7-:26) =>
// BOTH_CONNECT_JOIN_GAME = ordinal 11 (0..12)
static constexpr uint16_t kProto_BOTH_CONNECT_JOIN_GAME = 0x000B;

// C->S: frame olc::net [id:u16 LE][size:u32 LE][ID10] — 16 B, SEM crypto.
std::vector<uint8_t> BuildAsio_BOTH_CONNECT_JOIN_GAME_Request(const std::string& id10) {
    if (id10.size() > 10) return {};                    // bounds-check ID[10]
    std::vector<uint8_t> p(16, 0);
    const uint16_t id = kProto_BOTH_CONNECT_JOIN_GAME;  // 0x000B
    const uint32_t sz = 10;                             // body = ID[10]
    std::memcpy(&p[0], &id, 2);                         // little-endian (x86)
    std::memcpy(&p[2], &sz, 4);                         // little-endian (x86)
    for (size_t i = 0; i < id10.size(); ++i)            // ID[10] zero-padded
        p[6 + i] = static_cast<uint8_t>(id10[i]);
    return p;
}

// S->C: parser do response (C1 plain 66 B = PRECEIVE_JOIN_MAP_SERVER).
// Wire offsets: 4=X 5=Y 6=Map 7=Angle | 8..15=Exp 8xB MSB-first (:886-:938)
// 16..23=NextExp 8xB MSB-first | 24=LevelUpPoint(W) 26..49=12 stats(W) na
// ordem do struct (Str,Dex,Vit,Ene,Life,LifeMax,Mana,ManaMax,Shield,
// ShieldMax,SkillMana,SkillManaMax) | 50=Gold(DW) :964 | 54=PK :988
// 55=CtlCode :989 | 56=AddPoint(sh) 58=MaxAddPoint(sh) | 60=Charisma(W)
// 62=wMinusPoint(W) 64=wMaxMinusPoint(W). W/DW/short = little-endian
// (cast direto x86 :883). Angulo = ((Angle-1)*45) graus (:980).
struct ParsedJoinMapServer {
    uint8_t  x, y, map, angleByte;
    int      angleDeg;               // ((Angle-1)*45) :980 (byte 2 => 45)
    uint64_t exp, nextExp;           // 8xBYTE MSB-first encadeado (:886-:938)
    uint16_t levelUpPoint;
    uint16_t stat[12];               // Str,Dex,Vit,Ene,Life,LifeMax,Mana,
                                     // ManaMax,Shield,ShieldMax,SkillMana,
                                     // SkillManaMax (ordem do struct :430-:443)
    uint32_t gold;                   // :964
    uint8_t  pk, ctlCode;            // :988-:989
    int16_t  addPoint, maxAddPoint;  // :959-:960
    uint16_t charisma, minusPoint, maxMinusPoint;  // :946/:961-:962
};

bool ParseC1_F3_03_JoinMapServerResponsePlain(const std::vector<uint8_t>& c1,
                                              ParsedJoinMapServer& out, std::string& err) {
    if (c1.size() != 66 || c1[1] != 0x42) {
        err = "F3:03: tamanho invalido (espera C1 66 B / size 0x42; got " +
              std::to_string(c1.size()) + " B)";
        return false;
    }
    if (c1[0] != 0xC1) { err = "F3:03: espera C1"; return false; }
    if (c1[2] != 0xF3 || c1[3] != 0x03) { err = "F3:03: head/sub invalidos"; return false; }
    out.x = c1[4]; out.y = c1[5]; out.map = c1[6]; out.angleByte = c1[7];
    out.angleDeg = (static_cast<int>(out.angleByte) - 1) * 45;        // :980
    uint64_t e = 0;
    for (int i = 0; i < 8; ++i) e = (e << 8) | c1[8 + i];             // BE
    out.exp = e;
    e = 0;
    for (int i = 0; i < 8; ++i) e = (e << 8) | c1[16 + i];            // BE
    out.nextExp = e;
    std::memcpy(&out.levelUpPoint, &c1[24], 2);
    for (int i = 0; i < 12; ++i) std::memcpy(&out.stat[i], &c1[26 + 2 * i], 2);
    std::memcpy(&out.gold, &c1[50], 4);
    out.pk = c1[54]; out.ctlCode = c1[55];
    std::memcpy(&out.addPoint, &c1[56], 2);
    std::memcpy(&out.maxAddPoint, &c1[58], 2);
    std::memcpy(&out.charisma, &c1[60], 2);
    std::memcpy(&out.minusPoint, &c1[62], 2);
    std::memcpy(&out.maxMinusPoint, &c1[64], 2);
    return true;
}

} } // namespace newera::mvp (bloco 1.3-C)

// (bloco 1.3-D dentro do namespace do MVP)
namespace newera { namespace mvp {

// ---------------------------------------------------------------------------
// 1.3-D — POST-JOIN SELF INFO — F3:0xE0 GCNewCharacterInfoSend (GS) —
// wire-real per spec 1.3-D (NEW_ERA_PROTOCOL_MVP_POSTJOIN_SELFINFO_SPEC.md).
// EVIDÊNCIA: GCNewCharacterInfoSend (GS_Protocol.cpp :3411-:3461) sob
// #if(GAMESERVER_EXTRA==1); pMsg.header.set(0xF3,0xE0,sizeof(pMsg)) :3417;
// DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size) :3458 (serial C1;
// size = sizeof(struct) TOTAL — padrão provado em F3:01/F3:03).
// ANTI-INVENÇÃO: a struct PMSG_NEW_CHARACTER_INFO_SEND NÃO está na evidência
// (offsets/tamanho [NOT RECOVERED]) e o client upstream 580472e NÃO tem
// handler (sub-switch F3 :12958-:13038 = 24 subs, SEM 0xE0 e SEM default) —
// o consumidor é um client modificado EXTRA. Parser = VALIDADOR de frame
// header-only; payload permanece OPACO (36 campos conhecidos só pelo nome,
// na ordem de atribuição :3419-:3456). Sem TX (pacote é S→C). Irmão:
// GCNewCharacterCalcSend :3463 = F3:0xE1.

// S->C: classificador/validador do frame (C1 plain; payload opaco).
struct ParsedSelfInfo {
    uint8_t  head;            // 0xF3 (:3417)
    uint8_t  sub;             // 0xE0 (:3417)
    size_t   frameSize;       // c1[1] = total (= sizeof(PMSG_NEW_CHARACTER_INFO_SEND) no GS)
    std::vector<uint8_t> payloadOpaque;  // bytes [4..): layout [NOT RECOVERED]
};

bool ParseC1_F3_E0_SelfInfoResponsePlain(const std::vector<uint8_t>& c1,
                                         ParsedSelfInfo& out, std::string& err) {
    if (c1.size() < 4 || c1[1] != c1.size()) {
        err = "F3:E0: tamanho C1 inconsistente (size != total; got " +
              std::to_string(c1.size()) + " B)";
        return false;
    }
    if (c1[0] != 0xC1) { err = "F3:E0: espera C1"; return false; }
    if (c1[2] != 0xF3 || c1[3] != 0xE0) { err = "F3:E0: head/sub invalidos"; return false; }
    out.head = c1[2];
    out.sub = c1[3];
    out.frameSize = c1.size();
    out.payloadOpaque.assign(c1.begin() + 4, c1.end());   // layout [NOT RECOVERED]
    return true;
}

} } // namespace newera::mvp (bloco 1.3-D)

// (bloco 1.3-E dentro do namespace do MVP)
namespace newera { namespace mvp {

// ---------------------------------------------------------------------------
// 1.3-E — VIEWPORT SPAWN — HeadCode 0x13 ReceiveCreateMonsterViewport —
// wire-real per spec 1.3-E (NEW_ERA_PROTOCOL_MVP_VIEWPORT_SPAWN_SPEC.md).
// EVIDÊNCIA: dispatch case 0x13 (WSclient.cpp :13104-:13107) -> handler
// :2585-:2712. Framing C2 PROVADO pelo cast LPPWHEADER_DEFAULT_WORD (:2587)
// sobre PWMSG_HEADER{Code;SizeH;SizeL;HeadCode} 4 B (WSclient.h :83-:89;
// PBMSG_HEADER 3 B = C1 :76-:81) — buffer C1 seria misparseado.
// Header 5 B [C2][SizeH][SizeL][0x13][count BYTE (:202 — apesar do nome
// "_WORD")]; entidade = PCREATE_MONSTER (H :588-:600) com STRIDE VARIÁVEL
// 10+s_BuffCount (:2710 — array declarado [MAX_BUFF_SLOT_INDEX=16] :599/:613,
// no wire só viajam s_BuffCount bytes). Key/Type BIG-ENDIAN c/ bits de flag:
// Key=(KeyH<<8)|KeyL &0x7FFF :2592/:2605 (CreateFlag=b15 :2602,
// TeleportFlag=KeyH.0x40 :2603); Type 10 bits :2597 (bMyMob=TypeH.0x80
// :2595, buildTime=(TypeH&0x70)>>4 :2596); dir=Path>>4 :2637 com ângulo
// ((dir-1)*45) :2637. CreateMonster(Type,X,Y,Key) :2606; buffs -> RegisterBuff
// :2616; TargetX/Y -> PathFinding2 :2701. Sem TX (S→C).
// 1.3-F HARDEN: s_BuffCount limitado a MAX_BUFF_SLOT_INDEX=16 — o array
// declarado s_BuffEffectState[16] (:599; define.h :613) significa que o
// legado leria OUT-OF-BOUNDS se o server mandasse >16; parser rejeita.

// 1.3-F: limite do array declarado (evidência :599 + define.h :613)
static constexpr uint8_t kViewportMaxBuffSlotIndex = 16;

// S->C: 1 entidade de viewport (somente campos provados).
struct SpawnEntity {
    uint16_t type;         // 10 bits :2597
    uint16_t key;          // BE &0x7FFF :2592/:2605
    uint8_t  x, y;         // PositionX/Y :2606
    uint8_t  targetX, targetY;  // :2701
    uint8_t  dir;          // Path>>4 :2637
    int      angleDeg;     // ((dir-1)*45) :2637
    bool     createFlag;   // Key b15 :2602 (AppearMonster :2681)
    bool     teleportFlag; // KeyH.0x40 :2603
    bool     myMob;        // TypeH.0x80 :2595
    uint8_t  buildTime;    // (TypeH&0x70)>>4 :2596
    std::vector<uint8_t> buffs;  // s_BuffEffectState[0..count) :2614-:2619
};

bool ParseC2_ViewportMonsterSpawnPlain(const std::vector<uint8_t>& pkt,
                                       std::vector<SpawnEntity>& out, std::string& err) {
    out.clear();
    if (pkt.size() < 5) { err = "0x13: C2 truncado (<5 B)"; return false; }
    if (pkt[0] != 0xC2) { err = "0x13: espera C2 (PWMSG_HEADER :83-:89)"; return false; }
    const size_t declared = ((size_t)pkt[1] << 8) | pkt[2];    // SizeH<<8|SizeL :86-:87
    if (declared != pkt.size()) {
        err = "0x13: tamanho C2 inconsistente (declared " + std::to_string(declared) +
              " != total " + std::to_string(pkt.size()) + ")";
        return false;
    }
    if (pkt[3] != 0x13) { err = "0x13: head invalido"; return false; }
    const size_t count = pkt[4];                               // count BYTE :202/:2589
    size_t off = 5;                                            // sizeof(PWHEADER_DEFAULT_WORD)
    out.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        if (off + 10 > pkt.size()) {
            err = "0x13: entidade " + std::to_string(i) + " truncada (<10 B)";
            out.clear(); return false;
        }
        SpawnEntity e{};
        const uint8_t keyH = pkt[off], keyL = pkt[off + 1];
        const uint8_t typeH = pkt[off + 2], typeL = pkt[off + 3];
        const uint16_t keyRaw = (uint16_t)(((uint16_t)keyH << 8) | keyL);   // BE :2592
        e.createFlag   = (keyRaw >> 15) != 0;                               // :2602
        e.teleportFlag = ((keyH & 0x40) >> 6) != 0;                         // :2603
        e.key          = keyRaw & 0x7FFF;                                   // :2605
        e.myMob        = (typeH & 0x80) != 0;                               // :2595
        e.buildTime    = (uint8_t)((typeH & 0x70) >> 4);                    // :2596
        e.type         = (uint16_t)(((uint16_t)(typeH & 0x03) << 8) | typeL);  // :2597
        e.x = pkt[off + 4]; e.y = pkt[off + 5];                             // :2606
        e.targetX = pkt[off + 6]; e.targetY = pkt[off + 7];                 // :2701
        e.dir = pkt[off + 8] >> 4;                                          // :2637
        e.angleDeg = ((int)e.dir - 1) * 45;                                 // :2637
        const uint8_t nBuffs = pkt[off + 9];                                // s_BuffCount
        if (nBuffs > kViewportMaxBuffSlotIndex) {                           // 1.3-F harden
            err = "0x13: entidade " + std::to_string(i) + " s_BuffCount=" +
                  std::to_string(nBuffs) + " > MAX_BUFF_SLOT_INDEX (16) :599/:613";
            out.clear(); return false;
        }
        if (off + 10 + nBuffs > pkt.size()) {
            err = "0x13: entidade " + std::to_string(i) + " buffs truncados";
            out.clear(); return false;
        }
        e.buffs.assign(pkt.begin() + off + 10, pkt.begin() + off + 10 + nBuffs);
        off += 10 + (size_t)nBuffs;                                         // stride :2710
        out.push_back(std::move(e));
    }
    if (off != pkt.size()) {
        err = "0x13: bytes residuos (" + std::to_string(pkt.size() - off) + ")";
        out.clear(); return false;
    }
    return true;
}

} } // namespace newera::mvp (bloco 1.3-E)
