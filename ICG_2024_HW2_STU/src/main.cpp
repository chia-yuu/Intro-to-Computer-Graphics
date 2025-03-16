#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <bits/stdc++.h>

#include "./header/Object.h"
#include "./header/stb_image.h"

#include<cmath>

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
unsigned int createShader(const string &filename, const string &type);
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader);
unsigned int modelVAO(Object &model);
unsigned int loadTexture(const char *tFileName);
void init();
glm::vec3 rgb_to_hsv(glm::vec3 rgb);
glm::vec3 hsv_to_rgb(glm::vec3 hsv);

// settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

// Shader
unsigned int vertexShader, fragmentShader, shaderProgram;

// Texture
unsigned int airplaneTexture, earthTexture;

// VAO, VBO
unsigned int airplaneVAO, earthVAO, cubeVAO;

// Objects to display
Object *airplaneObject, *earthObject, *cubeObject;

// Constants you may need
const int airplaneHeight = 27;
const int rotateEarthSpeed = 30;
const int rotateAirplaneSpeed = 90;
const int squeezeSpeed = 90;
const int rainbowSpeed = 72;

// You can use these parameters
float rotateEarthDegree = 0;
float rotateAirplaneDegree = 0;
int rotateAxisDegree = 180;
bool useSqueeze = false;
float squeezeFactor = 0;
bool useRainbowColor = false;
float rainbowDegree = 0;
glm::vec3 rainbowColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::mat4 rotation_axis;
float hue = 0.0f, h_angle = 0.0f;
bool useHelicopter = false;
glm::vec3 helicopterColor;

