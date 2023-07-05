#ifndef AES_HPP
#define AES_HPP

#include <iostream>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>

namespace crypto = CryptoPP;

using blck      = std::array<uint8_t, 16>;
using bytearray = std::vector<uint8_t>;

enum class MODE : uint8_t
{
    ECB = 0, // not supported
    CBC,
    CFB, // not supproted
    OFB, // not supproted
    CTR  // not supproted
};

class Aes
{
  public:
    Aes() { };
    ~Aes() { };

    bytearray encrypt(const bytearray & input, const MODE & mode);
    bytearray decrypt(const bytearray & input, const MODE & mode);

    void setKey(const blck & key);
    void setIv(const blck & iv);

  private:
  // Fields

    crypto::SecByteBlock m_key { crypto::AES::DEFAULT_KEYLENGTH };
    crypto::SecByteBlock m_iv  { crypto::AES::BLOCKSIZE };

    MODE m_mode { MODE::ECB };
  // Methods
    bytearray encryptCBC(const bytearray & input);
    bytearray decryptCBC(const bytearray & input);
};

#endif // AES_HPP