# NOTE This is a common CMakeLists.txt to be included by each platform's
# NOTE specific CMakeLists.txt.
# This CMakeLists.txt requires the following variables:
# * OMC_DIR
# This CMakeLists.txt provides the following variables outside:
# * OSGCPE_DIR
# * COMPONENT_BINARY_NAME
# * OMC_LINK_LIBS

PROJECT(OMC04_SET_LAYOUT_THEME_SEED)
SET(COMPONENT_BINARY_NAME "omc04-set-layout-theme-seed")
SET(OSGCPE_DIR ${OMC_DIR}/../openscenegraph-cross-platform-examples)

# Use C++11.
ADD_DEFINITIONS("-std=c++11")

# Build OpenSceneGraph statically.
INCLUDE(${OSGCPE_DIR}/libs/OpenSceneGraph/CMakeLists.txt)

# Reference resources.
INCLUDE_DIRECTORIES(${OMC_DIR}/data)

# Provide libraries to link with.
SET(
    OMC_LINK_LIBS
    osgViewer
    osgUtil
    osgDB
    # osgDB plugins start.
    osgdb_osg
    osgdb_serializers_osg
    # osgDB plugins end.
    osgGA
    osg
    OpenThreads
)
