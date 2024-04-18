cmake_minimum_required (VERSION 3.5)

project (beskar_engine)

set(BESKAR_ENGINE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# DEPENDENCIES DIRECTORIES
set(DEPS_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps")
set(DEPS_INCLUDE_DIR "${DEPS_ROOT_DIR}/include")
set(DEPS_LIBRARIES_DIR "${DEPS_ROOT_DIR}/lib/")

add_subdirectory(src/engine)
add_subdirectory(src/editor)