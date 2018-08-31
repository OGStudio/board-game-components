CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(OMC_DIR "${CMAKE_SOURCE_DIR}/../../../ogs-mahjong-components")

# Include common CMakeLists.txt.
INCLUDE(${CMAKE_SOURCE_DIR}/../CMakeLists.txt)

# Build mongoose statically.
INCLUDE(${OSGCPE_DIR}/libs/mongoose/CMakeLists.txt)

# Reference sources.
INCLUDE_DIRECTORIES(src)
# Create executable.
ADD_EXECUTABLE(${COMPONENT_BINARY_NAME} src/main.cpp)

# Use ImageIO for macOS.
IF (${OSG_PLATFORM} STREQUAL "macos")
    SET(IMAGE_LINK_LIBS "osgdb_imageio")
# Use libpng for Linux and Windows.
ELSE ()
    SET(IMAGE_LINK_LIBS "osgdb_png" "png")
ENDIF ()

# Link executable with libraries.
TARGET_LINK_LIBRARIES(
    ${COMPONENT_BINARY_NAME}
    ${OMC_LINK_LIBS}
    ${IMAGE_LINK_LIBS}
    mongoose
)
