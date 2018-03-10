add_sources(
  World.hpp
  World.cpp
  Entity.hpp
  Entity.cpp
  Camera.hpp
  Camera.cpp
  Frustum.hpp
  Frustum.cpp
  BoundingBox.hpp
  BoundingBox.cpp
  Intersection.hpp
  Block.hpp
  Block.cpp
)

add_subdirectory(chunk)
add_subdirectory(octree)
