cmake_minimum_required(VERSION 3.15)

project(arcball LANGUAGES C)

include(FetchContent)

FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG f070c83a6059c604cbd098680ddaee391b0a7341)

FetchContent_MakeAvailable(SDL2)

find_package(OpenGL)
find_package(X11)

add_executable(${PROJECT_NAME})
target_sources(
  ${PROJECT_NAME}
  PRIVATE Ball.c
          Ball.h
          BallAux.c
          BallAux.h
          BallMath.c
          BallMath.h
          Body.c
          Body.h
          Demo.c)

target_link_libraries(
  ${PROJECT_NAME} PRIVATE X11::X11 OpenGL::GL OpenGL::GLU OpenGL::GLX m c SDL2::SDL2)