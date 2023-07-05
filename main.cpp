#include "aes.hpp"


int main(int,char**)
{
    blck key = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    blck iv = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    bytearray data = {3,3,2,6,2,2,5,6,2,243,6};
    Aes aes;
    aes.setIv(iv);
    aes.setKey(key);

    std::cout << "Input: ";
    for (auto el : data)
        std::cout << int(el) << ' ';
    std::cout << std::endl;

    auto enc = aes.encrypt(data, MODE::CBC);
    std::cout << "Encrypted: ";
    for (auto el : enc)
        std::cout << int(el) << ' ';
    std::cout << std::endl;

    auto dec = aes.decrypt(enc, MODE::CBC);
    std::cout << "Decrypted: ";
    for (auto el : dec)
        std::cout << int(el) << ' ';
    std::cout << std::endl;
    return 0;
}
