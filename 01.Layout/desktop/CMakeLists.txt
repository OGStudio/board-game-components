CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(OMC_DIR "${CMAKE_SOURCE_DIR}/../../../ogs-mahjong-components")

# Include common CMakeLists.txt.
INCLUDE(${CMAKE_SOURCE_DIR}/../CMakeLists.txt)

# Reference sources.
INCLUDE_DIRECTORIES(src)
# Create executable.
ADD_EXECUTABLE(${COMPONENT_BINARY_NAME} src/main.cpp)

# Link executable with libraries.
TARGET_LINK_LIBRARIES(
    ${COMPONENT_BINARY_NAME}
    ${OMC_LINK_LIBS}
)
