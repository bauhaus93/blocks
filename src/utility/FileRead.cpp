#include "FileRead.hpp"

namespace mc {

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

}   // namespace mc
