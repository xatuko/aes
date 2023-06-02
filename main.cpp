#include "tables.hpp"

using matrix = std::array<std::array<uint8_t, 4>, 4>;

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
        std::cout << idx << std::endl;
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

void mixColumns(matrix & m)
{

}

void round(matrix & state, const matrix & key)
{

}

int main(int, char**)
{
    std::vector<uint8_t> key = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10
                               };

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

    std::cout << "Key: " << vecToStr(key) << std::endl;
    std::cout << std::hex << uint(subBytes(0x0f)) << std::endl;

    std::array<uint8_t, 4> test = {1, 2, 3, 4};
    auto res = shiftLeft(test, 1);
    for (auto & el : res)
        std::cout << int(el) << ' ';
    std::cout << std::endl;
}
