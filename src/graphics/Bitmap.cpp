/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Bitmap.hpp"

namespace mc::graphics {

static BitmapResult CreateTexture(const char* data, int width, int height);

BitmapResult ReadBitmap(const std::string& filePath) {
    std::ifstream fs(filePath, std::ios::in | std::ios::binary);
    DEBUG("Reading bitmap: \"", filePath, "\"");

    if (fs.is_open()) {
        char header[54];
        std::vector<char> buffer;

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
        fs.read(buffer.data(), imageSize);
        if (fs.gcount() != imageSize) {
            throw ApplicationError(
                "BitmapError",
                __FUNCTION__,
                "Read only " + std::to_string(fs.gcount()) + " bytes of data, but expected to read" + std::to_string(imageSize));
        }
        DEBUG("Successfully read bitmap");
        return CreateTexture(buffer.data(), width, height);
    }
    throw ApplicationError(
        "BitmapError",
        __FUNCTION__,
        "Could not open \"" + filePath + "\"");
}

static BitmapResult CreateTexture(const char* data, int width, int height) {
    GLuint id;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return BitmapResult { id, Point2i(width, height) };
}

}   // namespace mc::graphics
