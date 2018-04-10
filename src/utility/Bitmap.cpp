/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Bitmap.hpp"

namespace mc {

BitmapResult ReadBitmap(const std::string& filePath) {
    assert(sizeof(char) == sizeof(uint8_t));
    std::ifstream fs(filePath, std::ios::in | std::ios::binary);
    DEBUG("Reading bitmap: \"", filePath, "\"");

    if (fs.is_open()) {
        char header[54];
        std::vector<uint8_t> buffer;

        fs.read(header, 54);
        if (fs.gcount() != 54) {
            throw ApplicationError(
                "BitmapError",
                __FUNCTION__,
                "Read only " + std::to_string(fs.gcount()) + " bytes for header, but expected 54");
        }

        if (header[0] != 'B' || header[1] != 'M')
            throw ApplicationError(
                "BitmapError",
                __FUNCTION__,
                "File \"" + filePath + "\" is no bitmap file");

        uint32_t dataPos = *reinterpret_cast<uint32_t*>(&header[0x0A]);
        uint32_t imageSize = *reinterpret_cast<uint32_t*>(&header[0x22]);
        int32_t width = *reinterpret_cast<int32_t*>(&header[0x12]);
        int32_t height = *reinterpret_cast<int32_t*>(&header[0x16]);

        if (dataPos == 0)
            dataPos = 54;

        if (imageSize == 0)
            imageSize = 3 * width * height;

        //imageSize -= dataPos;

        fs.seekg(dataPos, fs.beg);

        buffer.resize(imageSize, 0);
        fs.read(reinterpret_cast<char*>(buffer.data()), imageSize);
        if (fs.gcount() != imageSize) {
            throw ApplicationError(
                "BitmapError",
                __FUNCTION__,
                "Read only " + std::to_string(fs.gcount()) + " bytes of data, but expected to read" + std::to_string(imageSize));
        }
        DEBUG("Successfully read bitmap");
        return BitmapResult { std::move(buffer), Point2i(width, height) };
    }
    throw ApplicationError(
        "BitmapError",
        __FUNCTION__,
        "Could not open \"" + filePath + "\"");
}

}   // namespace mc
