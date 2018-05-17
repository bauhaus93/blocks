/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Facetree.hpp"

namespace mc::world::chunk {

Facetree::Facetree():
    Facetree(Point2i8(0), CHUNK_SIZE) {
}

Facetree::Facetree(Point2i8 origin_, int8_t size_):
    origin { origin_ },
    size { size_ },
    faceInfo { nullptr },
    children { { nullptr, nullptr, nullptr, nullptr } } {
}

uint8_t Facetree::GetQuadrant(Point2i8 pos) const {
    uint8_t index = 0;
    for (uint8_t i = 0; i < 2; i++) {
        if (pos[i] >= origin[i] + size / 2) {
            index |= (1 << i);
        }
    }
    assert(index < 4);
    return index;
}

void Facetree::CreateQuads(const BlockManager& blockManager,
    uint8_t axis,
    uint8_t layer,
    QuadVec& quads) const {

    static const std::array<std::array<Point2f, 4>, 6> vertexOffset = { {
        // Direction::NORTH
        { { Point2f(BLOCK_SIZE, 0.0f),
            Point2f(0.0f),
            Point2f(0.0f, BLOCK_SIZE),
            Point2f(BLOCK_SIZE, BLOCK_SIZE) } },
        // Direction::EAST
        { { Point2f(0.0f),
            Point2f(BLOCK_SIZE, 0.0f),
            Point2f(BLOCK_SIZE, BLOCK_SIZE),
            Point2f(0.0f, BLOCK_SIZE ) } },
        // Direction::SOUTH
        { { Point2f(0.0f),
            Point2f(BLOCK_SIZE, 0.0f),
            Point2f(BLOCK_SIZE, BLOCK_SIZE),
            Point2f(0.0f, BLOCK_SIZE) } },
        // Direction::WEST
        { { Point2f(BLOCK_SIZE, 0.0f),
            Point2f(0.0f, 0.0f),
            Point2f(0.0f, BLOCK_SIZE),
            Point2f(BLOCK_SIZE, BLOCK_SIZE) } },
        // Direction::UP
        { { Point2f(0.0f, 0.0f),
            Point2f(BLOCK_SIZE, 0.0f),
            Point2f(BLOCK_SIZE, BLOCK_SIZE),
            Point2f(0.0f, BLOCK_SIZE) } },
        // Direction::DOWN
        { { Point2f(0.0f, BLOCK_SIZE),
            Point2f(BLOCK_SIZE, BLOCK_SIZE),
            Point2f(BLOCK_SIZE, 0.0f),
            Point2f(0.0f) } }
    } };
    static const std::array<Point2f, 4> vertexUV = { {
        Point2f(1.0f, 1.0f),
        Point2f(0.0f, 1.0f),
        Point2f(0.0f),
        Point2f(1.0f, 0.0f)
    } };
    static const std::array<Point3f, 6> vertexNormal = { {
        Point3f(0.0f, -1.0f, 0.0f), // Direction::NORTH
        Point3f(1.0f, 0.0f, 0.0f),  // Direction::EAST
        Point3f(0.0f, 1.0f, 0.0f),  // Direction::SOUTH
        Point3f(-1.0f, 0.0f, 0.0f), // Direction::WEST
        Point3f(0.0f, 0.0f, 1.0f),  // Direction::UP
        Point3f(0.0f, 0.0f, -1.0f)  // Direction::DOWN
    } };

    if (faceInfo != nullptr) {
        if (faceInfo->type != BlockType::NONE) {
            mesh::Quad quad;
            for (uint8_t i = 0; i < 4; i++) {
                Point3f pos;
                Point3f uv { vertexUV[i][0] * static_cast<float>(size),
                             vertexUV[i][1] * static_cast<float>(size),
                             static_cast<float>(blockManager.GetBlockFace(faceInfo->type, faceInfo->dir))
                };

                auto& offset = vertexOffset[GetIndex(faceInfo->dir)][i];
                uint8_t currIndex = 0;
                for (uint8_t j = 0; j < 3; j++) {
                    if (j != axis) {
                        pos[j] = origin[currIndex] * BLOCK_SIZE + offset[currIndex] * size;
                        currIndex++;
                    } else {
                        pos[j] = static_cast<float>(layer) * BLOCK_SIZE;
                    }
                }
                quad.SetVertex(i, mesh::Vertex(pos, uv, vertexNormal[GetIndex(faceInfo->dir)]));
            }
            quads.emplace_back(std::move(quad));
        }
    } else {
        for (auto& child: children) {
            if (child != nullptr) {
                child->CreateQuads(blockManager, axis, layer, quads);
            }
        }
    }
}

void Facetree::InsertFaces(std::vector<Face> faces) {
    std::array<std::vector<Face>, 4> subSplit;

    for (auto& f : faces) {
        if (f.GetOrigin() == origin && f.GetSize() == size) {
            if (!IsFace()) {
                SetFace(FaceInfo(f.GetType(), f.GetDirection()));
            } else {
                SplitFaceToChildren(*faceInfo);
                SplitFaceToChildren(FaceInfo(f.GetType(), f.GetDirection()));
                SetFaceNull();
            }
        } else {
            uint8_t quadrant = GetQuadrant(f.GetOrigin());
            subSplit[quadrant].emplace_back(std::move(f));
        }
    }

    for (uint8_t i = 0; i < 4; i++) {
        if (subSplit[i].size() > 0) {
            if (IsFace()) {
                SplitFaceToChildren(*faceInfo);
                SetFaceNull();
            } else if (children[i] == nullptr) {
                CreateChild(i);
            }
            children[i]->InsertFaces(std::move(subSplit[i]));
        }
    }
    FaceInfo merge = IsMergeable();
    if (merge.type != BlockType::NONE) {
        SetFace(merge);
        DeleteChildren();
    }
}

void Facetree::GetFaces(std::vector<Face>& faces) const {
    if (faceInfo != nullptr) {
        faces.emplace_back(faceInfo->type, faceInfo->dir, origin, size);
    } else {
        for (auto& child: children) {
            if (child != nullptr) {
                child->GetFaces(faces);
            }
        }
    }
}

void Facetree::SplitFaceToChildren(const FaceInfo& info) {
    if (size > 1) {
        for (uint8_t i = 0; i < 4; i++) {
            if (children[i] == nullptr) {
                CreateChild(i);
                children[i]->SetFace(info);
            } else {
                if (children[i]->IsFace()) {    // assumes up to 2 faces can have same positions
                    children[i]->SplitFaceToChildren(*children[i]->faceInfo);
                    children[i]->SplitFaceToChildren(info);
                    children[i]->SetFaceNull();
                } else {
                    children[i]->SetFace(info);
                }
            }
        }
    } else {
        SetFaceNull();
    }
}

FaceInfo Facetree::IsMergeable() {
    bool foundOne = false;
    FaceInfo merge(BlockType::NONE, Direction::NORTH);
    for (uint8_t i = 0; i < 4; i++) {
        if (children[i] == nullptr || !children[i]->IsFace()) {
            return FaceInfo(BlockType::NONE, Direction::NORTH);
        }
        FaceInfo curr = children[i]->GetFace();
        if (!foundOne) {
            if (curr.type != BlockType::NONE) {
                foundOne = true;
                merge = curr;
            } else {
                return FaceInfo(BlockType::NONE, Direction::NORTH);
            }
        } else if (curr.type != merge.type || curr.dir != merge.dir) {
                return FaceInfo(BlockType::NONE, Direction::NORTH);
        }
    }
    return merge;
}

void Facetree::SetFace(const FaceInfo& face) {
    faceInfo = std::make_unique<FaceInfo>(face.type, face.dir);
}

void Facetree::SetFaceNull() {
    faceInfo = nullptr;
}

bool Facetree::IsFace() const {
    return faceInfo != nullptr;
}

void Facetree::CreateChild(uint8_t quadrant) {
    assert(children[quadrant] == nullptr);
    assert(size / 2 > 0);
    Point2i8 childOrigin(origin);
    for (int8_t j = 0; j < 2; j++) {
        if (((quadrant >> j) & 1) != 0) {
            childOrigin[j] += size / 2;
        }
    }
    children[quadrant] = std::make_unique<Facetree>(childOrigin, size / 2);
}

void Facetree::DeleteChild(uint8_t quadrant) {
    children[quadrant] = nullptr;
}

void Facetree::DeleteChildren() {
    for (uint8_t i = 0; i < 4; i++) {
        DeleteChild(i);
    }
}

}   // mc::world::chunk
