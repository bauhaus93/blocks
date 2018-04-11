/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <vector>
#include <cstdint>

#include "utility/Point2.hpp"

namespace mc {

class Image {
 public:
                Image(std::vector<uint8_t> data_, Point2u size_, uint8_t depth_);
                Image(Image&& other);
    Image&      operator=(Image&& other);
    const std::vector<uint8_t>  GetData() const { return data; }
    Point2u                     GetSize() const { return size; }
    uint8_t                     GetDepth() const { return depth; }

    Image       CreateSubImage(Point2u origin, Point2u subSize) const;
 private:
    std::vector<uint8_t>    data;
    Point2u                 size;
    uint8_t                 depth;
 };



}   // namespace mc
