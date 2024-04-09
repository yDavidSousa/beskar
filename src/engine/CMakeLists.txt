cmake_minimum_required (VERSION 3.5)

project (beskar_engine)

# DEPENDENCIES DIRECTORIES
set(DEPS_ROOT_DIR "../../deps")
set(DEPS_INCLUDE_DIR "${DEPS_ROOT_DIR}/include")
set(DEPS_LIBRARIES_DIR "${DEPS_ROOT_DIR}/lib/")

set(BESKAR_ENGINE_SRC
        ../editor/imgui/imgui_impl_opengl3.h
        ../editor/imgui/imgui_impl_opengl3.cpp
        ../editor/imgui/imgui_impl_opengl3_loader.h
        ../editor/imgui/imgui_impl_glfw.h
        ../editor/imgui/imgui_impl_glfw.cpp
        ../editor/imgui/imgui.h
        ../editor/imgui/imgui.cpp
        ../editor/imgui/imgui_draw.cpp
        ../editor/imgui/imgui_tables.cpp
        ../editor/imgui/imgui_widgets.cpp
        ../editor/imgui/imgui_demo.cpp
        asset/mesh_asset.cpp
        asset/sprite_asset.cpp
        graphic/gl_texture.cpp
        graphic/gl_renderer.cpp
        graphic/gl_shader.cpp
        graphic/gl_mesh.cpp
        application.cpp
)

add_library(beskar_engine STATIC)
target_sources(beskar_engine PRIVATE ${BESKAR_ENGINE_SRC}) # no need to add headers here, only sources are required

target_include_directories(beskar_engine
    PRIVATE
        .. # where the library itself will look for its internal headers
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include> # where top-level project will look for the library's public headers
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> # where external projects will look for the library's public headers
)

target_include_directories(beskar_engine PUBLIC ${DEPS_INCLUDE_DIR})
target_link_directories(beskar_engine PUBLIC ${DEPS_LIBRARIES_DIR})

if(WIN32)
    target_compile_definitions(beskar_engine PRIVATE GLEW_STATIC)
    target_link_libraries(beskar_engine glfw3 glew32s opengl32)
elseif(APPLE)
    target_link_libraries(beskar_engine glfw.3.4 GLEW.2.2.0 "-framework Cocoa"  "-framework OpenGL" "-framework IOKit")
elseif(UNIX)
    message("https://cmake.org/cmake/help/latest/variable/UNIX.html?highlight=unix")
endif()