set(BESKAR_EDITOR_SRC
        imgui/imgui_impl_opengl3.h
        imgui/imgui_impl_opengl3.cpp
        imgui/imgui_impl_opengl3_loader.h
        imgui/imgui_impl_glfw.h
        imgui/imgui_impl_glfw.cpp
        imgui/imgui.h
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui_demo.cpp
)

add_library(beskar_editor STATIC)
target_sources(beskar_editor PRIVATE ${BESKAR_EDITOR_SRC}) # no need to add headers here, only sources are required

target_include_directories(beskar_editor
        PRIVATE
        .. # where the library itself will look for its internal headers
        PUBLIC
        $<BUILD_INTERFACE:${BESKAR_ENGINE_ROOT_DIR}/include> # where top-level project will look for the library's public headers
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> # where external projects will look for the library's public headers
)

target_link_libraries(beskar_editor PUBLIC beskar_engine)