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

void Blocktree::InsertBlocks(std::vector<BlockElement> blocks) {
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

mesh::Mesh Blocktree::CreateMesh(const BlockManager& blockManager) const {
    std::vector<mesh::Quad> quads;
    constexpr std::array<Direction, 3> faceDirs = { { Direction::EAST,
                                                      Direction::NORTH,
                                                      Direction::UP } };

    for (uint8_t axis = 0; axis < 3; axis++) {
        for (uint8_t layer = 0; layer <= CHUNK_SIZE; layer++) {
            if (layer == 0 || layer == 16) continue;
            std::vector<Face> faces;
            if (layer > 0) {
                CollectFaces(faces, layer - 1, faceDirs[axis]);
            }
            if (layer < 16) {
                CollectFaces(faces, layer, GetOpposite(faceDirs[axis]));
            }

            std::sort(faces.begin(),
                      faces.end(),
                      [](const Face& a, const Face& b) {
                          return a.size > b.size;
                      });
            Facetree tree(Point2i8(0), CHUNK_SIZE);
            tree.InsertFaces(std::move(faces));
            tree.CreateQuads(blockManager, axis, layer, quads);
         }
    }
    return mesh::Mesh(std::move(quads));
}

void Blocktree::CollectFaces(std::vector<Face>& faces, uint8_t index, Direction dir) const {
    if (type != BlockType::NONE) {
        Point2i8 faceOrigin(0);
        switch (dir) {
            case Direction::EAST:
            case Direction::WEST:
                faceOrigin[0] = origin[1];
                faceOrigin[1] = origin[2];
                break;
            case Direction::NORTH:
            case Direction::SOUTH:
                faceOrigin[0] = origin[0];
                faceOrigin[1] = origin[2];
                break;
            case Direction::UP:
            case Direction::DOWN:
                faceOrigin[0] = origin[0];
                faceOrigin[1] = origin[1];
                break;
            default:    assert(0);
        }
        faces.emplace_back(type, dir, faceOrigin, size);
    } else {
        uint8_t axis = 0;
        switch (dir) {
            case Direction::EAST:
            case Direction::WEST:
                axis = 0;
                break;
            case Direction::NORTH:
            case Direction::SOUTH:
                axis = 1;
                break;
            case Direction::UP:
            case Direction::DOWN:
                axis = 2;
                break;
            default:    assert(0);
        }
        for (uint8_t i = 0; i < 8; i++) {
            if (children[i] != nullptr) {
                if (index >= children[i]->origin[axis] &&
                    index < children[i]->origin[axis] + children[i]->size) {
                        children[i]->CollectFaces(faces, index, dir);
                }
            }
        }
    }
}

void Blocktree::CollectBorderFaces(std::vector<Face>& faces, Direction border) {
    switch(border) {
    case Direction::NORTH:  CollectFaces(faces, 0, border);                 break;
    case Direction::EAST:   CollectFaces(faces, CHUNK_SIZE - 1, border);    break;
    case Direction::SOUTH:  CollectFaces(faces, CHUNK_SIZE - 1, border);    break;
    case Direction::WEST:   CollectFaces(faces, 0, border);                 break;
    case Direction::UP:     CollectFaces(faces, CHUNK_SIZE - 1, border);    break;
    case Direction::DOWN:   CollectFaces(faces, 0, border);                 break;
    default: assert(0);
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
