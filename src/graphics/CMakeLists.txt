add_sources(
  ShaderProgram.hpp
  ShaderProgram.cpp
  Bitmap.hpp
  Bitmap.cpp
  Texture.hpp
  Texture.cpp
  TextureAtlas.hpp
  TextureAtlas.cpp
  OpenGLError.hpp
  OpenGLError.cpp
  ShaderError.hpp
  ShaderError.cpp
  ShaderProgramError.hpp
  ShaderProgramError.cpp

)

add_subdirectory(shader)
add_subdirectory(glad)
