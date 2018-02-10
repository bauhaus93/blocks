add_sources(
  Main.cpp
  Application.cpp
  StateStack.cpp
  GameState.cpp
  WorldState.cpp
  PauseState.cpp
  ApplicationError.cpp
  FileNotFoundError.cpp
)

add_subdirectory(logger)
add_subdirectory(utility)
add_subdirectory(graphics)
add_subdirectory(world)
