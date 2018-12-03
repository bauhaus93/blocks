/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Bitmap.hpp"

namespace blocks {

struct BmpFileHeader {
    uint16_t    type;
    uint32_t    size;
    uint32_t    reserved;
    uint32_t    offBits;
} __attribute__((packed));  //gcc only

struct BmpInfoHeader {
    uint32_t    infoSize;
    int32_t     width;
    int32_t     height;
    uint16_t    planes;
    uint16_t    bitCount;
    uint32_t    compression;
    uint32_t    imgSize;
    int32_t     XPelsPerMeter;
    int32_t     YPelsPerMeter;
    uint32_t    clrUsed;
    uint32_t    clrImportant;
} __attribute__((packed));  //gcc only

Image ReadBitmap(const std::string& filePath) {
    std::vector<uint8_t> buffer = ReadFileRaw(filePath);
    std::vector<uint8_t> imgData;
    const BmpFileHeader* fh = reinterpret_cast<const BmpFileHeader*>(buffer.data());
    const BmpInfoHeader* ih = reinterpret_cast<const BmpInfoHeader*>(buffer.data() + 14);

    DEBUG("Reading bmp: size = ", ih->width, "/", ih->height, ", offset = ", fh->offBits, ", depth = ", ih->bitCount);
    assert(ih->height > 0);
    assert(ih->bitCount == 24);
    uint32_t lineSize = static_cast<uint32_t>(ih->width) * ih->bitCount / 8;

    imgData.reserve(lineSize * ih->height);
    imgData.insert(imgData.begin(),
                    buffer.begin() + fh->offBits,
                    buffer.end());
    

    auto startLo = imgData.begin();
    auto startHi = imgData.end() - lineSize;
    while (startLo < startHi) {
        std::swap_ranges(startLo, startLo + lineSize, startHi);
        startLo += lineSize;
        startHi -= lineSize;
    }

    for (auto iter = imgData.begin(); iter != imgData.end(); iter += 3) {
        std::swap(*iter, *(iter + 2));
    }

    return Image { std::move(imgData),
                   Point2u(static_cast<uint32_t>(ih->width), static_cast<uint32_t>(ih->height)),
                   static_cast<uint8_t>(ih->bitCount) };
}

}   // namespace blocks
