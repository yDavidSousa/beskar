#if BESKAR_EDITOR
#include "beskar_editor/main.h"
#include "beskar_editor/resource_management.h"

#include <editor/imgui/imgui_impl_glfw.h>
#include <editor/imgui/imgui_impl_opengl3.h>
#endif

#include "beskar_engine/application.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

GLFWwindow* window;
unsigned int fbo; // frame buffer object
unsigned int rbo; // rendering buffer object

application::application(const char* path, int width, int height) : width(width), height(height)
{
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

#if BESKAR_EDITOR
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
#endif
}

application::~application()
{
#if BESKAR_EDITOR
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
#endif

    glfwTerminate();
}

void application::run()
{
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

#if BESKAR_EDITOR
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
#endif
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

#if BESKAR_EDITOR
        ImGui::NewFrame();

        static bool show_resource_system_panel = false;
        if(show_resource_system_panel)
        {
            static resource_management p_resource_system(resource_system);
            p_resource_system.draw("Resource Management", &show_resource_system_panel);
        }

        if(ImGui::BeginMainMenuBar())
        {
            if(ImGui::BeginMenu("Window"))
            {
                ImGui::MenuItem("Resource Management", NULL, &show_resource_system_panel);
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        application::gui();
        ImGui::Render();
#endif

        application::update(delta_time);

#if BESKAR_EDITOR
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

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

int main(int argc, char** argv)
{
#if BESKAR_EDITOR
    write_cache(argv[0]);
#endif

    application* app = beskar_main(argc, argv);

    app->resource_system = new resource_system(argv[0]);
    app->renderer_system = new renderer_system();
    app->run();

    delete app->renderer_system;
    delete app->resource_system;
    delete app;
}