int main() {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    /* TODO#0: Change window title to "HW2 - [your student id]"
     *        Ex. HW2 - 312550000
     */

    // glfw window creation
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HW2 - 111550108", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSwapInterval(1);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // TODO#1: Finish function createShader
    // TODO#2: Finish function createProgram
    // TODO#3: Finish function modelVAO
    // TODO#4: Finish function loadTexture
    // You can find the above functions right below the main function

    // Initialize Object, Shader, Texture, VAO, VBO
    init();

    // Enable depth test, face culling
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    // Set viewport
    glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_HEIGHT);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // render loop variables
    double dt;
    double lastTime = glfwGetTime();
    double currentTime;
    double tot_time = 0;

    /* TODO#5: Data connection - Retrieve uniform variable locations
     *    1. Retrieve locations for model, view, and projection matrices.
     *    2. Retrieve locations for squeezeFactor, rainbowColor, and other parameters.
     * Hint:
     *    glGetUniformLocation
     */
    /* vertexShader
     * uniform mat4 model;
     * uniform mat4 view;
     * uniform mat4 projection;
     * uniform float squeezeFactor;
     * 
     * fragmentShader
     * uniform sampler2D ourTexture;
     * uniform vec3 rainbowColor;
     */
    GLuint model_loc = glGetUniformLocation(shaderProgram, "model");
    GLuint view_loc = glGetUniformLocation(shaderProgram, "view");
    GLuint proj_loc = glGetUniformLocation(shaderProgram, "projection");
    GLuint squz_loc = glGetUniformLocation(shaderProgram, "squeezeFactor");

    GLuint texture_loc = glGetUniformLocation(shaderProgram, "ourTexture");
    GLuint rainbow_loc = glGetUniformLocation(shaderProgram, "rainbowColor");
    GLuint use_rainbow_loc = glGetUniformLocation(shaderProgram, "useRainbowColor");
    GLuint helicopter_loc = glGetUniformLocation(shaderProgram, "helicopterColor");
    GLuint is_cube_loc = glGetUniformLocation(shaderProgram, "is_cube");

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // render
        glClearColor(0 / 255.0, 0 / 255.0, 0 / 255.0, 1.0f);
        // glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 50.0f, 90.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

        // glm::mat4 base(1.0f), airplaneModel(1.0f), earthModel(1.0f);
        glm::mat4 base(1.0f), airplaneModel(1.0f), earthModel(1.0f);

        /* TODO#6-1: Render Airplane
         *    1. Set up airplane model matrix.
         *    2. Send model, view, and projection matrices to the program.
         *    3. Send squeezeFactor, rainbowColor, or other parameters to the program.
         *    4. Apply the texture, and render the airplane.
         * Hint:
         *	  rotate, translate, scale
         *    glUniformMatrix4fv, glUniform1f, glUniform3fv
         *    glActiveTexture, glBindTexture, glBindVertexArray, glDrawArrays
         */
        
        // transform
        // float x = 0.0f, y = airplaneHeight * cos(rotateAirplaneDegree), z = airplaneHeight * sin(rotateAirplaneDegree);
        // airplaneModel = glm::scale(airplaneModel, glm::vec3(1, -1, 1));
        rotation_axis = glm::rotate(glm::mat4(1.0f), glm::radians((float)rotateAxisDegree), glm::vec3(0.0f, 1.0f, 0.0f));
        airplaneModel = glm::rotate(airplaneModel, glm::radians(rotateAirplaneDegree), glm::vec3(-1.0f, 0.0f, 0.0f));  // rotate around -x
        airplaneModel = glm::translate(airplaneModel, glm::vec3(0, 27, 0)); // radius 27
        airplaneModel = rotation_axis * airplaneModel;
        // airplaneModel = glm::translate(airplaneModel, glm::vec3(x, y, z));
        // std::cout<<"("<<rotateAirplaneDegree<<", "<<y<<", "<<z<<", "<<(y*y+z*z)<<"), ";
        // std::cout<<"("<<rotateAirplaneDegree<<", "<<y<<"), ";
        if(!useHelicopter){
            // send to shader
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(airplaneModel));
            glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection));
            glUniform1f(squz_loc, 0);
            glUniform1f(use_rainbow_loc, useRainbowColor);
            glUniform3fv(rainbow_loc, 1, glm::value_ptr(rainbowColor));
            glUniform1f(is_cube_loc, 0);

            glBindVertexArray(airplaneVAO);

            // texture
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, airplaneTexture);
            glUniform1i(texture_loc, 0);

            glDrawArrays(GL_TRIANGLES, 0, airplaneObject->positions.size());
            glBindVertexArray(0);
        }
        
        /* TODO#6-2: Render Earth
         *    1. Set up earth model matrix.
         *    2. Send model, view, and projection matrices to the program.
         *    3. Send squeezeFactor, rainbowColor, or other parameters to the program.
         *    4. Apply the texture, and render the earth.
         * Hint:
         *	  rotate, translate, scale
         *    glUniformMatrix4fv, glUniform1f, glUniform3fv
         *    glActiveTexture, glBindTexture, glBindVertexArray, glDrawArrays
         */

        // transform
        earthModel = glm::rotate(earthModel, glm::radians(rotateEarthDegree), glm::vec3(0.0f, 1.0f, 0.0f));
        earthModel = glm::scale(earthModel, glm::vec3(10, 10, 10));

        // send to shader
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(earthModel));
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1f(squz_loc, squeezeFactor);
        glUniform1f(use_rainbow_loc, 0);
        glUniform3fv(rainbow_loc, 1, glm::value_ptr(rainbowColor));
        glUniform1f(is_cube_loc, 0);

        glBindVertexArray(earthVAO);

        // texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, earthTexture);
        glUniform1i(texture_loc, 0);

        glDrawArrays(GL_TRIANGLES, 0, earthObject->positions.size());
        glBindVertexArray(0);

        // 6.3 bonus, render cube and helicopter, final matrix = airplaneModel * helicopter
        if(useHelicopter){
            // body
            glm::mat4 h_body1(1.0f);
            h_body1 = glm::scale(h_body1, glm::vec3(10, 5, 10));
            h_body1 = glm::rotate(h_body1, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            h_body1 = airplaneModel * h_body1;
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(h_body1));
            glUniform1f(squz_loc, 0);
            glUniform1f(use_rainbow_loc, useRainbowColor);
            glUniform3fv(rainbow_loc, 1, glm::value_ptr(rainbowColor));
            glUniform3fv(helicopter_loc, 1, glm::value_ptr(glm::vec3(255/255.0,215/255.0,0/255.0)));
            glUniform1f(is_cube_loc, 1);
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, cubeObject->positions.size());

            glm::mat4 h_body2;
            h_body2 = glm::translate(h_body1, glm::vec3(0.5f, 0.0f, 0.0f));
            h_body2 = glm::scale(h_body2, glm::vec3(0.8f, 0.8f, 0.8f));
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(h_body2));
            glUniform3fv(rainbow_loc, 1, glm::value_ptr(rainbowColor));
            glUniform3fv(helicopter_loc, 1, glm::value_ptr(glm::vec3(255/255.0,222/255.0,173/255.0)));
            glUniform1f(use_rainbow_loc, useRainbowColor);
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, cubeObject->positions.size());

            glm::mat4 h_body3;
            h_body3 = glm::translate(h_body1, glm::vec3(1.0f, 0.0f, 0.0f));
            h_body3 = glm::scale(h_body3, glm::vec3(0.3f, 0.6f, 0.6f));
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(h_body3));
            glUniform3fv(rainbow_loc, 1, glm::value_ptr(rainbowColor));
            glUniform3fv(helicopter_loc, 1, glm::value_ptr(glm::vec3(240/255.0,230/255.0,140/255.0)));
            glUniform1f(use_rainbow_loc, useRainbowColor);
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, cubeObject->positions.size());

            // connector
            glm::mat4 connection = h_body1;
            connection = glm::rotate(connection, h_angle, glm::vec3(0, 1, 0));
            connection = glm::translate(connection, glm::vec3(0, 0.5, 0));
            connection = glm::scale(connection, glm::vec3(0.3, 0.3, 0.3));
            h_angle += glm::radians(5.0f);
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(connection));
            glUniform3fv(rainbow_loc, 1, glm::value_ptr(rainbowColor));
            glUniform3fv(helicopter_loc, 1, glm::value_ptr(glm::vec3(70/255.0,130/255.0,180/255.0)));
            glUniform1f(use_rainbow_loc, useRainbowColor);
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, cubeObject->positions.size());

            // blade
            glm::mat4 blade1, blade2, blade3, blade4;
            blade1 = glm::translate(connection, glm::vec3(1.5, 0.3, 0));
            blade1 = glm::scale(blade1, glm::vec3(3, 0.1, 0.7));
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(blade1));
            glUniform3fv(rainbow_loc, 1, glm::value_ptr(rainbowColor));
            glUniform3fv(helicopter_loc, 1, glm::value_ptr(glm::vec3(100/255.0,149/255.0,237/255.0)));
            glUniform1f(use_rainbow_loc, useRainbowColor);
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, cubeObject->positions.size());

            blade2 = glm::translate(connection, glm::vec3(0, 0.3, 1.5));
            blade2 = glm::rotate(blade2, glm::radians(90.0f), glm::vec3(0, 1, 0));
            blade2 = glm::scale(blade2, glm::vec3(3, 0.1, 0.7));
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(blade2));
            glUniform3fv(rainbow_loc, 1, glm::value_ptr(rainbowColor));
            glUniform3fv(helicopter_loc, 1, glm::value_ptr(glm::vec3(147/255.0,112/255.0,219/255.0)));
            glUniform1f(use_rainbow_loc, useRainbowColor);
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, cubeObject->positions.size());

            blade3 = glm::translate(connection, glm::vec3(-1.5, 0.3, 0));
            blade3 = glm::rotate(blade3, glm::radians(180.0f), glm::vec3(0, 1, 0));
            blade3 = glm::scale(blade3, glm::vec3(3, 0.1, 0.7));
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(blade3));
            glUniform3fv(rainbow_loc, 1, glm::value_ptr(rainbowColor));
            glUniform3fv(helicopter_loc, 1, glm::value_ptr(glm::vec3(106/255.0,90/255.0,205/255.0)));
            glUniform1f(use_rainbow_loc, useRainbowColor);
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, cubeObject->positions.size());

            blade4 = glm::translate(connection, glm::vec3(0, 0.3, -1.5));
            blade4 = glm::rotate(blade4, glm::radians(90.0f), glm::vec3(0, 1, 0));
            blade4 = glm::scale(blade4, glm::vec3(3, 0.1, 0.7));
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(blade4));
            glUniform3fv(rainbow_loc, 1, glm::value_ptr(rainbowColor));
            glUniform3fv(helicopter_loc, 1, glm::value_ptr(glm::vec3(139/255.0,0/255.0,139/255.0)));
            glUniform1f(use_rainbow_loc, useRainbowColor);
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, cubeObject->positions.size());
        }

        // Status update
        currentTime = glfwGetTime();
        dt = currentTime - lastTime;
        lastTime = currentTime;
        tot_time += dt;

        /* TODO#7: Update "rotateEarthDegree", "rotateAirplaneDegree", "rotateAxisDegree",
         *          "squeezeFactor", "rainbowColor"
         */
        rotateEarthDegree += rotateEarthSpeed * dt;
        if(rotateEarthDegree >= 360.0f){rotateEarthDegree -= 360.0f;}

        rotateAirplaneDegree += rotateAirplaneSpeed * dt;
        if(rotateAirplaneDegree >= 360.0f){rotateAirplaneDegree -= 360.0f;}
        // std::cout<<rotateAirplaneDegree<<", ";

        if(rotateAxisDegree >= 360.0f){rotateAxisDegree -= 360.0f;}
        
        if(useSqueeze){
            squeezeFactor += 90 * dt;
        }

        if(useRainbowColor){
            hue += 72.0f * dt;
            if(hue >= 360){hue -= 360;}
            glm::vec3 tmp = hsv_to_rgb(glm::vec3(hue, 1.0f, 1.0f));
            rainbowColor = glm::vec3(tmp.r/255.0f, tmp.g/255.0f, tmp.b/255.0f);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

/* TODO#1: createShader
 * input:
 *      filename: shader file name
 *      type: shader type, "vert" means vertex shader, "frag" means fragment shader
 * output: shader object
 * Hint:
 *      glCreateShader, glShaderSource, glCompileShader
 */
unsigned int createShader(const string &filename, const string &type) {
    unsigned int shader;
    if(type == "vert") shader = glCreateShader(GL_VERTEX_SHADER);
    else shader = glCreateShader(GL_FRAGMENT_SHADER);

    std::ifstream f(filename);
    if(!f.is_open()){std::cout<<"in createShader, cannot open file\n";}
    std::stringstream source_stream;
    source_stream << f.rdbuf();
    std::string source = source_stream.str();
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, NULL);

    glCompileShader(shader);

    return shader;
}

