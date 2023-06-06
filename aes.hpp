#ifndef AES_HPP
#define AES_HPP

#include "tables.hpp"

using blck = std::array<uint8_t, 16>;
using word = std::array<uint8_t, 4>;
using matrix = std::array<word, 4>;

enum class MODE : uint8_t
{
    ECB = 0,
    CBC, // not supproted yet
    CFB, // not supproted yet
    OFB, // not supproted yet
    CTR  // not supproted yet
};

class Aes
{
  public:
    Aes() { };
    ~Aes() { };

    std::array<uint8_t,16> encryptBlck(const blck & input, const blck & key);

  private:
  // Fields
    blck m_key;
    MODE m_mode { MODE::ECB };

  // Methods
    uint8_t dot(uint8_t v1, uint8_t v2);

    uint8_t subBytes(const uint8_t & byte)
    { return s_box[byte >> 4][byte & 0xF]; }

    word shiftLeft(const word & arr, const int & n);
    word gFun(const word & w, const uint8_t & rc);
    word sumWords(const word & w1, const word & w2);
    matrix shiftRows(const matrix & m);
    matrix mixColumns(const matrix & m);
    matrix round(const matrix & state, const matrix & key, const int & rn);
    void keyExpansion();
};

#endif // AES_HPP