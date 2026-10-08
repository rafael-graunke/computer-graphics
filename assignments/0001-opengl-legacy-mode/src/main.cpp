#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define WIDTH 800
#define HEIGHT 600

void showMenu(void)
{
    std::cout << "Select an option:" << std::endl;
    std::cout << "0 - Exit" << std::endl;
    std::cout << "1 - Fixed Triangle" << std::endl;
    std::cout << "2 - Purple Triangle" << std::endl;
    std::cout << "3 - Moving Triangle" << std::endl;
    std::cout << "4 - Moving Triangle (draw on GL_FRONT for tearing)" << std::endl;
    std::cout << "5 - Hello Triangle (the classic RGB triangle)" << std::endl;
}

int triangle(GLFWwindow *window, bool hasColor)
{
    while (!glfwWindowShouldClose(window))
    {
        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT);

        // The following code demonstrate a bit of OpenGL Legacy mode.
        // This way of interacting with OpenGL is called Immediate Mode.
        // It works like a state machine, you can start the machine (glBegin),
        // stop the machine (glEnd) and set some configurations along the way.

        // We start the mode and specify the geometry primitive
        glBegin(GL_TRIANGLES);

        if (hasColor)
            glColor3d(1.0f, 0.0f, 1.0f);

        // We follow up by specifying the vertices the primitive will use
        glVertex2d(-0.5f, -0.5f);
        glVertex2d(0, 0.5);
        glVertex2d(0.5f, -0.5f);

        // Then we finish the mode
        glEnd();

        // The code above on it's own does nothing. This is because, by default,
        // the buffer OpenGL drawns on it GL_BACK instead of GL_FRONT.
        // This is by design. Drawing on the GL_FRONT buffer makes the tearing
        // from the ongoing draw apparent. If we swap after the draw is done,
        // we avoid this effect.
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}

int movingTriangle(GLFWwindow *window, bool tearing)
{
    float vOffset = 0.0f;
    float hOffset = 0.50f;
    float vSpeed = 0.05f;
    float hSpeed = 0.05f;

    float triangle_size = 0.20;

    // Main exec loop
    while (!glfwWindowShouldClose(window))
    {
        // This moves the triangle around
        hOffset += hSpeed;
        vOffset += vSpeed;
        if (hOffset > (1.0f - triangle_size / 2) || hOffset < (-1.0f + triangle_size / 2))
            hSpeed = -hSpeed;
        if (vOffset > (1.0f - triangle_size / 2) || vOffset < (-1.0f + triangle_size / 2))
            vSpeed = -vSpeed;

        // This forces the draw to happen on the front buffer, which causes tearing
        if (tearing)
            glDrawBuffer(GL_FRONT);

        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT);

        // The following code demonstrate a bit of OpenGL Legacy mode.
        // This way of interacting with OpenGL is called Immediate Mode.
        // It works like a state machine, you can start the machine (glBegin),
        // stop the machine (glEnd) and set some configurations along the way.

        // We start the mode and specify the geometry primitive
        glBegin(GL_TRIANGLES);

        // We follow up by specifying the vertices the primitive will use
        glVertex2d(-triangle_size / 2 + hOffset, -triangle_size / 2 - vOffset);
        glVertex2d(0 + hOffset, triangle_size / 2 - vOffset);
        glVertex2d(triangle_size / 2 + hOffset, -triangle_size / 2 - vOffset);

        // Then we finish the mode
        glEnd();

        // The code above on it's own does nothing. This is because, by default,
        // the buffer OpenGL drawns on it GL_BACK instead of GL_FRONT.
        // This is by design. Drawing on the GL_FRONT buffer makes the tearing
        // from the ongoing draw apparent. If we swap after the draw is done,
        // we avoid this effect.

        if (!tearing)
            glfwSwapBuffers(window);
        else
            glFlush();

        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}

int helloTriangle(GLFWwindow *window)
{
    while (!glfwWindowShouldClose(window))
    {
        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);

        // We specify the color on EACH vertex
        glColor3d(0.0f, 0.0f, 1.0f);
        glVertex2d(-0.5f, -0.5f);

        glColor3d(1.0f, 0.0f, 0.0f);
        glVertex2d(0, 0.5);

        glColor3d(0.0f, 1.0f, 0.0f);
        glVertex2d(0.5f, -0.5f);

        // Then when we draw, OpenGL we'll handle the color blending
        // between the vertices.
        glEnd();

        // The code above on it's own does nothing. This is because, by default,
        // the buffer OpenGL drawns on it GL_BACK instead of GL_FRONT.
        // This is by design. Drawing on the GL_FRONT buffer makes the tearing
        // from the ongoing draw apparent. If we swap after the draw is done,
        // we avoid this effect.
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}

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
    window = glfwCreateWindow(WIDTH, HEIGHT, "Legacy OpenGL Immediate Mode", NULL, NULL);

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

    // Read input and select what to run
    int x;
    showMenu();
    std::cin >> x;
    while (x != 0)
    {
        switch (x)
        {
        case 0:
            std::cout << "Bye!" << std::endl;
            return EXIT_SUCCESS;
        case 1:
            return triangle(window, false);
        case 2:
            return triangle(window, true);
        case 3:
            return movingTriangle(window, false);
        case 4:
            return movingTriangle(window, true);
        case 5:
            return helloTriangle(window);
        }
    }
}
