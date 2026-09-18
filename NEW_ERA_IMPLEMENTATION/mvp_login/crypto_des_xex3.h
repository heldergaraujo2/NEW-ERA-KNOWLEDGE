#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

#if __has_include(<openssl/evp.h>)
#include <openssl/evp.h>
#define NEWERA_HAS_OPENSSL_EVP 1
#else
#define NEWERA_HAS_OPENSSL_EVP 0
#endif

namespace newera::crypto {

// Crypto++ DES_XEX3-compatible single-block DESX primitive.
// 24-byte key layout is [pre-whitening 8][DES key 8][post-whitening 8].
// Crypto++ documents DES_XEX3 as a fixed 8-byte block / 24-byte key cipher.
// See: Crypto++ des.h / des.cpp.
class DesXex3 {
public:
    using Key = std::array<std::uint8_t,24>;
    using Block = std::array<std::uint8_t,8>;

    static bool EncryptBlock(const Key& key, const Block& in, Block& out, std::string* err=nullptr) {
#if NEWERA_HAS_OPENSSL_EVP
        return CryptBlock(key,in,out,true,err);
#else
        (void)key; (void)in; (void)out;
        if(err) *err="OpenSSL EVP is required for the portable DESX backend";
        return false;
#endif
    }

    static bool DecryptBlock(const Key& key, const Block& in, Block& out, std::string* err=nullptr) {
#if NEWERA_HAS_OPENSSL_EVP
        return CryptBlock(key,in,out,false,err);
#else
        (void)key; (void)in; (void)out;
        if(err) *err="OpenSSL EVP is required for the portable DESX backend";
        return false;
#endif
    }

private:
#if NEWERA_HAS_OPENSSL_EVP
    static bool CryptBlock(const Key& key, const Block& in, Block& out, bool encrypt, std::string* err) {
        std::array<unsigned char,8> x{};
        for(std::size_t i=0;i<8;++i) x[i]=static_cast<unsigned char>(in[i]^key[i]);

        EVP_CIPHER_CTX* ctx=EVP_CIPHER_CTX_new();
        if(!ctx){ if(err)*err="EVP_CIPHER_CTX_new failed"; return false; }

        // Triple-DES with K1=K2=K3 is exactly single DES, while EVP's
        // available 3DES interface remains usable on OpenSSL 3.
        std::array<unsigned char,24> des3{};
        for(std::size_t i=0;i<8;++i) {
            des3[i]=key[8+i]; des3[8+i]=key[8+i]; des3[16+i]=key[8+i];
        }

        const EVP_CIPHER* cipher=EVP_des_ede3_ecb();
        bool ok=EVP_CipherInit_ex(ctx,cipher,nullptr,des3.data(),nullptr,encrypt?1:0)==1;
        if(ok) EVP_CIPHER_CTX_set_padding(ctx,0);
        int n=0, total=0;
        unsigned char buf[8]{};
        if(ok) ok=EVP_CipherUpdate(ctx,buf,&n,x.data(),8)==1 && n==8;
        if(ok) {
            total=n;
            int final_n=0;
            ok=EVP_CipherFinal_ex(ctx,buf+total,&final_n)==1 && final_n==0;
        }
        if(ok) for(std::size_t i=0;i<8;++i) out[i]=static_cast<std::uint8_t>(buf[i]^key[16+i]);
        EVP_CIPHER_CTX_free(ctx);
        if(!ok && err) *err="DES-XEX3 EVP block operation failed";
        return ok;
    }
#endif
};

} // namespace newera::crypto
