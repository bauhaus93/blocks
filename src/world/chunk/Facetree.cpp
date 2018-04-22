/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "Facetree.hpp"

namespace mc::world::chunk {

Facetree::Facetree(Point2i8 origin_, int8_t size_):
    origin { origin_ },
    size { size_ },
    faceInfo { nullptr },
    children { nullptr, nullptr, nullptr, nullptr } {

}

void Facetree::InsertFace(const FaceInfo& info, Point2i8 faceOrigin, int8_t faceSize) {
    if (faceOrigin == origin && size == faceSize) {
        if (faceInfo != nullptr) {
            SetFaceNone();
        } else {
            SplitInsertFace(info);
        }
    } else {
        uint8_t index = 0;
        for (uint8_t i = 0; i < 2; i++) {
            if (faceOrigin[i] > origin[i] + size / 2) {
                index |= (1 << i);
            }
        }
        if (children[index] == nullptr) {
            CreateChild(index);
            children[index]->InsertFace(info, faceOrigin, faceSize);
        } else if (!children[index]->IsFace()) {
            children[index]->InsertFace(info, faceOrigin, faceSize);
        }
    }
}

void Facetree::SplitInsertFace(const FaceInfo& info) {
    for (uint8_t i = 0; i < 4; i++) {
        if (children[i] == nullptr) {
            if (size > 1) {
                CreateChild(i);
                children[i]->SetFace(info);
            }
        } else {
            if (children[i]->IsFace()) {
                children[i]->SetFaceNone();
            } else if (size > 1) {
                children[i]->SplitInsertFace(info);
            }
        }
    }
}

void Facetree::SetFace(const FaceInfo& info) {
    faceInfo = std::make_unique<FaceInfo>(info);
}

void Facetree::SetFaceNone() {
    if (faceInfo == nullptr) {
        faceInfo = std::make_unique<FaceInfo>(BlockType::NONE, Direction::NORTH);
    } else {
        faceInfo->type = BlockType::NONE;
    }
}

bool Facetree::IsFace() const {
    return faceInfo != nullptr;
}

void Facetree::CreateChild(uint8_t index) {
    assert(children[index] == nullptr);
    Point2i8 childOrigin(origin);
    for (int8_t j = 0; j < 2; j++) {
        if (((index >> j) & 1) != 0) {
            childOrigin[j] += size / 2;
        }
    }
    children[index] = std::make_unique<Facetree>(childOrigin, size / 2);
}

void Facetree::DeleteChild(uint8_t index) {
    children[index] = nullptr;
}

void Facetree::DeleteChildren() {
    for (uint8_t i = 0; i < 4; i++) {
        DeleteChild(i);
    }
}

}   // mc::world::chunk