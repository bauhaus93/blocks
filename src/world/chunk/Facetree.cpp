/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Facetree.hpp"

namespace mc::world::chunk {

Facetree::Facetree(Point2i8 origin_, int8_t size_):
    origin { origin_ },
    size { size_ },
    faceInfo { nullptr },
    children { nullptr, nullptr, nullptr, nullptr } {
}

void Facetree::CreateQuads(const std::map<BlockType, ProtoBlock>& protoblocks,
    uint8_t axis,
    uint8_t layer,
    std::vector<mesh::Quad>& quads) const {

    static const std::array<std::array<Point3f, 4>, 6> vertexOffset = { {
        // Direction::NORTH
        { { Point3f(0.0f),
            Point3f(BLOCK_SIZE, 0.0f, 0.0f),
            Point3f(BLOCK_SIZE, 0.0f, BLOCK_SIZE),
            Point3f(0.0f, 0.0f, BLOCK_SIZE) } },
        // Direction::EAST
        { { Point3f(BLOCK_SIZE, 0.0f, 0.0f),
            Point3f(BLOCK_SIZE, BLOCK_SIZE, 0.0f),
            Point3f(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE),
            Point3f(BLOCK_SIZE, 0.0f, BLOCK_SIZE ) } },
        // Direction::SOUTH
        { { Point3f(BLOCK_SIZE, BLOCK_SIZE, 0.0f),
            Point3f(0.0f, BLOCK_SIZE, 0.0f),
            Point3f(0.0f, BLOCK_SIZE, BLOCK_SIZE),
            Point3f(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE) } },
        // Direction::WEST
        { { Point3f(0.0f, BLOCK_SIZE, 0.0f),
            Point3f(0.0f, 0.0f, 0.0f),
            Point3f(0.0f, 0.0f, BLOCK_SIZE),
            Point3f(0.0f, BLOCK_SIZE, BLOCK_SIZE) } },
        // Direction::UP
        { {  Point3f(0.0f, 0.0f, BLOCK_SIZE),
            Point3f(BLOCK_SIZE, 0.0f, BLOCK_SIZE),
            Point3f(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE),
            Point3f(0.0f, BLOCK_SIZE, BLOCK_SIZE) } },
        // Direction::DOWN
        { {  Point3f(0.0f, BLOCK_SIZE, 0.0f),
            Point3f(BLOCK_SIZE, BLOCK_SIZE, 0.0f),
            Point3f(BLOCK_SIZE, 0.0f, 0.0f),
            Point3f(0.0f) } }
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
                Point3f uv { vertexUV[i][0],
                             vertexUV[i][1],
                             static_cast<float>(protoblocks.at(faceInfo->type).GetFace(faceInfo->dir))
                };
                switch (axis) {
                    case 0:
                        pos[0] = static_cast<float>(layer);
                        pos[1] = origin[0];
                        pos[2] = origin[1];
                        break;
                    case 1:
                        pos[0] = origin[0];
                        pos[1] = static_cast<float>(layer);
                        pos[2] = origin[1];
                        break;
                    case 2:
                        pos[0] = origin[0];
                        pos[1] = origin[1];
                        pos[2] = static_cast<float>(layer);
                        break;
                    default:    assert(0);
                }
                pos += vertexOffset[GetIndex(faceInfo->dir)][i] * static_cast<float>(size);
                quad.SetVertex(i, mesh::Vertex(pos, uv, vertexNormal[i]));
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
            if (faceInfo != nullptr) {
                SetFaceNone();
                break;
            } else {
                SplitInsertFace(f.info);
            }
        } else {
            uint8_t index = 0;
            for (uint8_t i = 0; i < 2; i++) {
                if (f.origin[i] >= origin[i] + size / 2) {
                    index |= (1 << i);
                }
            }
            if (children[index] == nullptr) {
                CreateChild(index);
                subSplit[index].emplace_back(std::move(f));
            } else if (!children[index]->IsFace()) {
                subSplit[index].emplace_back(std::move(f));
            }
        }
    }
    for (uint8_t i = 0; i < 4; i++) {
        if (subSplit[i].size() > 0) {
            children[i]->InsertFaces(std::move(subSplit[i]));
        }
    }
}

void Facetree::SplitInsertFace(const FaceInfo& info) {
    for (uint8_t i = 0; i < 4; i++) {
        if (children[i] == nullptr) {
            if (size > 1) {
                CreateChild(i);
                children[i]->SetFace(info);
            }
        } else {
            if (children[i]->IsFace()) {
                children[i]->SetFaceNone();
            } else if (size > 1) {
                children[i]->SplitInsertFace(info);
            }
        }
    }
}

void Facetree::SetFace(const FaceInfo& info) {
    faceInfo = std::make_unique<FaceInfo>(info);
}

void Facetree::SetFaceNone() {
    if (faceInfo == nullptr) {
        faceInfo = std::make_unique<FaceInfo>(BlockType::NONE, Direction::NORTH);
    } else {
        faceInfo->type = BlockType::NONE;
    }
}

bool Facetree::IsFace() const {
    return faceInfo != nullptr;
}

void Facetree::CreateChild(uint8_t index) {
    assert(children[index] == nullptr);
    assert(size / 2 > 0);
    Point2i8 childOrigin(origin);
    for (int8_t j = 0; j < 2; j++) {
        if (((index >> j) & 1) != 0) {
            childOrigin[j] += size / 2;
        }
    }
    children[index] = std::make_unique<Facetree>(childOrigin, size / 2);
}

void Facetree::DeleteChild(uint8_t index) {
    children[index] = nullptr;
}

void Facetree::DeleteChildren() {
    for (uint8_t i = 0; i < 4; i++) {
        DeleteChild(i);
    }
}

}   // mc::world::chunk