/* TODO#2: createProgram
 * input:
 *      vertexShader: vertex shader object
 *      fragmentShader: fragment shader object
 * output: shader program
 * Hint:
 *      glCreateProgram, glAttachShader, glLinkProgram, glDetachShader
 */
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader) {
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);

    return program;
}

/* TODO#3: modelVAO
 * input:
 *      model: Object you want to render
 * output: VAO
 * Hint:
 *      glGenVertexArrays, glBindVertexArray, glGenBuffers, glBindBuffer, glBufferData,
 *      glVertexAttribPointer, glEnableVertexAttribArray,
 */
/* class Object {
 *    vector<float> positions;
 *    vector<float> normals;
 *    vector<float> texcoords;
 * }
 */
unsigned int modelVAO(Object &model) {
    // init, VAO & VBO
    unsigned int VAO, VBO[3];
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(3, VBO);

    // pos
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(GL_FLOAT), &model.positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // norm
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, model.normals.size() * sizeof(GL_FLOAT), &model.normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // texcoor
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, model.texcoords.size() * sizeof(GL_FLOAT), &model.texcoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), 0);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    return VAO;
}

/* TODO#4: loadTexture
 * input:
 *      filename: texture file name
 * output: texture object
 * Hint:
 *      glEnable, glGenTextures, glBindTexture, glTexParameteri, glTexImage2D
 */
