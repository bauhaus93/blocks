/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <memory>
#include <cstdint>
#include <cassert>
#include <algorithm>

#include "logger/GlobalLogger.hpp"
#include "utility/Point2.hpp"
#include "world/Size.hpp"
#include "world/Direction.hpp"
#include "world/BlockType.hpp"

namespace mc::world::chunk {

class Facetree;

typedef std::array<std::unique_ptr<Facetree>, 4> FacetreeArray;

struct FaceInfo {
    BlockType type;
    Direction dir;
    FaceInfo(BlockType type_, Direction dir_):
        type { type_ },
        dir { dir_ } {
    }
};

class Facetree {
 public:
            Facetree(Point2i8 origin_, int8_t size_);
            Facetree(Facetree&& other) = default;
    void    InsertFace(const FaceInfo& info, Point2i8 faceOrigin, int8_t faceSize);

 private:

    void    SplitInsertFace(const FaceInfo& info);
    void    SetFace(const FaceInfo& info);
    void    SetFaceNone();
    bool    IsFace() const;
    void    CreateChild(uint8_t index);
    void    DeleteChild(uint8_t index);
    void    DeleteChildren();

    Point2i8                    origin;
    int8_t                      size;
    std::unique_ptr<FaceInfo>   faceInfo;
    FacetreeArray               children;
};


}   // mc::world::chunk