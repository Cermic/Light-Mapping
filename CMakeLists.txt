cmake_minimum_required(VERSION 3.8)

set(CMAKE_TOOLCHAIN_FILE "C:/Dev-Tools/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(testsdl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE headerFiles "headers/*.h")
file(GLOB_RECURSE sourceFiles "source/*.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers)

find_package(SDL2 CONFIG REQUIRED)

add_executable(testsdl ${headerFiles} ${sourceFiles})

target_link_libraries(testsdl PRIVATE SDL2::SDL2 SDL2::SDL2main)