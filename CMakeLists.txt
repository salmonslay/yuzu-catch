#
# Use the Makefile instead of this script.
# This is not maintained and only used for CLion to find the source files.
#

cmake_minimum_required(VERSION 3.22)
project(yuzu_catch)

set(CMAKE_CXX_STANDARD 17)

set(SDL2_INCLUDE_DIR C:/msys64/mingw64/include)
set(SDL2_LIB_DIR C:/msys64/mingw64/lib)

set(FRUITWORK_INCLUDE_DIR ../fruitwork/include)
set(FRUITWORK_SRC_DIR ../fruitwork/src)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIR}/SDL2 ${PROJECT_SOURCE_DIR}/include ${FRUITWORK_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

# this is very bad practice and not recommended, but since this
# script is not used for anything besides indexing, it's fine
# https://stackoverflow.com/a/18538444
file(GLOB ALL_SOURCES
        "include/*.h"
        "src/*.cpp"

        # fruitworks files
        "../fruitworks/include/*.h"
        "../fruitworks/src/*.cpp"
        )

add_executable(yuzu_catch ${ALL_SOURCES})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} SDL2main.lib SDL2.lib SDL2_image.lib SDL2_ttf.lib SDL2_mixer.lib)