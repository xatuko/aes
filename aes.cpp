#include "aes.hpp"

uint8_t Aes::dot(uint8_t v1, uint8_t v2)
{
    // x^8 = x^4 + x^3 + x + 1

    // k1 + k2*x + k3*x^2 + k4*x^3 + k5*x^4 + k6*x^5 + k7*x^6 + k8*x^7
    std::array<uint16_t, 16> kv1 = { uint16_t(v1 & 0x01),        uint16_t((v1 & 0x02) >> 1),
                                     uint16_t((v1 & 0x04) >> 2), uint16_t((v1 & 0x08) >> 3),
                                     uint16_t((v1 & 0x10) >> 4), uint16_t((v1 & 0x20) >> 5),
                                     uint16_t((v1 & 0x40) >> 6), uint16_t((v1 & 0x80) >> 7),
                                     0, 0, 0, 0, 0, 0, 0, 0
                                   };

    // k1 + k2*x + k3*x^2 + k4*x^3 + k5*x^4 + k6*x^5 + k7*x^6 + k8*x^7
    std::array<uint16_t, 16> kv2 = { uint16_t(v2 & 0x01),        uint16_t((v2 & 0x02) >> 1),
                                     uint16_t((v2 & 0x04) >> 2), uint16_t((v2 & 0x08) >> 3),
                                     uint16_t((v2 & 0x10) >> 4), uint16_t((v2 & 0x20) >> 5),
                                     uint16_t((v2 & 0x40) >> 6), uint16_t((v2 & 0x80) >> 7),
                                    0, 0, 0, 0, 0, 0, 0, 0
                                   };

    //                                0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15
    std::array<uint16_t, 16> poly = { 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 };
    std::array<int16_t, 16> tmp;
    bool b = false;

    std::vector<std::array<int16_t, 16>> parts;
    for (int i = 0; i < kv1.size(); i++)
    {
        if (kv1[i] == 0)
            continue;

        tmp.fill(0);

        uint16_t res = 0;
        for (int j = 0; j < kv2.size(); j++)
            if (kv2[j] != 0)
            {
                tmp[i+j]++;
                b = true;
            }

        if (b) parts.push_back(tmp);
        b = false;
    }

    tmp.fill(0);
    for (auto & part : parts)
        for (int i = 0; i < part.size(); i++)
            tmp[i] += part[i];

    for (int i = 8; i < tmp.size(); i++)
    {
        if (tmp[i] == 0 || tmp[i] % 2 == 0) continue;

        tmp[4+(i-8)]++;
        tmp[3+(i-8)]++;
        tmp[1+(i-8)]++;
        tmp[(i-8)]++;
    }

    for (auto & el : tmp)
        el = (el % 2 == 0) ? 0 : 1;

    uint16_t res = 0;
    for (int i = 0; i < 8; i++)
        if (tmp[i] != 0)
            res += (1 << i);

    return res;
}

word Aes::shiftLeft(const word & w, const int & n)
{
    word res;
    int idx;
    for (int i = 0; i < w.size(); i++)
    {
        idx = (i - n) >= 0 ? i - n : w.size() + i - n;
        res[idx] = w[i];
    }
    return res;
}

word Aes::shiftRight(const word & w, const int & n)
{
    word res;
    int idx;
    for (int i = 0; i < w.size(); i++)
    {
        idx = (i + n) < w.size() ? i + n : i + n - w.size();
        res[idx] = w[i];
    }
    return res;
}

matrix Aes::shiftRows(const matrix & m)
{
    matrix res;
    res[0] = m[0];
    res[1] = shiftLeft(m[1], 1);
    res[2] = shiftLeft(m[2], 2);
    res[3] = shiftLeft(m[3], 3);
    return res;
}

matrix Aes::invShiftRows(const matrix & m)
{
    matrix res;
    res[0] = m[0];
    res[1] = shiftRight(m[1], 1);
    res[2] = shiftRight(m[2], 2);
    res[3] = shiftRight(m[3], 3);

    return res;
}

matrix Aes::mixColumns(const matrix & m)
{
    word tmp;
    matrix res;

    for (int i = 0; i < m.size(); i++)
    {
        tmp[0] = dot(c_x[0][0], m[0][i]) ^ dot(c_x[0][1], m[1][i]) ^
                 dot(c_x[0][2], m[2][i]) ^ dot(c_x[0][3], m[3][i]);
        tmp[1] = dot(c_x[1][0], m[0][i]) ^ dot(c_x[1][1], m[1][i]) ^
                 dot(c_x[1][2], m[2][i]) ^ dot(c_x[1][3], m[3][i]);
        tmp[2] = dot(c_x[2][0], m[0][i]) ^ dot(c_x[2][1], m[1][i]) ^
                 dot(c_x[2][2], m[2][i]) ^ dot(c_x[2][3], m[3][i]);
        tmp[3] = dot(c_x[3][0], m[0][i]) ^ dot(c_x[3][1], m[1][i]) ^
                 dot(c_x[3][2], m[2][i]) ^ dot(c_x[3][3], m[3][i]);

        res[0][i] = tmp[0];
        res[1][i] = tmp[1];
        res[2][i] = tmp[2];
        res[3][i] = tmp[3];
    }

    return res;
}

