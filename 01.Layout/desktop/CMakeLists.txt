PROJECT(MC01_LAYOUT)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(OMC_DIR "${CMAKE_SOURCE_DIR}/../../../osg-mahjong-components")
SET(OSGCPE_DIR "${OMC_DIR}/../openscenegraph-cross-platform-examples")

# Build OpenSceneGraph statically.
INCLUDE(${OSGCPE_DIR}/libs/OpenSceneGraph/CMakeLists.txt)

# Use C++11.
ADD_DEFINITIONS("-std=c++11")

# Reference source headers.
INCLUDE_DIRECTORIES(src)
# Reference resource headers.
INCLUDE_DIRECTORIES(../../data)
# Create executable.
ADD_EXECUTABLE(mc01-layout src/main.cpp)
# Link executable with libraries.
TARGET_LINK_LIBRARIES(
    mc01-layout
    osgViewer
    osgUtil
    osgDB
    osgGA
    osg
    OpenThreads
)
