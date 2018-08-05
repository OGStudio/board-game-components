PROJECT(MC02_THEME)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(OMC_DIR "${CMAKE_SOURCE_DIR}/../../../ogs-mahjong-components")
SET(OSGCPE_DIR "${OMC_DIR}/../openscenegraph-cross-platform-examples")

# Build OpenSceneGraph statically.
INCLUDE(${OSGCPE_DIR}/libs/OpenSceneGraph/CMakeLists.txt)

# Use C++11.
ADD_DEFINITIONS("-std=c++11")

# Reference source headers.
INCLUDE_DIRECTORIES(src)
# Reference resource headers.
INCLUDE_DIRECTORIES(${OMC_DIR}/data)
# Create executable.
ADD_EXECUTABLE(mc02-theme src/main.cpp)
# Link executable with libraries.
TARGET_LINK_LIBRARIES(
    mc02-theme
    osgViewer
    osgUtil
    osgDB
    osgGA
    osgdb_osg
    osgdb_serializers_osg
    osgdb_png
    png
    osg
    OpenThreads
)