matrix Aes::invMixColumns(const matrix & m)
{
    word tmp;
    matrix res;

    for (int i = 0; i < m.size(); i++)
    {
        tmp[0] = dot(ic_x[0][0], m[0][i]) ^ dot(ic_x[0][1], m[1][i]) ^
                 dot(ic_x[0][2], m[2][i]) ^ dot(ic_x[0][3], m[3][i]);
        tmp[1] = dot(ic_x[1][0], m[0][i]) ^ dot(ic_x[1][1], m[1][i]) ^
                 dot(ic_x[1][2], m[2][i]) ^ dot(ic_x[1][3], m[3][i]);
        tmp[2] = dot(ic_x[2][0], m[0][i]) ^ dot(ic_x[2][1], m[1][i]) ^
                 dot(ic_x[2][2], m[2][i]) ^ dot(ic_x[2][3], m[3][i]);
        tmp[3] = dot(ic_x[3][0], m[0][i]) ^ dot(ic_x[3][1], m[1][i]) ^
                 dot(ic_x[3][2], m[2][i]) ^ dot(ic_x[3][3], m[3][i]);

        res[0][i] = tmp[0];
        res[1][i] = tmp[1];
        res[2][i] = tmp[2];
        res[3][i] = tmp[3];
    }

    return res;
}

word Aes::gFun(const word & w, const uint8_t & rc)
{
    auto ww = shiftLeft(w, 1); // Циклический сдвиг на 1 байт влево

    // Замена байт, полученных на прошлом шаге, в соответствии с таблицей s-box
    for (auto & el : ww)
        el = subBytes(el);

    ww[0] ^= rc; // суммирование с раундовой постоянной

    return ww;
}

word Aes::sumWords(const word & w1, const word & w2)
{
    return { uint8_t(w1[0]^w2[0]), uint8_t(w1[1]^w2[1]),
             uint8_t(w1[2]^w2[2]), uint8_t(w1[3]^w2[3]) };
}

matrix Aes::sumMatrix(const matrix & m1, const matrix & m2)
{
    matrix res;
    for (int i = 0; i < m1.size(); i++)
        res[i] = sumWords(m1[i], m2[i]);
    return res;
}

void Aes::keyExpansion()
{
    ws[0] = { m_key[0],  m_key[1],  m_key[2],  m_key[3]  };
    ws[1] = { m_key[4],  m_key[5],  m_key[6],  m_key[7]  };
    ws[2] = { m_key[8],  m_key[9],  m_key[10], m_key[11] };
    ws[3] = { m_key[12], m_key[13], m_key[14], m_key[15] };

    for (int i = 4; i < ws.size(); i++)
    {
        if (i % 4 == 0) ws[i] = sumWords(ws[i-4], gFun(ws[i-1], rc[i/4-1]));
        else ws[i] = sumWords(ws[i-1], ws[i-4]);
    }
}

matrix Aes::encryptRound(const matrix & state, const matrix & key, const int & rn)
{
    matrix tmp = state;
    // Pre round
    // Only add inital key
    if (rn == 0)
        return sumMatrix(state, key);

    // Sub bytes
    for (int i = 0; i < tmp.size(); i++)
        for (int j = 0; j < tmp[i].size(); j++)
            tmp[i][j] = subBytes(tmp[i][j]);

    tmp = shiftRows(tmp);

    // Last round without this operation
    if (rn != 10)
        tmp = mixColumns(tmp);

    // Add round key
    tmp = sumMatrix(tmp, key);

    return tmp;
}

blck Aes::encryptBlck(const blck & input)
{
    if (!m_have_key) return { };

    matrix tmp = { input[0], input[4], input[8],  input[12],
                   input[1], input[5], input[9],  input[13],
                   input[2], input[6], input[10], input[14],
                   input[3], input[7], input[11], input[15]
                 };

    matrix r_key;

    for (int i = 0; i <= 10; i++)
    {
        r_key = { word({ws[i*4][0], ws[i*4+1][0], ws[i*4+2][0], ws[i*4+3][0]}),
                  word({ws[i*4][1], ws[i*4+1][1], ws[i*4+2][1], ws[i*4+3][1]}),
                  word({ws[i*4][2], ws[i*4+1][2], ws[i*4+2][2], ws[i*4+3][2]}),
                  word({ws[i*4][3], ws[i*4+1][3], ws[i*4+2][3], ws[i*4+3][3]}),
                };

        tmp = encryptRound(tmp, r_key, i);
    }

    blck res;
    for (int i = 0; i < tmp.size(); i++)
        for (int j = 0; j < tmp[i].size(); j++)
            res[4*i+j] = tmp[j][i];

    return res;
}

