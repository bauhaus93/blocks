/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Chunk.hpp"

namespace mc::world::chunk {

static void CalculateHeights(HeightArray& height,
                    const Point3i chunkPos,
                    const SimplexNoise& heightNoise);
//bool IsBorderBlock(const Point3i& blockPos);

Chunk::Chunk(const Point3i& chunkPos_):
    chunkPos { chunkPos_ },
    origin { chunkPos * static_cast<float>(Chunk::SIZE * Block::SIZE) },
    model {  CreateTranslationMatrix(origin) * glm::mat4(1.0f) },
    checkedNeighbours { },
    blocks { },
    mesh { nullptr } {
}

Chunk::Chunk(Chunk&& other):
    chunkPos { other.chunkPos },
    origin { other.origin },
    model { std::move(other.model) },
    checkedNeighbours { other.checkedNeighbours },
    blocks { std::move(other.blocks) },
    mesh { std::move(other.mesh) } {
}

Chunk& Chunk::operator=(Chunk&& other) {
    chunkPos = other.chunkPos;
    origin = other.origin;
    model = std::move(other.model);
    checkedNeighbours = other.checkedNeighbours;
    blocks = std::move(other.blocks);
    mesh = std::move(other.mesh);
    return *this;
}

bool Chunk::IsEmpty() const {
    return blocks.size() == 0;
}

bool Chunk::IsFull() const {
    return blocks.size() == Chunk::SIZE * Chunk::SIZE;
}

bool Chunk::BlockExists(const Point3i& blockPos) const {
    return blocks.find(blockPos) != blocks.end();
}

bool Chunk::operator<(const Chunk& rhs) const {
    return GetPosition() < rhs.GetPosition();
}

bool Chunk::operator<(const Point3i& rhsChunkPos) const {
    return GetPosition() < rhsChunkPos;
}

/* measurements:
    70-100 ms (full 4096 chunk): (OLD) Block visibility checked after generation is done (through map accesses), excluding border blocks
    ~60 ms    (full 4096): (CURRENT) Visibility saved in block, calculated during building (partly using raw pointers), includes border blocks, which need additionial checking
*/
void Chunk::Generate(const SimplexNoise& noise, const Texture& texture) {
    sf::Clock clock;

    HeightArray relativeHeight;
    CalculateHeights(relativeHeight, chunkPos, noise);

    int32_t* currHeight = &relativeHeight[0];
    for (auto y = 0; y < Chunk::SIZE; y++) {
        for (auto x = 0; x < Chunk::SIZE; x++) {
            //border column neigbours in other chunks are considered having SIZE height
            //assuming they're full of blocks -> false positives occur (visible blocks classified invisible)
            std::array<int32_t, 4> neighbourHeight = { { Chunk::SIZE,
                                                       Chunk::SIZE,
                                                       Chunk::SIZE,
                                                       Chunk::SIZE
                                                     } };
            if (x > 0) {
                neighbourHeight[0] = *(currHeight - 1);
            }
            if (y > 0) {
                neighbourHeight[1] = *(currHeight - Chunk::SIZE);
            }
            if (x + 1 < Chunk::SIZE) {
                neighbourHeight[2] = *(currHeight + 1);
            }
            if (y + 1 < Chunk::SIZE) {
                neighbourHeight[3] = *(currHeight + Chunk::SIZE);
            }
            GenerateColumn(Point3i(x, y, *currHeight), texture, neighbourHeight);
            currHeight++;
        }
    }
   
    TRACE("Generated chunk ", chunkPos,
          ", time: ", clock.getElapsedTime().asMilliseconds(), "ms",
          ", blocks: ", blocks.size());
}


void Chunk::GenerateColumn(Point3i top, const Texture& texture, std::array<int32_t, 4>& neighbourHeight) {
    Point3i curr(top);
    static Direction neighbours[] = { Direction::WEST,    // -x
                                      Direction::NORTH,   // -y
                                      Direction::EAST,    // +x
                                      Direction::SOUTH }; // +y

    while (curr[2] >= 0) {
        Block block;

        if (curr[2] == top[2] && top[2] != Chunk::SIZE - 1) {
            block.AddNeighbour(Direction::DOWN);
        } else {
            block.AddNeighbour(Direction::UP);
            block.AddNeighbour(Direction::DOWN);
        }
        for (auto i = 0; i < 4; i++) {
            if (curr[2] <= neighbourHeight[i]) {
                block.AddNeighbour(neighbours[i]);
            }
        }
        blocks.emplace(curr, std::move(block));
        curr[2] = curr[2] - 1;
    }
}


//TODO finish, when chunk meshing is working
void Chunk::UpdateBlockVisibility(Direction dir, Chunk& neighbour) {
    /*
    if (!IsEmpty()) {
        Direction opp = GetOpposite(dir);

        assert(!curr.CheckedNeighbours(dir));
        assert(!neighbour.CheckedNeighbours(opp));
        checkedNeighbours.Add(dir);
        neighbour.checkedNeighbours.Add(opp);

        switch (dir) {
            case Direction::NORTH:
                for (auto i = 0; i < Chunk::SIZE; i++) {
                    for (auto j = 0; j < Chunk::SIZE; j++) {
                        Point3i localBlockPos(i, 0, j);
                        Point3i neigbourBlockPos(i, Chunk:SIZE - 1, j);
                        auto foundLocal = blocks.find(localBlockPos);
                        auto foundNeighbour = neighbour.blocks.find(neighbourBlockPos);
                        if (foundLocal != blocks.end() &&
                            foundNeighbour == blocks.end()) {
                            foundLocal->second.RemoveNeighbour(dir);
                        } else if (foundLocal == blocks.end() &&
                            foundNeighbour != blocks.end()) {
                            foundNeighbour->second.RemoveNeighbour(opp);
                        }
                        
                    }
                }
        }
    } 
    */
}

void Chunk::CreateMesh() {
    mesh = CreateCompositeMesh(blocks);
}

void Chunk::Draw(const Camera& camera) const {
    if (mesh != nullptr) {
        camera.LoadMVPMatrix(model);
        mesh->Draw();
    }
}

static void CalculateHeights(HeightArray& height,
                      const Point3i chunkPos,
                      const SimplexNoise& heightNoise) {

    constexpr double MIN_HEIGHT = 1.0;
    constexpr double HEIGHT_VARIATION = 10.0;
    int32_t chunkMinHeight = chunkPos[2] * Chunk::SIZE;    //incl
    int32_t chunkMaxHeight = chunkMinHeight + Chunk::SIZE; //excl
    int32_t* currHeight = &height[0];

    for (auto y = 0; y < Chunk::SIZE; y++) {
        for (auto x = 0; x < Chunk::SIZE; x++) {
            double normalizedNoise = (1.0 + heightNoise.GetOctavedNoise(
                chunkPos[0] * Chunk::SIZE + x,
                chunkPos[1] * Chunk::SIZE + y,
                6, 0.1, 0.025)) / 2.0;
            //height given in global block granularity
            *currHeight = static_cast<int32_t>(MIN_HEIGHT + HEIGHT_VARIATION * normalizedNoise);

            if (*currHeight >= chunkMinHeight && *currHeight < chunkMaxHeight) {
                *currHeight -= chunkMinHeight;       // part air/underground col
            } else if (*currHeight >= chunkMaxHeight) {
                *currHeight = Chunk::SIZE - 1;      // full underground col
            } else {
                *currHeight = -1;                     // full air col
            }
            currHeight++;
        }
    }
}

}       // namespace mc::world::chunk
