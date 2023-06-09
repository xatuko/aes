#ifndef AES_HPP
#define AES_HPP

#include "tables.hpp"

using blck      = std::array<uint8_t, 16>;
using word      = std::array<uint8_t, 4>;
using matrix    = std::array<word, 4>;
using bytearray = std::vector<uint8_t>;

enum class MODE : uint8_t
{
    ECB = 0,
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

    // void setKey(const blck & key);
    bytearray encrypt(bytearray input, const MODE & mode, const blck & iv);
    bytearray decrypt(bytearray input, const MODE & mode, const blck & iv);

    void setKey(const blck & key)
    { m_key = key; keyExpansion(); m_have_key = true; }

  private:
  // Fields
    blck m_key;
    bool m_have_key { false };
    MODE m_mode { MODE::ECB };

  // Methods
    uint8_t dot(uint8_t v1, uint8_t v2);

    uint8_t subBytes(const uint8_t & byte)
    { return s_box[byte >> 4][byte & 0xF]; }

    uint8_t invSubBytes(const uint8_t & byte)
    { return is_box[byte >> 4][byte & 0xF]; }

    word shiftLeft(const word & arr, const int & n);
    word shiftRight(const word & arr, const int & n);
    word gFun(const word & w, const uint8_t & rc);
    word sumWords(const word & w1, const word & w2);
    matrix sumMatrix(const matrix & m1, const matrix & m2);

    matrix shiftRows(const matrix & m);
    matrix invShiftRows(const matrix & m);
    matrix mixColumns(const matrix & m);
    matrix invMixColumns(const matrix & m);

    blck encryptBlck(const blck & input);
    blck decryptBlck(const blck & input);

    matrix encryptRound(const matrix & state, const matrix & key, const int & rn);
    matrix decryptRound(const matrix & state, const matrix & key, const int & rn);

    void keyExpansion();
};

#endif // AES_HPP