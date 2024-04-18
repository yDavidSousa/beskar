set(BESKAR_ENGINE_SRC
        texture.cpp
        shader.cpp
        mesh.cpp
        resource_system.cpp
        renderer_system.cpp
        application.cpp
)

add_library(beskar_engine STATIC)
target_sources(beskar_engine PRIVATE ${BESKAR_ENGINE_SRC}) # no need to add headers here, only sources are required

if(DEFINED BESKAR_EDITOR)
    target_compile_definitions(beskar_engine PUBLIC BESKAR_EDITOR)
endif ()

target_include_directories(beskar_engine
    PRIVATE
        .. # where the library itself will look for its internal headers
    PUBLIC
        $<BUILD_INTERFACE:${BESKAR_ENGINE_ROOT_DIR}/include> # where top-level project will look for the library's public headers
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