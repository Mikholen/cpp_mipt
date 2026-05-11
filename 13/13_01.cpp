#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <string>
#include <stdexcept>

std::string bytesToHex(const std::vector<std::uint8_t>& data)
{
    std::stringstream ss;

    for (std::uint8_t byte : data)
    {
        ss << std::hex
           << std::right
           << std::setw(2)
           << std::setfill('0')
           << static_cast<int>(byte);
    }

    return ss.str();
}

std::uint8_t hexCharToValue(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';

    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;

    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;

    throw std::runtime_error("Invalid hex character");
}

std::vector<std::uint8_t> hexToBytes(const std::string& hex)
{
    if (hex.size() % 2 != 0)
        throw std::runtime_error("Invalid hex string length");

    std::vector<std::uint8_t> result;

    for (std::size_t i = 0; i < hex.size(); i += 2)
    {
        std::uint8_t high = hexCharToValue(hex[i]);
        std::uint8_t low  = hexCharToValue(hex[i + 1]);

        std::uint8_t byte = (high << 4) | low;

        result.push_back(byte);
    }

    return result;
}

int main()
{
    std::vector<std::uint8_t> data =
    {
        0x12, 0xAB, 0x05, 0xFF, 0x80
    };

    std::string hex = bytesToHex(data);

    std::cout << hex << std::endl;

    std::vector<std::uint8_t> restored = hexToBytes(hex);

    for (std::uint8_t byte : restored)
    {
        std::cout << "0x"
                  << std::hex
                  << std::setw(2)
                  << std::setfill('0')
                  << static_cast<int>(byte)
                  << ' ';
    }

    std::cout << std::endl;

    return 0;
}