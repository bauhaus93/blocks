/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <array>
#include <memory>
#include <vector>
#include <iostream>
#include <set>

#include "logger/GlobalLogger.hpp"
#include "utility/Point3.hpp"
#include "Frustum.hpp"
#include "Intersection.hpp"

namespace mc::world {

template<typename T>
class Octree;

template<typename T>
using OctreePtr = std::unique_ptr<Octree<T>>;

template<typename T>
using OctreeArray = std::array<OctreePtr<T>, 8>;

template<typename T>
using Point3Vec = std::vector<Point3<T>>;

template<typename T>
using Point3Set = std::set<Point3<T>>;

template<typename T>
bool BelowMinSize(const Point3<T>& size, const Point3<T>& minSize);

typedef OctreePtr<int32_t> OctreePtrI;

template<typename T>
class Octree {

    const Point3<T>     MIN_SIZE = Point3<T>(1, 1, 1);

 public:
                        Octree(Point3<T> min_,
                               Point3<T> max_);

    bool                IsLeaf() const;
    const Point3<T>&    GetMin() const;
    const Point3<T>&    GetMax() const;
    const Point3Vec<T>& GetElements() const;
    void                QueueElement(Point3<T> element);
    void                QueueElements(const std::set<Point3i>& newQueuedElements);
    void                InsertQueuedElements();

    Point3Vec<T>        GetPointsInFrustum(const Frustum& frustum) const;

 private:
    void                CreateChildren();
    void                InsertOnTree();
    void                InsertOnSubtree();
    void                GetPointsInFrustum(const Frustum& frustum, Point3Vec<T>& points) const;

    Point3<T>       min;
    Point3<T>       max;
    OctreeArray<T>  children;
    Point3Vec<T>    elements;
    Point3Vec<T>    queuedElements;
};


template<typename T>
Octree<T>::Octree(Point3<T> min_,
                  Point3<T> max_):
    min { min_ },
    max { max_ },
    children { { nullptr, nullptr, nullptr, nullptr,
               nullptr, nullptr, nullptr, nullptr } } {
}

template<typename T>
bool Octree<T>::IsLeaf() const {
    if (children[0] == nullptr) {
        for (int i = 0; i < 8; i++) {
            assert(children[i] == nullptr);
        }
        return true;
    }
    return false;
}

template<typename T>
const Point3<T>& Octree<T>::GetMin() const {
    return min;
}

template<typename T>
const Point3<T>& Octree<T>::GetMax() const {
    return max;
}

template<typename T>
const Point3Vec<T>& Octree<T>::GetElements() const {
    return elements;
}

template<typename T>
Point3Vec<T> Octree<T>::GetPointsInFrustum(const Frustum& frustum) const {
    Point3Vec<T> points;
    GetPointsInFrustum(frustum, points);
    return points;
}

template<typename T>
void Octree<T>::GetPointsInFrustum(const Frustum& frustum, Point3Vec<T>& points) const {
    BoundingBox bb(min, max);

    switch(frustum.Intersects(bb)) {
        case Intersection::INSIDE:
            points.insert(points.end(), elements.begin(), elements.end());
            break;
        case Intersection::PARTIAL:
            //TODO use PARTIAL!, call children
            break;
        case Intersection::OUTSIDE:
            break;
    }
}

template<typename T>
void Octree<T>::QueueElement(Point3<T> element) {
    queuedElements.push_back(element);
}

template<typename T>
void Octree<T>::QueueElements(const std::set<Point3i>& newQueuedElements) {
    queuedElements.insert(queuedElements.end(),
                          newQueuedElements.begin(),
                          newQueuedElements.end());
}

template<typename T>
void Octree<T>::InsertQueuedElements() {
    if (!queuedElements.empty()) {
        if (IsLeaf()) {
            CreateChildren();
        }
        if (!IsLeaf()) {
            InsertOnSubtree();
        } else {
            InsertOnTree();
        }
        queuedElements.clear();
    }
}

template<typename T>
void Octree<T>::InsertOnTree() {
    if (IsLeaf()) {
        for (const auto& e: queuedElements) {
            TRACE("Element ", e, " inserted at leaf node min = ", min, ", max = ", max);
        }
    }
    elements.insert(elements.end(),
                    queuedElements.begin(),
                    queuedElements.end());
}

template<typename T>
void Octree<T>::InsertOnSubtree() {
    TRACE("Insert queued elements on sub trees");
    assert(!IsLeaf());
    for (auto e: queuedElements) {
        for (uint8_t i = 0; i < 8; i++) {
            Octree<T>& child = *children[i];
            if (e.InBoundaries(child.GetMin(), child.GetMax())) {
                child.QueueElement(e);
            }
        }
    }

    for (uint8_t i = 0; i < 8; i++) {
        children[i]->InsertQueuedElements();
    }
    InsertOnTree(); // Every tree also saves each element of it's subtrees
                    // Maybe needs checking if really useful
                    //(requires more space, but needs less tree traversal for checking)
}

template<typename T>
void Octree<T>::CreateChildren() {
    assert(IsLeaf());
    Point3<T> subSize = (max - min) / 2;

    if (BelowMinSize(subSize, MIN_SIZE)) {
        TRACE("Octree MIN_SIZE reached, this tree will be a leaf");
        return;
    }

    for (uint8_t i = 0; i < 8; i++) {
        Point3<T> subMin(min);
        for (uint8_t j = 0; j < 3; j++) {   // for each axis (xyz)
            if (((i >> j) & 1) != 0) {
                subMin[j] += subSize[j];
            }
        }
        Point3<T> subMax = subMin + subSize;
        children[i] = std::make_unique<Octree<T>>(subMin, subMax);
    }
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Octree<T>& tree) {
    os << "Octree min = " << tree.GetMin() << ", max = " << tree.GetMax();
    if (tree.IsLeaf()) {
        os << ", leaf, ";
    } else {
        os << ", branch, ";
    }
    os << "elements:";
    for (const auto& e: tree.GetElements()) {
        os << " " << e;
    }
    return os;
}

template<typename T>
bool BelowMinSize(const Point3<T>& size, const Point3<T>& minSize) {
    for (uint8_t i = 0; i < 3; i++) {
        if (size[i] < minSize[i]) {
            return true;
        }
    }
    return false;
}

}   // namespace mc::world
