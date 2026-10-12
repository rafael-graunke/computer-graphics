#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define WIDTH 800
#define HEIGHT 600

static GLuint compileShader(GLenum type, const std::string& source)
{
    GLuint id = glCreateShader(type); // We start of by creating a shader of the specified type
    const char *src = source.c_str(); // This is a pointer to she sequence of chars
    
    // Specify:
    // - Shader ID
    // - Count of elements in the string and length array
    // - String, an array of pointer to strings
    // - Lenght, an array of the string lengths
    glShaderSource(id, 1, &src, nullptr);
    // So this means, the shader ID = id, a single string and
    // the address of src would be the pointer to the pointer, i.e and array of string.
    // According to the specification, if length is not specified, the string are assumed
    // to be null terminated.
    glCompileShader(id);

    // This is just error handling.
    // We get the status of the compilation, gather and print any logs.
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char *) alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " <<
        (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" <<
        std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
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
    window = glfwCreateWindow(WIDTH, HEIGHT, "First Shader App", NULL, NULL);

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

    // Here something new happens. Before we we're using legacy OpenGL,
    // which meant controlling the state through configurations.
    // Now we are able to create a program that the GPU runs, called a shader.
    // Before diving into the setup, it's important to know a bit about
    // how the pipeline works now:
    //
    // *Vertex Shader*:
    // The vertex shader is responsible for drawing the vertices, based on the
    // primitives, like GL_TRIANGLES, same as before. It scales linearly with the
    // vertex count. The more vertice the more executions. It usually used for
    // transformation, commonly used for model, view and projection matrices.
    // The Model Matrix transforms an objects from local space (0, 0, 0)
    // to it's actual the world space. This includes translation, rotation and scaling.
    // The View Matrix determines how we see the world, by transforming the world space.
    // Common parameters are:
    // `eye` - This is the location of the camera
    // `center` - This is point the camera is looking at
    // `up` - It's a vector that determines how the camera is tilted. It'd be the roll in Euler angles.
    // The Projection Matrix takes the camera and projects a prodonized volume, called clip space.
    // There are two main projections, perspective (commonly seen in games)
    // and orthographic (commonly used by CAD softwares).
    //
    // *Tesselation Shader*:
    // Added in OpenGL 4.0, tesselation is a technique for subdividing low-resolution
    // geometric shapes into highly detailed primevites, directly in the GPU. It's commonly
    // used for generating terrain. Having a 1 million triangle mesh and a low-res triangle mesh
    // tesselated up to 1 million triangles might have the same performance result, but there are
    // lots of benefits with tesselation:
    // - Eliminates overhead between CPU and GPU, since the GPU does the expansion to n triangles.
    // - Dynamic LOD. Based on how close a mesh is to the camera, the higher the tesselation factor.
    // - VRAM footprint reduction. Instead of having a heavy 3d model with lots of triangles,
    // have a low-poly model + a height map. This is very interesting and I'll look more into it.
    //
    // *Geometry Shader*:
    // This is used to change how the primitives work. There are many ways to use geometry shaders,
    // for creating grass, fur, but the example that really helped me understand it was
    // for creating particles. While a vertex shader is vertex in, vertex out, geometry shader can
    // create new vertices. Let's say we want a particle. Instead of creating it as n vertices,
    // we can simply specify it's location (single vertex) and then use the geometry shader to
    // increment that particle as we which. For instance, creating a square on that position.
    //
    // *Rasterization*:
    // So far we've been working with geometry. Rasterization is the process to generate fragments
    // based of the geomtry we've specified. A fragment is basically a pixel, sort of. It's a
    // potential pixel. Let's say we have two overlaping triangles. The rasterizer will create
    // fragments for BOTH triangles, even where they overlap each other. At an overlapping
    // position, if one triangle is blue and the other is red, we'll have BOTH color fragments.
    //
    // *Fragment Shader*:
    // It's responsible for setting colors, texture and lighting. It runs once for each fragment
    // previously generated. I'm oversimplifying, but this is one of the most interesting ones.
    //
    // *Pixel Operations*:
    // This is the part were the fragment becomes a pixel. It's mainly responsible for hidding
    // fragments that are behind other. This is called Hidden Surface Removal, and it uses
    // a color buffer and depth buffer (also called z-buffer).

    // Let's define our vertices for a triangle
    float positions[6] = {
        -0.5f,
        -0.5f,
        0.0f,
        0.5f,
        0.5f,
        -0.5f,
    };

    // Then we'll create a reference for the VBO.
    // VBO stands for Vertex BUffer Objects. It's a place in the GPU memory that stores
    // vertex data: positions, colors, texture coordinates...

    GLuint buffer;            // This holds a 4bytes value. We could have used `unsigned int` or `uint32_t`
    glGenBuffers(1, &buffer); // Then we ask OpenGL to create a buffer

    // This tells OpenGL what the buffer will hold. In this case it's an array.
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // Then we load the values into the buffer.
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    // GL_STATIC_DRAW is a usage HINT for the allocation. It determines in which region
    // of the VRAM the data is going to get stored. This is determined by:
    // - Frequency: How frequently the data changes.
    //   STATIC: writes once, uses many times;
    //   DYNAMIC: changed repeatedly and used many times between changes;
    //   STREAM: written once and used a few times, before being replace
    //           (like particles, that change position every frame)

    // - Nature: determines who writes and who reads.
    //   DRAW: CPU writes, GPU reads
    //   READ: GL writes, and app (CPU) reads it
    //   COPY: GL writes and reads, CPU doesn't touch it.

    glVertexAttribPointer(
        0,        // The slot we're going to be referencing in the shader, matches `layout(location = N)`.
        2,        // How many components per vertex, so a 2D vertex is 2, 3D is 3...
        GL_FLOAT, // The type.
        GL_FALSE,
        // The `normalized` is only used for integers. If true, unsigned gets normalized to 0 - 1
        // and signed to -1 to 1;
        0,
        // stride is the distance between one vertex info an another. It would be 2 * sizeof(float),
        // setting it to 0 means "tightly packed" and let's OpenGL figure it out based on size and type.
        0 // This is actually the offset to start from the buffer.
    );
    glEnableVertexAttribArray(0);
    // The zero here is the same number as the slot as before.
    // This basically YOU saying "hey, you can start using this."

    // We still need to write, compile and attach the shaders.
    std::string vert = R"(
#version 320 es

layout(location = 0) in vec4 position;

// Since we have 2d vertices and the position is being declared as vec4,
// the value we'll get is (x, y, 0.0, 1.0). So it works fine, it just
// saves us time when doing the following attribution;

void main()
{
    gl_Position = position;
    // gl_Position is a built-in output variable. Whatever you write on it
    // becomes that vertex final position. It's type is a vec4, so declaring
    // the incomming position as vec4 makes it easier. Otherwise, if
    // we use vec2, we'd have to do this:
    // gl_Position = vec4(position, 0.0, 1.0);
    //
    // Furthermore, the W in (x, y, z, w) is a homogeneous coordinate.
    // It's specially used in matrix multiplication.
}
)";

    std::string frag = R"(
#version 320 es

precision mediump float; // This tells OpenGL to use 16bits for floating point math

layout (location = 0) out vec4 color; // Here we'll be outputing the color variable

void main()
{
    color = vec4(1.0, 1.0, 1.0, 1.0);
    // Set color to white for every fragment
}
)";

    // Now we'll compile and attach the shader. Check compileShader function for more info
    GLuint program = glCreateProgram(); // Create the shader
    GLuint vertShader = compileShader(GL_VERTEX_SHADER, vert);
    GLuint fragShader = compileShader(GL_FRAGMENT_SHADER, frag);

    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    // Finally, tell GL to use the shaders
    glUseProgram(program);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // How this differs from immediate mode is, we tell it "draw what you have in memory".
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
