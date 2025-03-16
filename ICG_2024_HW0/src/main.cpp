#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <bits/stdc++.h>

#include "./header/Shader.h"
#include "./header/Object.h"

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void drawModel(const string &name, const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, int r, int g, int b);
void init();

// settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

// model position
glm::vec3 modelPosition = glm::vec3(0.0f, 0.0f, 0.0f);

// Shader
Shader *shader;

// Objects to display
Object *cube;

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ICG_2024_HW1", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSwapInterval(1);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // TODO: Enable depth test, face culling
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    // Display loop
    glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_HEIGHT);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Initialize Object and Shader
    init();

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // render
        glClearColor(153 / 255.0, 204 / 255.0, 255 / 255.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader
        shader->use();

        // TODO: Create model, view, and perspective matrix
        glm::mat4 view = glm::lookAt(glm::vec3(0, 10, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

        glm::mat4 Model = glm::mat4(1.0f);
        Model = glm::translate(Model, modelPosition);

        glm::mat4 body = Model;

        // scale body
        body = glm::scale(body, glm::vec3(8, 8, 8));

        // draw body
        drawModel("Cube", body, view, projection, 255, 0, 0); // color:red

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if (key == GLFW_KEY_W)
        {
            // move up
            modelPosition.y += 1.0f;
        }
        else if (key == GLFW_KEY_S && modelPosition.y >= 5)
        {
            // move down
            modelPosition.y -= 1.0f;
        }
        else if (key == GLFW_KEY_D)
        {
            // move right
            modelPosition.x += 1.0f;
        }
        else if (key == GLFW_KEY_A)
        {
            // move left
            modelPosition.x -= 1.0f;
        }
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

float hairLength = 0.8;
int hairCount = 5;
void drawModel(const string &name,
               const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection,
               int r, int g, int b)
{

    shader->set_uniform("projection", projection);
    shader->set_uniform("view", view);
    shader->set_uniform("model", model);
    shader->set_uniform("objectColor", glm::vec3(r / 255.0, g / 255.0, b / 255.0));
    shader->set_uniform("hairLength", hairLength);
    shader->set_uniform("hairCount", hairCount);

    if (name == "Cube")
    {
        cube->draw();
    }
}

void init()
{
#if defined(__linux__) || defined(__APPLE__)
    string dirShader = "../../src/shaders/";
    string dirAsset = "../../src/asset/";
#else
    string dirShader = "..\\..\\src\\shaders\\";
    string dirAsset = "..\\..\\src\\asset\\";
#endif

    string v = dirShader + "elmo.vert";
    string g = dirShader + "elmo.geom";
    string f = dirShader + "elmo.frag";
    shader = new Shader(v.c_str(), f.c_str(), g.c_str());
    cube = new Object(dirAsset + "cube.obj");
}