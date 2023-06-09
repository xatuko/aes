#include "aes.hpp"

int main(int,char**)
{
    Aes aes;
    bytearray input = { 0x54, 0x77, 0x6f, 0x20, 0x4f, 0x6e, 0x65, 0x20, 0x4e, 0x69, 0x6e, 0x65, 0x20, 0x54, 0x77, 0x6f};
    blck key = { 0x54, 0x68, 0x61, 0x74, 0x73, 0x20, 0x6d, 0x79, 0x20, 0x4b, 0x75, 0x6e, 0x67, 0x20, 0x46, 0x75 };

    aes.setKey(key);
    aes.setIv({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});

    auto res = aes.encrypt(input, MODE::CBC);

    std::cout << "Input: ";
    for (auto & el : input)
        std::cout << std::hex << int(el) << ' ';
    std::cout << std::endl;

    std::cout << "Key: ";
    for (auto & el : key)
        std::cout << std::hex << int(el) << ' ';
    std::cout << std::endl;

    std::cout << "Result: ";
    for (auto & el : res)
        std::cout << std::hex << int(el) << ' ';
    std::cout << std::endl;

    res = aes.decrypt(res, MODE::CBC);

    std::cout << "Decrypt: ";
    for (auto & el : res)
        std::cout << std::hex << int(el) << ' ';
    std::cout << std::endl;

    return 0;
}
