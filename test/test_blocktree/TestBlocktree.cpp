/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

void TestBlockreeMeshCreation::SetUp() {
    std::array<BlockType, 2> types { { BlockType::MUD, BlockType::GRASS } };
    for (uint8_t i = 0; i < 2; i++) {
        protoblocks.emplace(types[i]);
        for (uint8_t j = 0; j < 6; j++) {
            protoblocks.at(types[i]).AddFace(j, i);
        }
    }
}
