#include "tables.hpp"

using matrix = std::array<std::array<uint8_t, 4>, 4>;

void printWord(const std::array<uint8_t, 4> & arr)
{
    std::cout << "Arr: ";
    for (auto & el : arr)
        std::cout << std::hex << uint(el) << ' ';
    std::cout << std::endl;
}

std::string vecToStr(const std::vector<uint8_t> & v)
{
    std::stringstream str;
    for (auto & el : v)
        str << std::hex << uint(el) << ' ';
    return str.str();
}

std::string matrToStr(const matrix & m)
{
    std::stringstream str;
    for (auto & row : m)
    {
        for (auto & el : row)
            str << std::hex << uint(el) << ' ';
        str << std::endl;
    }
    return str.str();
}

//------------------------------------------------------------------------------

uint8_t subBytes(uint8_t byte)
{
    return s_box[byte >> 4][byte & 0xF];
}

//
// TODO: Над этим надо подумать.
//
std::array<uint8_t, 4> shiftLeft(const std::array<uint8_t, 4> & arr, int n)
{
    std::array<uint8_t, 4> res;
    int idx;
    for (int i = 0; i < arr.size(); i++)
    {
        idx = (i - n) >= 0 ? i - n : arr.size() + i - n;
        res[idx] = arr[i];
    }

    return res;
}

void shiftRows(matrix & m)
{
    m[1] = shiftLeft(m[1], 1);
    m[2] = shiftLeft(m[2], 2);
    m[3] = shiftLeft(m[3], 3);
}


//
// c(x) = / 02 03 01 01 \
//        | 01 02 03 01 |
//        | 01 01 02 03 |
//        \ 03 01 01 02 /
//
void mixColumns(matrix & m)
{
    std::array<uint8_t, 4> res;

    for (int i = 0; i < m.size(); i++)
    {
        res[0] = c_x[0][0] * m[0][i] + c_x[0][1] * m[1][i] +
                 c_x[0][2] * m[2][i] + c_x[0][3] * m[3][i];
        res[1] = c_x[1][0] * m[0][i] + c_x[1][1] * m[1][i] +
                 c_x[1][2] * m[2][i] + c_x[1][3] * m[3][i];
        res[2] = c_x[2][0] * m[0][i] + c_x[2][1] * m[1][i] +
                 c_x[2][2] * m[2][i] + c_x[2][3] * m[3][i];
        res[3] = c_x[3][0] * m[0][i] + c_x[3][1] * m[1][i] +
                 c_x[3][2] * m[2][i] + c_x[3][3] * m[3][i];

        m[0][i] = res[0];
        m[1][i] = res[1];
        m[2][i] = res[2];
        m[3][i] = res[3];
    }
}

std::array<uint8_t, 4> sumWords(const std::array<uint8_t, 4> & w1,
                                const std::array<uint8_t, 4> & w2)
{
    return { w1[0]^w2[0], w1[1]^w2[1], w1[2]^w2[2], w1[3]^w2[3] };
}

std::array<uint8_t, 4> gFun(const std::array<uint8_t, 4> & w, uint8_t & rc)
{
    auto ww = shiftLeft(w, 1); // Циклический сдвиг на 1 байт влево

    // Замена байт, полученных на прошлом шаге, в соответствии с таблицей s-box
    for (auto & el : ww)
        el = subBytes(el);

    ww[0] ^= rc; // суммирование с раундовой постоянной

    rc *= 2;

    return ww;
}

void keyExpansion()
{
    int r = 1;
    uint8_t rc = 1;
    for (int i = 4; i < ws.size(); i++)
    {
        if (i % 4 == 0) ws[i] = sumWords(ws[i-4], gFun(ws[i-1], rc));
        else ws[i] = sumWords(ws[i-1], ws[i-4]);
    }
}

void round(matrix & state, const matrix & key, int rn)
{
    if (rn == 0)
    {

    }

    if (rn == 10)
    {

    }

    // subBytes
    // shiftRows
    // mixColumns
    // addRoundKey
}

int main(int, char**)
{
    std::vector<uint8_t> key = { 0x0f, 0x15, 0x71, 0xc9, 0x47, 0xd9, 0xe8, 0x59,
                                 0x0c, 0xb7, 0xad, 0xdf, 0xaf, 0x7f, 0x67, 0x98
                               };

    ws[0] = { key[0],  key[1],  key[2],  key[3]  };
    ws[1] = { key[4],  key[5],  key[6],  key[7]  };
    ws[2] = { key[8],  key[9],  key[10], key[11] };
    ws[3] = { key[12], key[13], key[14], key[15] };

    keyExpansion();

    int i = 0;
    for (auto & w : ws)
    {
        std::cout << "W" << std::dec << i << ": ";
        for (auto & el : w)
            std::cout << std::hex << uint(el) << ' ';
        std::cout << std::endl;
        i++;
    }

    auto ta = sumWords({0x04, 0x05, 0x06, 0x07}, {0x4, 0x5, 0x6, 0x7});
    std::cout << "ta: ";
    for (auto & el : ta)
        std::cout << std::hex << uint(el) << ' ';
    std::cout << std::endl;

    // 10 раундов для ключа 128-бит
    // 12 раундов для ключа 192-бит
    // 14 раундов для ключа 256-бит
    const int rounds = 10;

    std::vector<uint8_t> input = { 7, 8, 9, 2, 3, 4, 7, 5,
                                   1, 4, 6, 2, 5, 1, 6, 7
                                 };

    matrix m_input;

    int col = 0;
    int row = 0;
    for (int i = 0; i < input.size(); i++)
    {
        m_input[row][col] = input[i];
        row++;
        if (row == m_input.size())
        {
            row = 0;
            col++;
        }
    }

    std::cout << matrToStr(m_input);
    mixColumns(m_input);
    std::cout << std::endl << matrToStr(m_input);

    std::cout << "Key: " << vecToStr(key) << std::endl;
    std::cout << std::hex << uint(subBytes(0x0f)) << std::endl;

    std::array<uint8_t, 4> test = {1, 2, 3, 4};
    auto res = shiftLeft(test, 1);
    for (auto & el : res)
        std::cout << int(el) << ' ';
    std::cout << std::endl;
}
