/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Facetree.hpp"

namespace mc::world::chunk {

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

void Facetree::CreateQuads(const ProtoBlockMap& protoblocks,
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
                             static_cast<float>(protoblocks.at(faceInfo->type).GetFace(faceInfo->dir))
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

                //INFO("Dir = ", faceInfo->dir, ", layer = ", (int)layer, ", pos = ", pos);
                quad.SetVertex(i, mesh::Vertex(pos, uv, vertexNormal[GetIndex(faceInfo->dir)]));
            }
            quads.emplace_back(std::move(quad));
        }
    } else {
        for (auto& child: children) {
            if (child != nullptr) {
                child->CreateQuads(protoblocks, axis, layer, quads);
            }
        }
    }
}

void Facetree::InsertFaces(std::vector<Face> faces) {
    std::array<std::vector<Face>, 4> subSplit;

    for (auto& f : faces) {
        if (f.origin == origin && f.size == size) {
            if (!IsFace()) {
                //INFO("Found right face @ ", origin, ", size = ", static_cast<int>(size));
                SetFace(f.info);
            } else {
                //INFO("Found right face, but already has equal face");
                SetFaceNull();
            }
        } else {
            uint8_t quadrant = GetQuadrant(f.origin);
            subSplit[quadrant].emplace_back(std::move(f));
        }
    }

    for (uint8_t i = 0; i < 4; i++) {
        if (subSplit[i].size() > 0) {
            if (children[i] == nullptr) {
                CreateChild(i);
            }
            children[i]->InsertFaces(std::move(subSplit[i]));
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
                    //INFO("Resplit bc overlap @ ", children[i]->origin, ", size = ", children[i]->size);
                    children[i]->SplitFaceToChildren(*children[i]->faceInfo);
                    children[i]->SplitFaceToChildren(info);
                    children[i]->SetFaceNull();
                } else {
                    children[i]->SetFace(info);
                }
            }
        }
    } else {
        //INFO("Set to null bc EOT");
        SetFaceNull();
    }
}

void Facetree::SetFace(const FaceInfo& info) {
    //INFO("SetFace @ ", origin, ", size = ", static_cast<int>(size), ", dir = ", info.dir);
    faceInfo = std::make_unique<FaceInfo>(info);
}

void Facetree::SetFaceNull() {
    //INFO("SetFaceNull @ ", origin, ", size = ", static_cast<int>(size), ", dir = ", faceInfo->dir);
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
