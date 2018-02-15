/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Element.hpp"
#include "Chunk.hpp"

namespace mc::world::chunk {

Element::Element(Chunk&& chunk_):
    chunk { std::move(chunk_) },
    neighbour { nullptr, nullptr, nullptr, nullptr } {

}

void Element::Link(Element* nb, Direction direction) {
    LinkElement(nb, direction);
    nb->LinkElement(this, GetOpposite(direction));
}

void Element::LinkElement(Element* nb, Direction direction) {
    auto index = GetIndex(direction);
    assert(index < 4);
    if (neighbour[index] != nullptr) {
        throw ApplicationError("LinkExistingDirectionError",
                               __FUNCTION__,
                               "Wanted to overwrite existing neighbour");
    }
    TRACE("Chunk ", chunk.GetPosition(),
          " has new neighbour ", nb->chunk.GetPosition(),
          " @ ", GetString(direction));
    neighbour[index] = nb;
}

Element* Element::GetNeighbour(Direction dir) {
    return neighbour[GetIndex(dir)];
}

}   // namespace mc::world::chunk
