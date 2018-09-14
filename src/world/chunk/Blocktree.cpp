/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Blocktree.hpp"

namespace mc::world::chunk {

Blocktree::Blocktree():
    origin { 0, 0, 0 },
    size { CHUNK_SIZE },
    type { BlockType::NONE },
    children { { nullptr, nullptr, nullptr, nullptr,
                 nullptr, nullptr, nullptr, nullptr } } {
}

Blocktree::Blocktree(Point3i8 origin_, int8_t size_):
    origin { origin_ },
    size { size_ },
    type { BlockType::NONE },
    children { { nullptr, nullptr, nullptr, nullptr,
                 nullptr, nullptr, nullptr, nullptr } } {
}

bool Blocktree::HasChild(uint8_t index) const {
    assert(index < 8);
    return children[index] != nullptr;
}

const Blocktree& Blocktree::GetChild(uint8_t octant) const {
  assert(HasChild(octant));
  return *children[octant];
}

uint32_t Blocktree::MaxDepth() const {
    uint32_t max = 0;
    for (auto& child: children) {
        if (child != nullptr) {
            max = std::max(max, 1 + child->MaxDepth());
        }
    }
    return max;
}

BlockType Blocktree::GetBlockType(Point3i8 pos) const {
    uint8_t oct = GetOctant(pos);
    if (children[oct] == nullptr) {
        return type;
    } else {
        return children[oct]->GetBlockType(pos);
    }
}

bool Blocktree::IsLeaf() const {
    for (auto& child: children) {
        if (child != nullptr) {
            return false;
        }
    }
    return true;
}

bool Blocktree::IsEmpty() const {
    return type == BlockType::NONE && IsLeaf();
}

uint8_t Blocktree::GetOctant(Point3i8 pos) const {
    uint8_t index = 0;
    for (uint8_t i = 0; i < 3; i++) {
        if (pos[i] >= origin[i] + size / 2) {
            index |= (1 << i);
        }
    }
    assert(index < 8);
    return index;
}

void Blocktree::InsertBlocks(const std::vector<BlockElement>& blocks) {
    if (size == 1) {
        assert(blocks.size() == 1);
        type = blocks[0].second;
    } else {
        auto childBlocks = SplitToChildren(blocks);
        for (uint8_t i = 0; i < 8; i++) {
            if (childBlocks[i].size() > 0) {
                if (children[i] == nullptr) {
                    CreateChild(i);
                }
                children[i]->InsertBlocks(std::move(childBlocks[i]));
            }
        }
        type = IsMergeable();
        if (type != BlockType::NONE) {
            ClearChildren();
        }
    }
}

BlockType Blocktree::IsMergeable() const {
    bool foundOne = false;
    BlockType mergeType = BlockType::NONE;
    for (uint8_t i = 0; i < 8; i++) {
        if (children[i] == nullptr) {
            return BlockType::NONE;
        }
        BlockType curr = children[i]->GetBlockType();
        if (!foundOne) {
            if (curr != BlockType::NONE) {
                foundOne = true;
                mergeType = curr;
            } else {
                return BlockType::NONE;
            }
        } else if (curr != mergeType) {
            return BlockType::NONE;
        }
    }
    return mergeType;
}

void Blocktree::CreateChild(uint8_t octant) {
    assert(children[octant] == nullptr);
    Point3i8 childOrigin(origin);
    for (int8_t j = 0; j < 3; j++) {
        if (((octant >> j) & 1) != 0) {
            childOrigin[j] += size / 2;
        }
    }
    children[octant] = std::make_unique<Blocktree>(childOrigin, size / 2);
}

void Blocktree::ClearChildren() {
    for (uint8_t i = 0; i < 8; i++) {
        if (children[i] != nullptr) {
            children[i] = nullptr;
        }
    }
}

LayerFaces Blocktree::CreateFaces() const {
    LayerFaces faces;

    for (uint8_t axis = 0; axis < 3; ++axis) {
        std::map<uint8_t, std::vector<Face>> layer;
        CollectFaces(layer, axis);
        faces[axis] = std::move(layer);
    }
    return faces;
}

std::vector<Face> Blocktree::CreateFaces(uint8_t layer, uint8_t axis) const {
    std::vector<Face> faces;
    CollectFaces(faces, layer, axis);
    return faces;
}

void Blocktree::CollectFaces(std::map<uint8_t, std::vector<Face>>& layerMap, uint8_t axis) const {
    assert(axis < 3);
    if (type != BlockType::NONE) {
        Point2i8 faceOrigin(0);
        Direction faceDir;
        switch (axis) {
        case 0:
                faceOrigin[0] = origin[1];
                faceOrigin[1] = origin[2];
                faceDir = Direction::WEST;
                break;
        case 1:
                faceOrigin[0] = origin[0];
                faceOrigin[1] = origin[2];
                faceDir = Direction::SOUTH;
                break;
        case 2:
                faceOrigin[0] = origin[0];
                faceOrigin[1] = origin[1];
                faceDir = Direction::DOWN;
                break;
            default:    assert(0);
        }
        if (layerMap.find(origin[axis]) == layerMap.end()) {
            layerMap.emplace(origin[axis], std::vector<Face>());
        }
        layerMap.at(origin[axis]).emplace_back(type, faceDir, faceOrigin, size);

        if (layerMap.find(origin[axis] + size) == layerMap.end()) {
            layerMap.emplace(origin[axis] + size, std::vector<Face>());
        }
        layerMap.at(origin[axis] + size).emplace_back(type, GetOpposite(faceDir), faceOrigin, size);
    } else {
        for (uint8_t i = 0; i < 8; i++) {
            if (children[i] != nullptr) {
                children[i]->CollectFaces(layerMap, axis);
            }
        }
    }
}

void Blocktree::CollectFaces(std::vector<Face>& faces, uint8_t layer, uint8_t axis) const {
    assert(layer <= CHUNK_SIZE);
    assert(axis < 3);
    if (type != BlockType::NONE) {
        if (origin[axis] == layer || origin[axis] + size == layer) {
            Point2i8 faceOrigin(0);
            Direction faceDir;
            switch (axis) {
            case 0:
                    faceOrigin[0] = origin[1];
                    faceOrigin[1] = origin[2];
                    if (origin[axis] == layer) {
                        faceDir = Direction::WEST;
                    } else {
                        faceDir = Direction::EAST;
                    }
                    break;
            case 1:
                    faceOrigin[0] = origin[0];
                    faceOrigin[1] = origin[2];
                    if (origin[axis] == layer) {
                        faceDir = Direction::SOUTH;
                    } else {
                        faceDir = Direction::NORTH;
                    }
                    break;
            case 2:
                    faceOrigin[0] = origin[0];
                    faceOrigin[1] = origin[1];
                    if (origin[axis] == layer) {
                        faceDir = Direction::DOWN;
                    } else {
                        faceDir = Direction::UP;
                    }
                    break;
                default:    assert(0);
            }
            faces.emplace_back(type, faceDir, faceOrigin, size);
        }
    } else {
        for (uint8_t i = 0; i < 8; i++) {
            if (children[i] != nullptr) {
                if (children[i]->origin[axis] <= layer &&
                    children[i]->origin[axis] + size >= layer) {
                    children[i]->CollectFaces(faces, layer, axis);
                }
            }
        }
    }
}


std::array<std::vector<BlockElement>, 8> Blocktree::SplitToChildren(const std::vector<BlockElement>& blocks) {
    std::array<std::vector<BlockElement>, 8> queue;

    for (auto& element: blocks) {
        const Point3i8& pos = element.first;
        uint8_t octant = GetOctant(pos);
        queue[octant].emplace_back(element);
    }

    return queue;
}

}   // namespace mc::world::chunk
