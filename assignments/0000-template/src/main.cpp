#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define WIDTH 800
#define HEIGHT 600

int main(void)
{
    // Instantiate window
    GLFWwindow *window;

    if (!glfwInit())
    {
        std::cout << "Could not initialize GLFW." << std::endl;
        return EXIT_FAILURE;
    }

    // Initialize window
    window = glfwCreateWindow(WIDTH, HEIGHT, "First OpenGL App", NULL, NULL);

    if (!window)
    {
        std::cout << "Could not initialize GLFW window." << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Binds current windows to OpenGL context
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Could not initialize GLEW." << std::endl;
    }
    else
    {
        std::cout << "GLEW OK - OpenGL v" << glGetString(GL_VERSION) << std::endl;
    }

    // Main exec loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Here's where the magic (should) happens

        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