unsigned int loadTexture(const string &filename) {
    // init
    GLuint texture;
    // glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load img
    int width, height, nrChannel;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannel, 0);
    if(!data){std::cout<<"in loadTexture, load img fail\n"; return 0;}

    GLenum format;
    if(nrChannel == 1) format = GL_RED;
    else if(nrChannel == 3) format = GL_RGB;
    else if(nrChannel == 4) format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return texture;
}

/* TODO#8: Key callback
 *    1. Press 'd' to increase the "rotateAxisDegree" by 1.
 *    2. Press 'a' to decrease the "rotateAxisDegree" by 1.
 *    3. Press 's' to squeeze the earth.
 *    4. Press 'r' to make the color of the airplane rainbow.
 * Hint:
 *      GLFW_KEY_D, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_R
 *      GLFW_PRESS, GLFW_REPEAT
 */
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        rotateAxisDegree += 1;
    }
    else if(key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        rotateAxisDegree -= 1;
    }
    else if(key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        useSqueeze = !useSqueeze;
    }
    else if(key == GLFW_KEY_R && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        useRainbowColor = !useRainbowColor;
    }
    else if(key == GLFW_KEY_H && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        useHelicopter = !useHelicopter;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

void init() {
#if defined(__linux__) || defined(__APPLE__)
    string dirShader = "../../src/shaders/";
    string dirAsset = "../../src/asset/obj/";
    string dirTexture = "../../src/asset/texture/";
#else
    string dirShader = "..\\..\\src\\shaders\\";
    string dirAsset = "..\\..\\src\\asset\\obj\\";
    string dirTexture = "..\\..\\src\\asset\\texture\\";
#endif

    // Object
    airplaneObject = new Object(dirAsset + "airplane.obj");
    earthObject = new Object(dirAsset + "earth.obj");
    cubeObject = new Object(dirAsset + "cube.obj");

    // Shader
    vertexShader = createShader(dirShader + "vertexShader.vert", "vert");
    fragmentShader = createShader(dirShader + "fragmentShader.frag", "frag");
    shaderProgram = createProgram(vertexShader, fragmentShader);
    glUseProgram(shaderProgram);

    // Texture
    airplaneTexture = loadTexture(dirTexture + "airplane.jpg");
    earthTexture = loadTexture(dirTexture + "earth.jpg");

    // VAO, VBO
    airplaneVAO = modelVAO(*airplaneObject);
    earthVAO = modelVAO(*earthObject);
    cubeVAO = modelVAO(*cubeObject);
}

glm::vec3 hsv_to_rgb(glm::vec3 hsv){
    float h = hsv.r;
    float s = hsv.g;
    float v = hsv.b;

    float c = v*s;
    float x = c * (1 - std::fabs(fmod(h / 60.0f, 2) - 1));
    float m = v-c;

    float r = 0.0f, g = 0.0f, b = 0.0f;
    if(0 <= h && h < 60){
        r=c, g=x, b=0;
    }
    else if(60 <= h && h < 120){
        r=x, g=c, b=0;
    }
    else if(120 <= h && h < 180){
        r=0, g=c, b=x;
    }
    else if(180 <= h && h < 240){
        r=0, g=x, b=c;
    }
    else if(240 <= h && h < 300){
        r=x, g=0, b=c;
    }
    else if(300 <= h && h < 360){
        r=c, g=0, b=x;
    }

    r += m; g += m; b += m;
    return glm::vec3(r*255.0f, g*255.0f, b*255.0f);
}

/*
[ 1 0 0 0 ]
[ 0 -0.987688 0.156434 0 ]
[ -0 -0.156434 -0.987688 -0 ]
[ 0 0 0 1 ]

[ -1 8.74228e-08 -7.64274e-15 0 ]
[ 5.51151e-08 0.630443 -0.776235 0 ]
[ -6.78606e-08 -0.776235 -0.630443 0 ]
[ 1.48811e-06 17.022 -20.9584 1 ]
*/