cmake_minimum_required(VERSION 3.0)
project(rollercoaster)

# Add module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(GLAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glad")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

configure_file("${SRC_DIR}/rootdir.h.in" "${SRC_DIR}/rootdir.h")

# Add source files
file(GLOB_RECURSE SOURCE_FILES ${SRC_DIR}/*.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES ${SRC_DIR}/*.h)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")

# freeimage
find_package(FreeImage REQUIRED)

# glwf
find_package(glfw3 REQUIRED)

# gml
find_package(glm REQUIRED)

# glad
add_subdirectory(${GLAD_DIR})

# resources
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE ${FREEIMAGE_LIBRARIES} glad glfw)
