add_sources(
  Main.cpp
  Application.hpp
  Application.cpp
  ApplicationError.hpp
  ApplicationError.cpp
  FileNotFoundError.hpp
  FileNotFoundError.cpp
)

add_subdirectory(logger)
add_subdirectory(utility)
add_subdirectory(graphics)
add_subdirectory(world)
add_subdirectory(states)
