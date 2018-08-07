PROJECT(MC03_REMOTE_LAYOUT_THEME)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(OMC_DIR "${CMAKE_SOURCE_DIR}/../../../ogs-mahjong-components")
SET(OSGCPE_DIR "${OMC_DIR}/../openscenegraph-cross-platform-examples")

# Build OpenSceneGraph statically.
INCLUDE(${OSGCPE_DIR}/libs/OpenSceneGraph/CMakeLists.txt)

# Set special build flags:
# * -O3: make compact binary, critical for web
# * -s USE_SDL=2: provide SDL2
# * -s ALLOW_MEMORY_GROWTH=1: allow total memory used to increase when necessary
# * -s USE_LIBPNG=1: use libpng
# * -s USE_ZLIB=1: use zlib, necessary for libpng
# * -s FETCH=1: use FetchAPI, necessary for making HTTP(s) requests
# * -s WASM=0: disable WebAssembly because FetchAPI is unsupported there yet
SET(BUILD_FLAGS "-O3 -s USE_SDL=2 -s ALLOW_MEMORY_GROWTH=1 -s USE_LIBPNG=1 -s USE_ZLIB=1 -s FETCH=1 -s WASM=0")
ADD_DEFINITIONS(${BUILD_FLAGS})

# Use C++11.
ADD_DEFINITIONS("-std=c++11")

# Reference source headers.
INCLUDE_DIRECTORIES(src)
# Reference resource headers.
INCLUDE_DIRECTORIES(${OMC_DIR}/data)
# Create executable (aka HTML page).
ADD_EXECUTABLE(mc03-remote-layout-theme src/main.cpp)
# Link executable with libraries.
TARGET_LINK_LIBRARIES(
    mc03-remote-layout-theme
    osgViewer
    osgUtil
    osgDB
    osgGA
    osgdb_osg
    osgdb_serializers_osg
    osgdb_png
    osg
    OpenThreads
)

# Make Emscripten generate ready-to-open HTML page.
SET(CMAKE_EXECUTABLE_SUFFIX ".html")
# Link with special build flags.
SET_TARGET_PROPERTIES(mc03-remote-layout-theme PROPERTIES LINK_FLAGS ${BUILD_FLAGS})
