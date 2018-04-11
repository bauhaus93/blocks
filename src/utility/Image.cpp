/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Image.hpp"

namespace mc {

Image::Image(std::vector<uint8_t> data_, Point2u size_, uint8_t depth_):
    data { data_ },
    size { size_ },
    depth { depth_ } {
}

Image::Image(Image&& other):
    data { std::move(other.data) },
    size { other.size },
    depth { other.depth } {
}

Image& Image::operator=(Image&& other) {
    data = std::move(other.data);
    size = other.size;
    depth = other.depth;
    return *this;
}

Image Image::CreateSubImage(Point2u origin, Point2u subSize) const {
    assert(origin < size);
    assert(origin + subSize <= size);
    std::vector<uint8_t> subData;
    uint32_t lineSize = size[0] * depth / 8;
    uint32_t subLineSize = subSize[0] * depth / 8;
    auto start = data.begin() + origin[1] * lineSize + origin[0];
    for (uint32_t i = 0; i < subSize[1]; i++) {
        subData.insert(subData.end(),
                       start,
                       start + subLineSize);
        start += lineSize;
    }
    return Image { std::move(subData), subSize, depth };
}


}   // namespace mc
