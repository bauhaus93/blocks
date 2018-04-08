/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "CompositeMesh.hpp"

namespace mc::world::chunk {

static std::vector<mesh::Quad> CreateQuads(const std::vector<Block>& blocks);
static std::vector<mesh::Triangle> CreateTriangles(std::vector<mesh::Quad> quads);

std::unique_ptr<mesh::Mesh> CreateCompositeMesh(const std::vector<Block>& blocks) {
    std::vector<mesh::Triangle> triangles = CreateTriangles(CreateQuads(blocks));
    TRACE("Created ", triangles.size(), " triangles for ", blocks.size(), " blocks");
    return std::make_unique<mesh::Mesh>(std::move(triangles));
}

static std::vector<mesh::Quad> CreateQuads(const std::vector<Block>& blocks) {
    std::vector<mesh::Quad> quads;

    for (auto iter = blocks.begin(); iter != blocks.end(); ++iter) {
        const Block& block = *iter;
        const Point3f pos = block.GetPosition() * BLOCK_SIZE;
        
        if (block.IsVisible()) {
            if (!block.HasNeighbour(Direction::NORTH)) {
                static const Point3f normal(0.0f, -1.0f, 0.0f);
                static const std::array<Point3f, 4> vertOffsets { { Point3f(0.0f),
                                                                    Point3f(BLOCK_SIZE, 0.0f, 0.0f),
                                                                    Point3f(BLOCK_SIZE, 0.0f, BLOCK_SIZE),
                                                                    Point3f(0.0f, 0.0f, BLOCK_SIZE) } };
                Point2f uv = 
                mesh::Quad quad;
                for (uint8_t i = 0; i < 4; i++) {
                    quad.SetVertex(i, mesh::Vertex(pos + vertOffsets[i], normal));
                }
                quads.emplace_back(std::move(quad));
            }
            if (!block.HasNeighbour(Direction::EAST)) {
                static const Point3f normal(1.0f, 0.0f, 0.0f);
                mesh::Quad quad;
                quad.SetVertex(0, mesh::Vertex(pos + Point3f(BLOCK_SIZE, 0.0f, 0.0f), normal));
                quad.SetVertex(1, mesh::Vertex(pos + Point3f(BLOCK_SIZE, BLOCK_SIZE, 0.0f), normal));
                quad.SetVertex(2, mesh::Vertex(pos + Point3f(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE), normal));
                quad.SetVertex(3, mesh::Vertex(pos + Point3f(BLOCK_SIZE, 0.0f, BLOCK_SIZE), normal));
                quads.emplace_back(std::move(quad));
            }
            if (!block.HasNeighbour(Direction::SOUTH)) {
                static const Point3f normal(0.0f, 1.0f, 0.0f);
                mesh::Quad quad;
                quad.SetVertex(0, mesh::Vertex(pos + Point3f(BLOCK_SIZE, BLOCK_SIZE, 0.0f), normal));
                quad.SetVertex(1, mesh::Vertex(pos + Point3f(0.0f, BLOCK_SIZE, 0.0f), normal));
                quad.SetVertex(2, mesh::Vertex(pos + Point3f(0.0f, BLOCK_SIZE, BLOCK_SIZE), normal));
                quad.SetVertex(3, mesh::Vertex(pos + Point3f(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE), normal));
                quads.emplace_back(std::move(quad));
            }
            if (!block.HasNeighbour(Direction::WEST)) {
                static const Point3f normal(-1.0f, 0.0f, 0.0f);
                mesh::Quad quad;
                quad.SetVertex(0, mesh::Vertex(pos + Point3f(0.0f, BLOCK_SIZE, 0.0f), normal));
                quad.SetVertex(1, mesh::Vertex(pos, normal));
                quad.SetVertex(2, mesh::Vertex(pos + Point3f(0.0f, 0.0f, BLOCK_SIZE), normal));
                quad.SetVertex(3, mesh::Vertex(pos + Point3f(0.0f, BLOCK_SIZE, BLOCK_SIZE), normal));
                quads.emplace_back(std::move(quad));
            }
            if (!block.HasNeighbour(Direction::UP)) {
                static const Point3f normal(0.0f, 0.0f, 1.0f);
                mesh::Quad quad;
                quad.SetVertex(0, mesh::Vertex(pos + Point3f(0.0f, 0.0f, BLOCK_SIZE), normal));
                quad.SetVertex(1, mesh::Vertex(pos + Point3f(BLOCK_SIZE, 0.0f, BLOCK_SIZE), normal));
                quad.SetVertex(2, mesh::Vertex(pos + Point3f(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE), normal));
                quad.SetVertex(3, mesh::Vertex(pos + Point3f(0.0f, BLOCK_SIZE, BLOCK_SIZE), normal));
                quads.emplace_back(std::move(quad));
            }
            if (!block.HasNeighbour(Direction::DOWN)) {
                static const Point3f normal(0.0f, 0.0f, -1.0f);
                mesh::Quad quad;
                quad.SetVertex(0, mesh::Vertex(pos + Point3f(0.0f, BLOCK_SIZE, 0.0f), normal));
                quad.SetVertex(1, mesh::Vertex(pos + Point3f(BLOCK_SIZE, BLOCK_SIZE, 0.0f), normal));
                quad.SetVertex(2, mesh::Vertex(pos + Point3f(BLOCK_SIZE, 0.0f, 0.0f), normal));
                quad.SetVertex(3, mesh::Vertex(pos, normal));
                quads.emplace_back(std::move(quad));
            }
        }
    }
    return quads; 
}

static std::vector<mesh::Triangle> CreateTriangles(std::vector<mesh::Quad> quads) {
    std::vector<mesh::Triangle> triangles;

    for (auto& quad: quads) {
        triangles.emplace_back(quad.GetFirstTriangle());
        triangles.emplace_back(quad.GetSecondTriangle());    
    }
    return triangles;
}


}       // namespace mc::world::chunk

