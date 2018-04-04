/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "CompositeMesh.hpp"

namespace mc::world::chunk {

static std::vector<mesh::Quad> CreateQuads(const Map3D<Block>& blocks);
static std::vector<mesh::Triangle> CreateTriangles(std::vector<mesh::Quad> quads);

mesh::Mesh CreateCompositeMesh(const Map3D<Block>& blocks) {
    std::vector<mesh::Triangle> triangles = CreateTriangles(CreateQuads(blocks));
    TRACE("Created ", triangles.size(), " triangles for ", blocks.size(), " blocks");
    return mesh::Mesh(std::move(triangles));
}

static std::vector<mesh::Quad> CreateQuads(const Map3D<Block>& blocks) {
    std::vector<mesh::Quad> quads;

    for (auto iter = blocks.begin(); iter != blocks.end(); ++iter) {
        const Point3f pos(iter->first);
        const Block& block = iter->second;
        
        if (block.IsVisible()) {
            if (!block.HasNeighbour(Direction::NORTH)) {
                static const Point3f normal(0.0f, -1.0f, 0.0f);
                mesh::Quad quad;
                quad.SetVertex(0, mesh::Vertex(pos, normal));
                quad.SetVertex(1, mesh::Vertex(pos + Point3f(Block::SIZE, 0.0f, 0.0f), normal));
                quad.SetVertex(2, mesh::Vertex(pos + Point3f(Block::SIZE, 0.0f, Block::SIZE), normal));
                quad.SetVertex(3, mesh::Vertex(pos + Point3f(0.0f, 0.0f, Block::SIZE), normal));
                quads.emplace_back(std::move(quad));
            }
            if (!block.HasNeighbour(Direction::EAST)) {
                static const Point3f normal(1.0f, 0.0f, 0.0f);
                mesh::Quad quad;
                quad.SetVertex(0, mesh::Vertex(pos + Point3f(Block::SIZE, 0.0f, 0.0f), normal));
                quad.SetVertex(1, mesh::Vertex(pos + Point3f(Block::SIZE, Block::SIZE, 0.0f), normal));
                quad.SetVertex(2, mesh::Vertex(pos + Point3f(Block::SIZE, Block::SIZE, Block::SIZE), normal));
                quad.SetVertex(3, mesh::Vertex(pos + Point3f(Block::SIZE, 0.0f, Block::SIZE), normal));
                quads.emplace_back(std::move(quad));
            }
            if (!block.HasNeighbour(Direction::SOUTH)) {
                static const Point3f normal(0.0f, 1.0f, 0.0f);
                mesh::Quad quad;
                quad.SetVertex(0, mesh::Vertex(pos + Point3f(Block::SIZE, Block::SIZE, 0.0f), normal));
                quad.SetVertex(1, mesh::Vertex(pos + Point3f(0.0f, Block::SIZE, 0.0f), normal));
                quad.SetVertex(2, mesh::Vertex(pos + Point3f(0.0f, Block::SIZE, Block::SIZE), normal));
                quad.SetVertex(3, mesh::Vertex(pos + Point3f(Block::SIZE, Block::SIZE, Block::SIZE), normal));
                quads.emplace_back(std::move(quad));
            }
            if (!block.HasNeighbour(Direction::WEST)) {
                static const Point3f normal(-1.0f, 0.0f, 0.0f);
                mesh::Quad quad;
                quad.SetVertex(0, mesh::Vertex(pos + Point3f(0.0f, Block::SIZE, 0.0f), normal));
                quad.SetVertex(1, mesh::Vertex(pos, normal));
                quad.SetVertex(2, mesh::Vertex(pos + Point3f(0.0f, 0.0f, Block::SIZE), normal));
                quad.SetVertex(3, mesh::Vertex(pos + Point3f(0.0f, Block::SIZE, Block::SIZE), normal));
                quads.emplace_back(std::move(quad));
            }
            if (!block.HasNeighbour(Direction::UP)) {
                static const Point3f normal(0.0f, 0.0f, 1.0f);
                mesh::Quad quad;
                quad.SetVertex(0, mesh::Vertex(pos + Point3f(0.0f, 0.0f, Block::SIZE), normal));
                quad.SetVertex(1, mesh::Vertex(pos + Point3f(Block::SIZE, 0.0f, Block::SIZE), normal));
                quad.SetVertex(2, mesh::Vertex(pos + Point3f(Block::SIZE, Block::SIZE, Block::SIZE), normal));
                quad.SetVertex(3, mesh::Vertex(pos + Point3f(0.0f, Block::SIZE, Block::SIZE), normal));
                quads.emplace_back(std::move(quad));
            }
            if (!block.HasNeighbour(Direction::DOWN)) {
                static const Point3f normal(0.0f, 0.0f, -1.0f);
                mesh::Quad quad;
                quad.SetVertex(0, mesh::Vertex(pos + Point3f(0.0f, Block::SIZE, 0.0f), normal));
                quad.SetVertex(1, mesh::Vertex(pos + Point3f(Block::SIZE, Block::SIZE, 0.0f), normal));
                quad.SetVertex(2, mesh::Vertex(pos + Point3f(Block::SIZE, 0.0f, 0.0f), normal));
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

