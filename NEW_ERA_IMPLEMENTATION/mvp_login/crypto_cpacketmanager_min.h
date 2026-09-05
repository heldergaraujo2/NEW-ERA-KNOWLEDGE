// NEW-ERA — MVP LOGIN — crypto_cpacketmanager_min.h
// Implementação MÍNIMA fiel do cipher clássico 8B->11B (SimpleModulus / CPacketManager).
// EVIDÊNCIA primária (read-only): NEW_ERA_EVIDENCE/0C10A__PacketManager…cpp @580472e
//   (sha 3e82e5d21ecfb84367a4139293aedb0b3146a2f7e221ffe03d45c2499006cfe4):
//   Encrypt      :214 (caminho clássico #else :236-:263: blocos de 8 -> 11 bytes)
//   EncryptBlock :319 (:327-:364: 4x WORD; modulo; chain; AddBits 16+2; checksum)
//   AddBits      :430 (:438-:465: bitstream MSB-first, OR no destino)
//   Tipos: ENCDEC_DATA{DWORD Modulus[4];DWORD Key[4];DWORD Xor[4]} (PacketManager.h:25-30);
//   WORD = 16 bits little-endian (x86/MSVC — evidência assume LE em ((WORD*)src)[n]).
// NÃO implementado aqui (TODO 1.0-D+): caminho GAMESERVER_UPDATE>=701 (DES ProcessData :216-:233);
//   Decrypt (:268 clássico :288-:314 via DecryptBlock :369) — fica para o passo RX.
// Chaves NÃO são carregadas de arquivo: injeção via SetKeys (test-vector ready).
#pragma once
#include <stdint.h>
#include <stddef.h>
#include <cstring>
#include <string>
#include "crypto_sm_keyfile_loader.h"

namespace newera {
namespace crypto {

class PacketCryptoSM {
public:
    struct Keys {                   // = ENCDEC_DATA (PacketManager.h:25-30)
        uint32_t modulus[4];
        uint32_t key[4];
        uint32_t xor_[4];
    };

    void SetKeys(const Keys& k) { keys_ = k; loaded_ = true; }

    // 1.0-D: carrega chaves do arquivo ENCDEC (formato evidenciado em LoadKey :147-:212).
    // type: 0 = encryption (Enc1.dat), 1 = decryption (Dec2.dat) — LoadKey :176-:183.
    bool LoadKeysFromFile(const std::string& path, std::string* err = nullptr, int type = 0) {
        if (type != 0 && type != 1) { if (err) *err = "type invalido (0=enc,1=dec)"; return false; }
        EncDecKeys k{};
        if (!LoadEncDecKeysFromFile(path, k, err)) return false;
        Keys kk;
        std::memcpy(kk.modulus, k.modulus, sizeof(kk.modulus));
        std::memcpy(kk.key, k.key, sizeof(kk.key));
        std::memcpy(kk.xor_, k.xor_, sizeof(kk.xor_));
        SetKeys(kk);
        return true;
    }

    bool KeysLoaded() const { return loaded_; }

    // 1.0-F: acesso às chaves carregadas p/ derivar o par inverso (§48) nos test vectors
    const Keys& DebugKeys() const { return keys_; }

    // Espelha CPacketManager::Encrypt caminho clássico (:236-:263).
    // Retorna tamanho do ciphertext (size -> blocos de 8 em 11) ou <0 em erro.
    int Encrypt(uint8_t* target, const uint8_t* source, int size) {
#ifndef NEWERA_ALLOW_DUMMY_KEYS   // 1.0-D: chaves reais OBRIGATÓRIAS (default OFF p/ dummy)
        if (!loaded_) return -2;  // erro claro: carregar Enc1.dat via LoadKeysFromFile/TryLoadLoginKeys
#endif
        if (!target || !source || size <= 0) return -1;
        int oriSize = size;
        int dec = (size + 7) / 8;                       // :242
        int outSize = ((dec + (dec * 4)) * 2) + dec;    // :244 (8B -> 11B por bloco)
        uint8_t* t = target;
        int remaining = oriSize;
        for (int n = 0; n < oriSize; n += 8, remaining -= 8, t += 11) {  // :250
            int blockSize = (remaining >= 8) ? 8 : remaining;            // :252-:257
            // NOTA de fidelidade: EncryptBlock lê 8 bytes do fonte incondicionalmente
            // (:356-:359 checksum). O último bloco parcial exige fonte com padding
            // (zero-pad aqui) — HIPÓTESE marcada (TODO 1.0-D: confirmar padding do caller legado).
            uint8_t blk[8] = { 0 };
            std::memcpy(blk, source + n, static_cast<size_t>(blockSize));
            EncryptBlock(t, blk, blockSize);
        }
        return outSize;
    }

