#include "beskar_engine/application.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

application::application()
{

}

application::~application()
{

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

    application::initialize();

    float delta_time = 0.0f, last_frame = 0.0f;

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    while (glfwWindowShouldClose(window) == false)
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        delta_time = currentFrame - last_frame;
        last_frame = currentFrame;

        process_input(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        application::update(delta_time);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
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