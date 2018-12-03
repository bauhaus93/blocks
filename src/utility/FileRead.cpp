#include "FileRead.hpp"

namespace blocks {

std::string ReadFile(const std::string& filePath) {
    std::ifstream fs(filePath, std::ios::in);
    std::stringstream ss;
    if (fs.is_open()) {
        ss << fs.rdbuf();
    }
    else {
        throw FileNotFoundError(__FUNCTION__, filePath);
    }

    return ss.str();
}

std::vector<uint8_t> ReadFileRaw(const std::string& filePath) {
    assert(sizeof(uint8_t) == sizeof(char));
    DEBUG("Reading raw file: \"", filePath, "\"");
    std::ifstream fs(filePath, std::ios::in | std::ios::binary);
    std::vector<uint8_t> buffer;

    if (fs.is_open()) {
        fs.seekg(0, std::ios::end);
        auto size = fs.tellg();
        fs.seekg(0, std::ios::beg);
        buffer.resize(size);
        fs.read(reinterpret_cast<char*>(buffer.data()), size);
    } else {
        throw FileNotFoundError(__FUNCTION__, filePath);
    }
    return buffer;
}

}   // namespace blocks
