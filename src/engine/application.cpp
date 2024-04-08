#include "editor/imgui/imgui_impl_glfw.h"
#include "editor/imgui/imgui_impl_opengl3.h"
#include "beskar_engine/application.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

application::application(const char* path)
{
    std::filesystem::path current_path = path;
    while (current_path.empty() == false)
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