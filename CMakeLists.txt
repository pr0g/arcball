cmake_minimum_required(VERSION 3.15)

project(arcball LANGUAGES C)

include(FetchContent)

FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG f070c83a6059c604cbd098680ddaee391b0a7341)

FetchContent_MakeAvailable(SDL2)

find_package(OpenGL)

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

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL SDL2::SDL2 SDL2::SDL2main)

if(APPLE)
  target_compile_definitions(${PROJECT_NAME} PRIVATE GL_SILENCE_DEPRECATION)
endif()

if (WIN32)
  # copy the SDL2.dll to the same folder as the executable
  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL2::SDL2>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    VERBATIM)
endif ()