matrix Aes::decryptRound(const matrix & state, const matrix & key, const int & rn)
{
    matrix tmp = state;
    // Pre round
    // Only add inital key
    if (rn == 0)
        return sumMatrix(state, key);

    tmp = invShiftRows(tmp);

    // Inv Sub bytes
    for (int i = 0; i < tmp.size(); i++)
        for (int j = 0; j < tmp[i].size(); j++)
            tmp[i][j] = invSubBytes(tmp[i][j]);

    // Add round key
    tmp = sumMatrix(tmp, key);

    // Last round without this operation
    if (rn != 10)
        tmp = invMixColumns(tmp);

    return tmp;
}

blck Aes::decryptBlck(const blck & input)
{
    if (!m_have_key) return { };

    matrix tmp = { input[0], input[4], input[8],  input[12],
                   input[1], input[5], input[9],  input[13],
                   input[2], input[6], input[10], input[14],
                   input[3], input[7], input[11], input[15]
                 };

    matrix r_key;

    for (int i = 0; i <= 10; i++)
    {
        int idx = ws.size() - 4 - i*4;
        r_key = { word({ws[idx][0], ws[idx+1][0], ws[idx+2][0], ws[idx+3][0]}),
                  word({ws[idx][1], ws[idx+1][1], ws[idx+2][1], ws[idx+3][1]}),
                  word({ws[idx][2], ws[idx+1][2], ws[idx+2][2], ws[idx+3][2]}),
                  word({ws[idx][3], ws[idx+1][3], ws[idx+2][3], ws[idx+3][3]}),
                };

        tmp = decryptRound(tmp, r_key, i);
    }

    blck res;
    for (int i = 0; i < tmp.size(); i++)
        for (int j = 0; j < tmp[i].size(); j++)
            res[4*i+j] = tmp[j][i];

    return res;
}

bytearray Aes::encrypt(bytearray input, const MODE & mode, const blck & iv)
{
    if (input.size() % iv.size() != 0)
        input.insert(input.end(), (iv.size()-(input.size()%iv.size())), 0);

    bytearray output;
    blck tmp;
    auto it = input.begin();

    switch (mode)
    {
        case MODE::ECB:
            for (int i = 0; i < input.size(); i += iv.size())
            {
                for (int j = 0; j < iv.size(); j++)
                    tmp[j] = input[i+j];

                tmp = encryptBlck(tmp);
                for (auto & el : tmp)
                    output.push_back(el);
            }
            break;

        case MODE::CBC:
        {
            for (int i = 0; i < input.size(); i += iv.size())
            {
                for (int j = 0; j < iv.size(); j++)
                    if (i == 0) tmp[j] = input[i+j] ^ iv[j];
                    else tmp[j] = input[i+j] ^ tmp[j];

                tmp = encryptBlck(tmp);
                for (auto & el : tmp)
                    output.push_back(el);
            }
        }
            break;
        default:
            std::cout << "Requested mode not supported" << std::endl;
            break;
    }

    return output;
}

bytearray Aes::decrypt(bytearray input, const MODE & mode, const blck & iv)
{
    bytearray output;
    blck tmp;

    switch (mode)
    {
        case MODE::ECB:
        {
            if (input.size() % iv.size() != 0)
            {
                std::cout << "Wrong input size" << std::endl;
                break;
            }

            for (int i = 0; i < input.size(); i += iv.size())
            {
                for (int j = 0; j < iv.size(); j++)
                    tmp[j] = input[i+j];

                tmp = decryptBlck(tmp);

                for (auto & el : tmp)
                    output.push_back(el);
            }
        }
            break;
        case MODE::CBC:
        {
            blck prev_in;
            for (int i = 0; i < input.size(); i += iv.size())
            {
                prev_in = tmp;
                for (int j = 0; j < iv.size(); j++)
                    tmp[j] = input[j+i];

                tmp = decryptBlck(tmp);

                for (int j = 0; j < tmp.size(); j++)
                    if (i == 0) output.push_back(tmp[j] ^ iv[j]);
                    else output.push_back(tmp[j]);
            }
        }
            break;
        default:
            std::cout << "Requested mode not supported" << std::endl;
            break;
    }

    return output;
}