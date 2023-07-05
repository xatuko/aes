#include "aes.hpp"

void Aes::setKey(const blck & key)
{
    m_key = { key.data(), key.size() };
}

void Aes::setIv(const blck & iv)
{
    m_iv = { iv.data(), iv.size() };
}

bytearray Aes::encrypt(const bytearray & input, const MODE & mode)
{
    if (m_key.empty())
        return { };

    switch (mode)
    {
        case MODE::ECB:
        case MODE::CFB:
        case MODE::CTR:
        case MODE::OFB:
            // not supported
            break;
        case MODE::CBC:
            return encryptCBC(input);
    }

    return {};
}

bytearray Aes::decrypt(const bytearray & input, const MODE & mode)
{
    switch (mode)
    {
        case MODE::ECB:
        case MODE::CFB:
        case MODE::CTR:
        case MODE::OFB:
            // not supported
            break;
        case MODE::CBC:
            return decryptCBC(input);
    }

    return {};
}

bytearray Aes::encryptCBC(const bytearray & input)
{
    bytearray result;

    if (m_key.empty() || m_iv.empty())
        return result;

    try
    {
        crypto::CBC_Mode<crypto::AES>::Encryption enc;
        enc.SetKeyWithIV(m_key, m_key.size(), m_iv);
        crypto::VectorSource source(input, true,
            new crypto::StreamTransformationFilter(enc,
                new crypto::VectorSink(result)));
    }
    catch (const crypto::Exception & e)
    {
        std::cerr << e.what() << std::endl;
        return { };
    }

    return result;
}

bytearray Aes::decryptCBC(const bytearray & input)
{
    bytearray result;

    if (m_key.empty() || m_iv.empty())
        return result;

    try
    {
        crypto::CBC_Mode<crypto::AES>::Decryption dec;
        dec.SetKeyWithIV(m_key, m_key.size(), m_iv);
        crypto::VectorSource source(input, true,
            new crypto::StreamTransformationFilter(dec,
                new crypto::VectorSink(result)));
    }
    catch (const crypto::Exception & e)
    {
        std::cerr << e.what() << std::endl;
        return { };
    }

    return result;

}