    // Espelha EncryptBlock (:327-:364). size = bytes válidos do bloco (<=8).
    int EncryptBlock(uint8_t* target, const uint8_t* source, int size) {
        uint32_t encBuffer[4] = { 0 };
        uint32_t encValue = 0;
        std::memset(target, 0, 11);                                     // :333

        for (int n = 0; n < 4; ++n) {                                   // :335-:339
            uint16_t w = 0;
            std::memcpy(&w, source + n * 2, sizeof(w));                 // WORD LE
            encBuffer[n] = (((keys_.xor_[n] ^ w) ^ encValue) * keys_.key[n]) % keys_.modulus[n]; // :337
            encValue = static_cast<uint16_t>(encBuffer[n]);             // :338
        }
        for (int n = 0; n < 3; ++n) {                                   // :341-:344
            encBuffer[n] = (encBuffer[n] ^ keys_.xor_[n]) ^ static_cast<uint16_t>(encBuffer[n + 1]);
        }
        int bitPos = 0;                                                 // :346-:352
        for (int n = 0; n < 4; ++n) {
            bitPos = AddBits(target, bitPos, reinterpret_cast<uint8_t*>(&encBuffer[n]), 0, 16);
            bitPos = AddBits(target, bitPos, reinterpret_cast<uint8_t*>(&encBuffer[n]), 22, 2);
        }
        uint8_t checkSum = 0xF8;                                        // :354-:359
        for (int n = 0; n < 8; ++n) checkSum ^= source[n];
        uint16_t tail = 0;                                              // :361-:362
        uint8_t* tb = reinterpret_cast<uint8_t*>(&tail);
        tb[0] = static_cast<uint8_t>((checkSum ^ size) ^ 0x3D);
        tb[1] = checkSum;
        return AddBits(target, bitPos, reinterpret_cast<uint8_t*>(&tail), 0, 16); // :364
    }

    // Reescrita EQUIVALENTE de AddBits (:430-:470): copia `size` bits do source
    // (a partir do bit SourceBitPos) para o target (a partir de TargetBitPos),
    // bitstream MSB-first (bit 0 = MSB do byte 0), combinando por OR (:462-:465).
    static int AddBits(uint8_t* target, int targetBitPos, const uint8_t* source, int sourceBitPos, int size) {
        for (int i = 0; i < size; ++i) {
            int sb = sourceBitPos + i, tb = targetBitPos + i;
            uint8_t bit = (source[sb >> 3] >> (7 - (sb & 7))) & 1u;     // MSB-first
            if (bit) target[tb >> 3] |= static_cast<uint8_t>(1u << (7 - (tb & 7)));
        }
        return targetBitPos + size;
    }

    // ----- RX (1.0-E): fiel a Decrypt (:288-:314) + DecryptBlock (:377-:425) -----
    // Espelha CPacketManager::Decrypt clássico: blocos de 11 -> 8 bytes.
    // QUIRK de fidelidade: o loop original checa `result<0` (acumulado), NÃO
    // TempResult (:302) — um bloco com checksum ruim só é detectado na
    // iteração seguinte (ou no retorno final). Mantido como na evidência.
    int Decrypt(uint8_t* target, const uint8_t* source, int size) {
        if (!target || !source || size <= 0) return -1;
        int result = (size * 8) / 11;                    // :288
        int decSize = 0;
        result = 0;                                      // :296
        const uint8_t* s = source; uint8_t* t = target;
        while (decSize < size) {                         // :298
            int tempResult = DecryptBlock(t, s);         // :300
            if (result < 0) return result;               // :302 (quirk preservado)
            result += tempResult;                        // :307
            decSize += 11; s += 11; t += 8;              // :308-:310
        }
        return result;                                   // :314
    }

    // Espelha DecryptBlock (:377-:425): retorna bytes válidos do bloco (0..8) ou -1 (checksum).
    int DecryptBlock(uint8_t* out8, const uint8_t* in11) {
        uint32_t decBuffer[4] = { 0 };
        std::memset(out8, 0, 8);                         // :382
        int bitPos = 0;                                  // :384
        for (int n = 0; n < 4; ++n) {                    // :386-:392 — BITPO AVANÇA EXPLICITAMENTE
            AddBits(reinterpret_cast<uint8_t*>(&decBuffer[n]), 0, in11, bitPos, 16);   // :388
            bitPos += 16;                                // :389 (retorno de AddBits = targetPos+size — NÃO usar aqui)
            AddBits(reinterpret_cast<uint8_t*>(&decBuffer[n]), 22, in11, bitPos, 2);  // :390
            bitPos += 2;                                 // :391
        }
        for (int n = 2; n >= 0; --n) {                   // :394-:397 (unchain)
            decBuffer[n] = (decBuffer[n] ^ keys_.xor_[n]) ^ static_cast<uint16_t>(decBuffer[n + 1]);
        }
        uint32_t value = 0;                              // :399-:405
        for (int n = 0; n < 4; ++n) {
            uint16_t w = static_cast<uint16_t>((((keys_.key[n] * decBuffer[n]) % keys_.modulus[n]) ^ keys_.xor_[n]) ^ value);
            std::memcpy(out8 + n * 2, &w, sizeof(w));    // ((WORD*)target)[n] = ...
            value = static_cast<uint16_t>(decBuffer[n]);
        }
        decBuffer[0] = 0;                                // :407-:409 (tail 16 bits)
        AddBits(reinterpret_cast<uint8_t*>(&decBuffer[0]), 0, in11, bitPos, 16);
        uint8_t* db = reinterpret_cast<uint8_t*>(decBuffer);
        db[0] = static_cast<uint8_t>((db[0] ^ db[1]) ^ 0x3D);   // :411 (size do bloco)
        uint8_t checkSum = 0xF8;                                // :413-:418
        for (int n = 0; n < 8; ++n) checkSum ^= out8[n];
        if (checkSum != db[1]) return -1;                       // :420-:423
        return db[0];                                           // :425
    }

private:
    Keys keys_{};
    bool loaded_ = false;   // 1.0-D: false até SetKeys/LoadKeysFromFile
};

} // namespace crypto
} // namespace newera
