/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <memory>
#include <map>
#include <cstdint>
#include <cassert>
#include <algorithm>
#include <vector>
#include <bitset>

#include "logger/GlobalLogger.hpp"
#include "utility/Point2.hpp"
#include "utility/Point3.hpp"
#include "world/Size.hpp"
#include "world/Direction.hpp"
#include "world/BlockManager.hpp"
#include "world/ProtoBlock.hpp"
#include "mesh/Quad.hpp"
#include "mesh/Vertex.hpp"
#include "Face.hpp"

namespace blocks{

class Facetree;

typedef std::array<std::unique_ptr<Facetree>, 4> FacetreeArray;
typedef std::vector<Quad>                  QuadVec;

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
                    Facetree();
                    Facetree(Point2i8 origin_, int8_t size_);
                    Facetree(Facetree&& other) = default;
    Facetree&       operator=(Facetree&& rhs) = default;

    void            InsertFaces(std::vector<Face> faces);
    void            RemoveFaces(Direction faceDir);
    void            GetFaces(std::vector<Face>& faces) const;
    void            CreateQuads(const BlockManager& blockManager,
                                uint8_t axis,
                                uint8_t layer,
                                QuadVec& quads) const;
    void            CreateQuadsByDirection(const BlockManager& blockManager,
                                uint8_t axis,
                                uint8_t layer,
                                QuadVec& quads,
                                Direction dir) const;
 private:

    void            InsertFacesSorted(std::vector<Face> faces);
    uint8_t GetQuadrant(Point2i8 pos) const;
    void    SplitFaceToChildren(const FaceInfo& info);
    void    SetFace(const FaceInfo& face);
    void    SetFaceNull();
    bool    IsFace() const;
    void    CreateChild(uint8_t quadrant);
    void    DeleteChild(uint8_t quadrant);
    void    DeleteChildren();
    const FaceInfo& GetFace() const { assert(faceInfo != nullptr); return *faceInfo; }
    FaceInfo        IsMergeable();

    Point2i8                    origin;
    int8_t                      size;
    std::unique_ptr<FaceInfo>   faceInfo;
    FacetreeArray               children;
};


}   // chunk
