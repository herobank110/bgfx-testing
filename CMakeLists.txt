cmake_minimum_required(VERSION 3.17)
project(bgfx-testing)

Include(FetchContent)

message("Fetching dependencies")
FetchContent_Declare(
  bgfx_cmake
  GIT_REPOSITORY https://github.com/widberg/bgfx.cmake.git)
FetchContent_MakeAvailable(bgfx_cmake)
find_package(SDL2 CONFIG REQUIRED)

add_executable(basic-window)
target_sources(basic-window PRIVATE main.cpp sdl_backend.cpp)
target_compile_features(basic-window PRIVATE cxx_std_20)
target_link_libraries(basic-window PRIVATE bgfx SDL2::SDL2 SDL2::SDL2main)
