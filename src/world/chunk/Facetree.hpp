/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <memory>
#include <map>
#include <cstdint>
#include <cassert>
#include <algorithm>
#include <vector>

#include "logger/GlobalLogger.hpp"
#include "utility/Point2.hpp"
#include "utility/Point3.hpp"
#include "world/Size.hpp"
#include "world/Direction.hpp"
#include "world/BlockType.hpp"
#include "world/ProtoBlock.hpp"
#include "mesh/Quad.hpp"
#include "mesh/Vertex.hpp"

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

struct Face {
    FaceInfo info;
    Point2i8 origin;
    int8_t   size;
    Face(BlockType type_, Direction dir_, Point2i8 origin_, int8_t size_):
        info { type_, dir_ },
        origin { origin_ },
        size { size_ } {
    }
};

class Facetree {
 public:
                    Facetree(Point2i8 origin_, int8_t size_);
                    Facetree(Facetree&& other) = default;
    void            InsertFaces(std::vector<Face> faces);
    void            CreateQuads(const std::map<BlockType, ProtoBlock>& protoblocks,
                                uint8_t axis,
                                uint8_t layer,
                                std::vector<mesh::Quad>& quads) const;

 private:

    uint8_t GetQuadrant(Point2i8 pos) const;
    void    SplitFaceToChildren(const FaceInfo& info);
    void    SetFace(const FaceInfo& info);
    void    SetFaceNull();
    bool    IsFace() const;
    void    CreateChild(uint8_t quadrant);
    void    DeleteChild(uint8_t quadrant);
    void    DeleteChildren();

    Point2i8                    origin;
    int8_t                      size;
    std::unique_ptr<FaceInfo>   faceInfo;
    FacetreeArray               children;
};


}   // mc::world::chunk
