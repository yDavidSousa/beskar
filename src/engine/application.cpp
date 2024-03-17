#include "editor/imgui/imgui_impl_glfw.h"
#include "editor/imgui/imgui_impl_opengl3.h"
#include "beskar_engine/application.h"
#include "beskar_engine/resource_manager.h"

#include <iostream>
#include <filesystem>
#include <fstream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

application::application(const char* path)
{
    std::filesystem::path current_path = path;
    while (!current_path.empty())
    {
        std::filesystem::path target_path = current_path / "data";
        if(std::filesystem::exists(target_path) && std::filesystem::is_directory(target_path))
        {
            current_path = target_path;
            break;
        }

        current_path = current_path.parent_path();
    }

    data_path = current_path;

    resource_manager rm;
    rm.data_path = data_path;
    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{rm.data_path})
    {
        //std::cout << dir_entry << '\n';
        if (dir_entry.is_directory())
        {
            //std::cout << "is directory" << '\n';
            continue;
        }

        std::filesystem::path asset_path = dir_entry.path();
        auto extension = asset_path.extension().string();
        if(extension == ".meta")
        {
            //std::cout << "is meta file" << '\n';
            continue;
        }

//        if (std::filesystem::exists(asset_path + ".meta"))
//        {
//            std::cout << "already has a meta file" << '\n';
//            continue;
//        }

        auto filename = asset_path.stem().string();
        auto filepath = asset_path.string();

        asset_path += ".meta";
        std::ofstream metadata (asset_path);
        metadata << "metaversion: " << 0 << "\n";
        std::filesystem::file_time_type filetime = std::filesystem::last_write_time(asset_path);
        long long int lastwritetime = filetime.time_since_epoch().count();
        metadata << "timecreated: " << lastwritetime << "\n";
        metadata << "name: " << filename << "\n";
        metadata << "file: " << filepath << "\n";
        metadata.close();
    }
}

application::~application()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

void application::run(int width, int height)
{
    GLFWwindow* window;

    if (glfwInit() == false)
    {
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(width, height, "Pacman", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    application::initialize();

    float delta_time = 0.0f, last_frame = 0.0f;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    while (glfwWindowShouldClose(window) == false)
    {
        glfwPollEvents();
        float currentFrame = static_cast<float>(glfwGetTime());
        delta_time = currentFrame - last_frame;
        last_frame = currentFrame;

        process_input(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        application::gui();
        ImGui::Render();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        application::update(delta_time);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

//    float cameraSpeed = static_cast<float>(212.5 * delta_time);
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//        camera_position += cameraSpeed * glm::vec3(0.0f, -1.0f, 0.0f);
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        camera_position -= cameraSpeed * glm::vec3(0.0f, -1.0f, 0.0f);
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        camera_position -= cameraSpeed * glm::vec3(-1.0f, 0.0f, 0.0f);
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        camera_position += cameraSpeed * glm::vec3(-1.0f, 0.0f, 0.0f);
}