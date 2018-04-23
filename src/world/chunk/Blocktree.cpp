/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Blocktree.hpp"

namespace mc::world::chunk {



Blocktree::Blocktree(Point3i8 origin_, int8_t size_):
    origin { origin_ },
    size { size_ },
    type { BlockType::NONE },
    children { { nullptr, nullptr, nullptr, nullptr,
                 nullptr, nullptr, nullptr, nullptr } } {
}

void Blocktree::InsertBlocks(std::vector<BlockElement> blocks) {
    //INFO("Inserting blocks @ ", origin, ", size = ", static_cast<uint32_t>(size), ", count = ", blocks.size());

    if (size == 1) {
        assert(blocks.size() == 1);
        type = blocks[0].second;
    } else {
        auto childBlocks = SplitToChildren(blocks);
        for (uint8_t i = 0; i < 8; i++) {
            if (childBlocks[i].size() > 0) {
                if (children[i] == nullptr) {
                    CreateChildren(i);
                }
                children[i]->InsertBlocks(std::move(childBlocks[i]));
            }
        }
        type = IsMergeable();
        if (type != BlockType::NONE) {
            //INFO("Tree ", origin, ", size = ", static_cast<int32_t>(size), " will be merged!");
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

void Blocktree::CreateChildren(uint8_t index) {
    assert(children[index] == nullptr);
    Point3i8 childOrigin(origin);
    for (int8_t j = 0; j < 3; j++) {
        if (((index >> j) & 1) != 0) {
            childOrigin[j] += size / 2;
        }
    }
    children[index] = std::make_unique<Blocktree>(childOrigin, size / 2);
}

void Blocktree::ClearChildren() {
    for (uint8_t i = 0; i < 8; i++) {
        if (children[i] != nullptr) {
            children[i] = nullptr;
        }
    }
}

mesh::Mesh Blocktree::CreateMesh(const std::map<BlockType, ProtoBlock>& protoblocks) const {
    std::vector<mesh::Quad> quads;

    for (uint8_t axis = 0; axis < 3; axis++) {
        for (uint8_t layer = 0; layer < CHUNK_SIZE; layer++) {
            std::vector<Face> faces;
            switch (axis) {
                case 0:     CollectFaces(faces, layer, Direction::EAST);
                            CollectFaces(faces, layer, Direction::WEST);
                            break;
                case 1:     CollectFaces(faces, layer, Direction::NORTH);
                            CollectFaces(faces, layer, Direction::SOUTH);
                            break;
                case 2:     CollectFaces(faces, layer, Direction::UP);
                            CollectFaces(faces, layer, Direction::DOWN);
                            break;
                default:    assert(0);
            }
            std::sort(faces.begin(),
                      faces.end(),
                      [](const Face& a, const Face& b) {
                          return a.size > b.size;
                      });

            Facetree tree(Point2i8(0), CHUNK_SIZE);
            tree.InsertFaces(std::move(faces));
            tree.CreateQuads(protoblocks, axis, layer, quads);
            if (quads.size() > 0) {
                INFO("Axis = ", static_cast<int>(axis), ", layer = ", static_cast<int>(layer), ", quadcount: ", quads.size());
            }
         }
    }

    return mesh::Mesh(std::move(quads));
}

void Blocktree::CollectFaces(std::vector<Face>& faces, uint8_t layer, Direction dir) const {
    if (type != BlockType::NONE) {
        Point2i8 faceOrigin;
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
                if (layer >= children[i]->origin[axis] &&
                    layer <= children[i]->origin[axis] + children[i]->size) {
                        children[i]->CollectFaces(faces, layer, dir);
                }
            }
        }
    }
}

std::array<std::vector<BlockElement>, 8> Blocktree::SplitToChildren(const std::vector<BlockElement>& blocks) {
    std::array<std::vector<BlockElement>, 8> queue;

    for (auto& element: blocks) {
        const Point3i8& pos = element.first;
        uint8_t index = 0;
        for (uint8_t i = 0; i < 3; i++) {
            if (pos[i] >= origin[i] + size / 2) {
                index |= (1 << i);
            }
        }

        queue[index].emplace_back(element);
    }

    return queue;
}


}   // namespace mc::world::chunk