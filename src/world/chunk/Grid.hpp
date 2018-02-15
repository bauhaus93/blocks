/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cassert>

#include "ApplicationError.hpp"
#include "utility/Point2.hpp"
#include "utility/Point3.hpp"

#include "Direction.hpp"
#include "SimplexNoise.hpp"
#include "Element.hpp"

namespace mc::world::chunk {

typedef std::map<Point2i, Element*> ElementMap;

class Grid {

 public:
                    Grid(int32_t gridSize,
                         Point2i chunkSize_,
                         Point3f blockSize_);

    void            SetCenter(Point3f worldPos);
    void            MoveCenter(Direction direction);
    void            Draw(const Camera& camera) const;

 private:
     void           SetCenter(Point2i centerPos);
     Element*       GenerateElement(Point2i pos);

     int32_t        size;
     Point2i        chunkSize;
     Point3f        blockSize;
     SimplexNoise   heightNoise;
     Element*       center;

     Mesh        	mesh;
     Texture     	texture;

};

}   // namespace mc::world::chunk
