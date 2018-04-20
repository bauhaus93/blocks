/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "CompositeMesh.hpp"

namespace mc::world::chunk {

/*
static std::vector<mesh::Quad> CreateQuads(const Blocktree& blocktree) {
    std::vector<mesh::Quad> quads;
    quads.reserve(blocks.size() * 6);
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

    for (auto iter = blocks.begin(); iter != blocks.end(); ++iter) {
        const Block& block = *iter;
        const Point3f pos = block.GetPosition() * BLOCK_SIZE;
        
        if (block.IsVisible()) {
            auto protoBlock = block.GetPrototype();
            for (uint8_t i = 0; i < 6; i++) {
                Direction dir = GetDirection(i);
                uint32_t layer = protoBlock.GetFace(dir);
                if (!block.HasNeighbour(dir)) {
                    mesh::Quad quad;
                    for (uint8_t j = 0; j < 4; j++) {
                        Point3f uv(vertexUV[j][0], vertexUV[j][1], static_cast<float>(layer));
                        quad.SetVertex(j, mesh::Vertex(pos + vertexOffset[i][j], uv, vertexNormal[i]));
                    }
                    //INFO(quad);
                    quads.emplace_back(std::move(quad));
                }
            }
        }
    }
    return quads; 
}
*/
}       // namespace mc::world::chunk

