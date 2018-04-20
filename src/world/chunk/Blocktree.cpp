/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Blocktree.hpp"

namespace mc::world::chunk {

static std::array<std::vector<BlockElement>, 8> SplitToChildren(Point3i8 origin, const std::vector<BlockElement>& blocks);

Blocktree::Blocktree(Point3i8 origin_, int8_t size_):
    origin (origin_ ),
    size { size_ },
    type { BlockType::NONE },
    children { { nullptr, nullptr, nullptr, nullptr,
                 nullptr, nullptr, nullptr, nullptr } } {
}

void Blocktree::InsertBlocks(std::vector<BlockElement> blocks) {
    if (size == 1) {
        assert(blocks.size() == 1);
        type = blocks[0].second;
    } else {
        auto childBlocks = SplitToChildren(origin, blocks);
        for (uint8_t i = 0; i < 8; i++) {
            if (childBlocks[i].size() > 0) {
                AssignChildBlocks(i, std::move(childBlocks[i]));
            }
        }
        type = IsMergeable();
        if (type != BlockType::NONE) {
            //INFO("Tree ", origin, ", size = ", static_cast<int32_t>(size), " will be merged!");
            ClearChildren();
        }
    }
}

void Blocktree::AssignChildBlocks(int8_t index, std::vector<BlockElement> blocks) {
    if (children[index] == nullptr) {
        Point3i8 childOrigin(origin);
        for (int8_t j = 0; j < 3; j++) {
            if (((index >> j) & 1) != 0) {
                childOrigin[j] += size / 2;
            }
        }
        children[index] = std::make_unique<Blocktree>(childOrigin, size / 2);
    }
    children[index]->InsertBlocks(std::move(blocks));
}

BlockType Blocktree::IsMergeable() const {
    BlockType mergeType = BlockType::NONE;
    for (uint8_t i = 0; i < 8; i++) {
        if (children[i] == nullptr) {
            return BlockType::NONE;
        }
        BlockType curr = children[i]->GetBlockType();
        if (mergeType == BlockType::NONE) {
            mergeType = curr;
        } else if (curr != mergeType) {
            return BlockType::NONE;
        }
    }
    return mergeType;
}

void Blocktree::ClearChildren() {
    for (uint8_t i = 0; i < 8; i++) {
        if (children[i] != nullptr) {
            children[i] = nullptr;
        }
    }
}



mesh::Mesh Blocktree::CreateMesh() const {
    std::vector<mesh::Quad> quads;
    std::vector<Face> knownFaces;

    //TODO: handle when root is leaf
    for (uint8_t i = 0; i < 8; i++) {
        if (children[i] != nullptr) {
            children[i]->CollectQuads(*this, knownFaces, quads);
        }
    }
    return mesh::Mesh(std::move(quads));
}

void Blocktree::CollectQuads(const Blocktree& parent, std::vector<Face>& knownFaces, std::vector<mesh::Quad>& quads) const {
    if (type != BlockType::NONE) {  //Leaf
        for (uint8_t i = 0; i < 6; i++) {
            Face f { origin, Point3i(0) };
            switch(GetDirection(i)) {
                case Direction::NORTH:
                case Direction::EAST:
                case Direction::SOUTH:
                case Direction::WEST:
            }
        }

    } else {
        for (uint8_t i = 0; i < 8; i++) {
            if (children[i] != nullptr) {
                children[i]->CollectQuads(*this, knownFaces, quads);
            }
        }
    }
}


static std::array<std::vector<BlockElement>, 8> SplitToChildren(Point3i8 origin, const std::vector<BlockElement>& blocks) {
    std::array<std::vector<BlockElement>, 8> queue;
    for (auto& element: blocks) {
        const Point3i8& pos = element.first;
        uint8_t index = 0;
        for (uint8_t i = 0; i < 3; i++) {
            if (pos[i] != origin[0]) {
                index |= (1 << i);
            }
        }
        queue[index].emplace_back(element);
    }
    return queue;
}


}   // namespace mc::world::chunk