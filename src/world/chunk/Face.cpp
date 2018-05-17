/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Face.hpp"

namespace mc::world::chunk {

Face::Face(BlockType type_, Direction dir_, Point2i8 origin_, int8_t size_):
    type { type_ },
    dir { dir_ },
    origin { origin_ },
    size { size_ } {
}

}   // namespace mc::world::chunk
