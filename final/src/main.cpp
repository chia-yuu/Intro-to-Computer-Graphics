#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>

#if __cplusplus >= 201103L
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <forward_list>
#include <future>
#include <initializer_list>
#include <mutex>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <system_error>
#include <thread>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#endif
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "header/cube.h"
#include "header/object.h"
#include "header/shader.h"
#include "header/stb_image.h"

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
unsigned int loadCubemap(std::vector<string> &mFileName);

struct material_t
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float gloss;
};

struct light_t
{
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct model_t
{
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    Object *object;
};

struct camera_t
{
    glm::vec3 position;
    glm::vec3 up;
    float rotationY;
};

// settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

// cube map
unsigned int cubemapTexture;
unsigned int cubemapVAO, cubemapVBO;

// shader programs
int shaderProgramIndex = 0;
std::vector<shader_program_t *> shaderPrograms;
shader_program_t *cubemapShader;

// additional dependencies
light_t light;
material_t material;
camera_t camera;

model_t elmo_chair;
model_t elmo_head;
model_t elmo_eye_left;
model_t elmo_eye_right;
model_t elmo_eye_white_left;
model_t elmo_eye_white_right;
model_t elmo_nose;
model_t elmo_body;

model_t elmo_mouth_up;
model_t elmo_mouth_down;

model_t elmo_arm_left;
model_t elmo_arm_right;

model_t elmo_leg_left;
model_t elmo_leg_right;

// piano
model_t box0;
model_t box1;
model_t box2;
model_t box3;
model_t box4;
model_t box5;
model_t box6;
model_t box7;
model_t box8;
model_t box9;
model_t box10;
model_t box11;
model_t box12;
model_t box13;
model_t box14;
model_t box15;
model_t box16;
model_t box17;
model_t box18;
model_t box19;
model_t box20;
model_t box21;
model_t boxa;
model_t boxb;
model_t boxc;
model_t boxd;
model_t boxe;
model_t boxf;
model_t boxg;
model_t boxh;
model_t boxi;
model_t boxj;
model_t boxk;
model_t boxl;

// model matrix
int moveDir = -1;
float left_arm_move = 0.25f;
float right_arm_move = 0.25f;
// float mouth_up_move = 0.2f;
float mouth_down_move = 0.2f;
float move_k = 1.0;
float head_move = 0.3f;

// Elmo's hair
int hasHair = -1;
float hair_length = 1.0;

glm::mat4 cameraModel;

glm::mat4 elmo_chair_model;
glm::mat4 elmo_head_model;
glm::mat4 elmo_body_model;
glm::mat4 elmo_eye_left_model;
glm::mat4 elmo_eye_right_model;
glm::mat4 elmo_eye_white_left_model;
glm::mat4 elmo_eye_white_right_model;
glm::mat4 elmo_nose_model;

glm::mat4 elmo_mouth_up_model;
glm::mat4 elmo_mouth_down_model;

glm::mat4 elmo_arm_left_model;
glm::mat4 elmo_arm_right_model;

glm::mat4 elmo_leg_left_model;
glm::mat4 elmo_leg_right_model;

// pianoooo
glm::mat4 box0Model;
glm::mat4 box1Model;
glm::mat4 box2Model;
glm::mat4 box3Model;
// keyboard
glm::mat4 box4Model;
glm::mat4 box5Model;
glm::mat4 box6Model;
glm::mat4 box7Model;
glm::mat4 box8Model;
glm::mat4 box9Model;
glm::mat4 box10Model;
glm::mat4 box11Model;
glm::mat4 box12Model;
glm::mat4 box13Model;
glm::mat4 box14Model;
glm::mat4 box15Model;
glm::mat4 box16Model;
glm::mat4 box17Model;
glm::mat4 box18Model;
glm::mat4 box19Model;
glm::mat4 box20Model;
glm::mat4 box21Model;
// black keys
glm::mat4 boxaModel;
glm::mat4 boxbModel;
glm::mat4 boxcModel;
glm::mat4 boxdModel;
glm::mat4 boxeModel;
glm::mat4 boxfModel;
glm::mat4 boxgModel;
glm::mat4 boxhModel;
glm::mat4 boxiModel;
glm::mat4 boxjModel;
glm::mat4 boxkModel;
glm::mat4 boxlModel;

//////////////////////////////////////////////////////////////////////////
// Parameter setup,
// You can change any of the settings if you want

void camera_setup()
{
    camera.position = glm::vec3(0.0, 20.0, 100.0);
    camera.up = glm::vec3(0.0, 1.0, 0.0);
    camera.rotationY = 0;
}

void light_setup()
{
    light.position = glm::vec3(-800.0, 1000.0, 500.0);
    light.ambient = glm::vec3(1.0);
    light.diffuse = glm::vec3(1.0);
    light.specular = glm::vec3(1.0);
}

void material_setup()
{
    material.ambient = glm::vec3(1.0);
    material.diffuse = glm::vec3(1.0);
    material.specular = glm::vec3(0.7);
    material.gloss = 10.5;
}
//////////////////////////////////////////////////////////////////////////

void model_setup()
{

    // Load the object and texture for each model here

#if defined(__linux__) || defined(__APPLE__)
    std::string objDir = "../../src/asset/obj/";
    std::string textureDir = "../../src/asset/texture/";
#else
    std::string objDir = "..\\..\\src\\asset\\obj\\";
    std::string textureDir = "..\\..\\src\\asset\\texture\\";
#endif

    // 設定elmo
    elmo_chair.position = glm::vec3(2.0f, -2.0f, 0.0f);
    elmo_chair.scale = glm::vec3(10.0f, 10.0f, 30.0f);
    elmo_chair.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    elmo_chair.object = new Object(objDir + "cube.obj");
    elmo_chair.object->load_to_buffer();

    elmo_body.position = glm::vec3(0.0f, 0.75f, 0.0f);
    elmo_body.scale = glm::vec3(0.8f, 2.0f, 0.3f);
    elmo_body.rotation = elmo_chair.rotation;

    elmo_head.position = glm::vec3(0.5f, 7.5f, 0.0f);
    elmo_head.scale = glm::vec3(0.4f, 0.2f, 0.4f);
    elmo_head.rotation = elmo_chair.rotation;
    elmo_head.object = new Object(objDir + "earth.obj");
    elmo_head.object->load_to_buffer();

    elmo_eye_left.position = glm::vec3(-10.0f, 21.0f, -5.0f);
    elmo_eye_left.scale = glm::vec3(0.1f, 0.1f, 0.1f);
    elmo_eye_left.rotation = elmo_chair.rotation;

    elmo_eye_white_left.position = glm::vec3(1.3f, 0.0f, 0.0f);
    elmo_eye_white_left.scale = glm::vec3(2.0f, 2.0f, 2.0f);
    elmo_eye_white_left.rotation = elmo_chair.rotation;

    elmo_eye_right.position = glm::vec3(-10.0f, 21.0f, 5.0f);
    elmo_eye_right.scale = glm::vec3(0.1f, 0.1f, 0.1f);
    elmo_eye_right.rotation = elmo_chair.rotation;

    elmo_eye_white_right.position = glm::vec3(1.3f, 0.0f, 0.0f);
    elmo_eye_white_right.scale = glm::vec3(2.0f, 2.0f, 2.0f);
    elmo_eye_white_right.rotation = elmo_chair.rotation;

    elmo_nose.position = glm::vec3(-2.2f, -0.5f, 0.0f);
    elmo_nose.scale = glm::vec3(0.2f, 0.3f, 0.2f);
    elmo_nose.rotation = glm::vec3(0.0f, 0.0f, -45.0f);

    elmo_mouth_up.position = glm::vec3(-9.5f, -4.2f, 0.0f);
    elmo_mouth_up.scale = glm::vec3(0.2f, 0.05f, 0.4f);
    elmo_mouth_up.rotation = elmo_chair.rotation;

    elmo_mouth_down.position = glm::vec3(-9.5f, -5.7f, 0.0f);
    elmo_mouth_down.scale = glm::vec3(0.2f, 0.05f, 0.4f);
    elmo_mouth_down.rotation = elmo_chair.rotation;

    elmo_arm_left.position = glm::vec3(40.0f, -3.0f, 0.0f);
    elmo_arm_left.scale = glm::vec3(1.5, 0.1f, 0.3f);
    elmo_arm_left.rotation = glm::vec3(0.0f, 0.0f, 180.0f);

    elmo_arm_right.position = glm::vec3(40.0f, -3.0f, 0.0f);
    elmo_arm_right.scale = glm::vec3(1.5f, 0.1f, 0.3f);
    elmo_arm_right.rotation = glm::vec3(0.0f, 0.0f, 170.0f);

    elmo_leg_left.position = glm::vec3(-0.8f, -4.5f, -1.0f);
    elmo_leg_left.scale = glm::vec3(1.2f, 0.1f, 0.3f);
    elmo_leg_left.rotation = elmo_chair.rotation;

    elmo_leg_right.position = glm::vec3(-0.8f, -4.5f, 1.0f);
    elmo_leg_right.scale = glm::vec3(1.2f, 0.1f, 0.3f);
    elmo_leg_right.rotation = elmo_chair.rotation;

    // 設定pianoooo
    box1Model = glm::mat4(1.0f);
    box1.position = glm::vec3(-0.5f, 0.0f, 0.0f);
    box1.scale = glm::vec3(7.5f, 30.0f, 45.0f);
    box1.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    box1.object = new Object(objDir + "cube.obj");
    box1.object->load_to_buffer();

    // box0.position = box1.position + glm::vec3(0.0f, -0.5f, 0.0f);
    // box0.scale = glm::vec3(2.5f, 15.0f, 48.0f);
    box0.position = box1.position + glm::vec3(0.0f, -0.5f, 0.0f);
    box0.scale = glm::vec3(2.5f, 5.0f, 48.0f);
    box0.rotation = box1.rotation;
    box0.object = new Object(objDir + "cube.obj");
    box0.object->load_to_buffer();

    box2.position = box1.position + glm::vec3(1.0f, -0.5f, -7.5f);
    box2.scale = glm::vec3(7.5f, 22.5f, 3.0f);
    box2.rotation = box1.rotation;
    box2.object = new Object(objDir + "cube.obj");
    box2.object->load_to_buffer();

    box3.position = box1.position + glm::vec3(1.0f, -0.5f, 7.5f);
    box3.scale = glm::vec3(7.5f, 22.5f, 3.0f);
    box3.rotation = box1.rotation;
    box3.object = new Object(objDir + "cube.obj");
    box3.object->load_to_buffer();

    box4.position = box1.position + glm::vec3(1.0f, -2.5f, 12.5f);
    box4.scale = glm::vec3(7.5f, 1.5f, 1.6f);
    box4.rotation = box1.rotation;
    box4.object = new Object(objDir + "cube.obj");
    box4.object->load_to_buffer();

    box5.position = box1.position + glm::vec3(1.0f, -2.5f, 11.0f);
    box5.scale = glm::vec3(7.5f, 1.5f, 1.6f);
    box5.rotation = box1.rotation;
    box5.object = new Object(objDir + "cube.obj");
    box5.object->load_to_buffer();

    box6.position = box1.position + glm::vec3(1.0f, -2.5f, 9.5f);
    box6.scale = glm::vec3(7.5f, 1.5f, 1.6f);
    box6.rotation = box1.rotation;
    box6.object = new Object(objDir + "cube.obj");
    box6.object->load_to_buffer();

    box7.position = box1.position + glm::vec3(1.0f, -2.5f, 8.0f);
    box7.scale = glm::vec3(7.5f, 1.5f, 1.6f);
    box7.rotation = box1.rotation;
    box7.object = new Object(objDir + "cube.obj");
    box7.object->load_to_buffer();

    box8.position = box1.position + glm::vec3(1.0f, -2.5f, 6.5f);
    box8.scale = glm::vec3(7.5f, 1.5f, 1.6f);
    box8.rotation = box1.rotation;
    box8.object = new Object(objDir + "cube.obj");
    box8.object->load_to_buffer();

    box9.position = box1.position + glm::vec3(1.0f, -2.5f, 5.0f);
    box9.scale = glm::vec3(7.5f, 1.5f, 1.6f);
    box9.rotation = box1.rotation;
    box9.object = new Object(objDir + "cube.obj");
    box9.object->load_to_buffer();

    box10.position = box1.position + glm::vec3(1.0f, -2.5f, 3.5f);
    box10.scale = glm::vec3(7.5f, 1.5f, 1.6f);
    box10.rotation = box1.rotation;
    box10.object = new Object(objDir + "cube.obj");
    box10.object->load_to_buffer();

    box11.position = box1.position + glm::vec3(1.0f, -2.5f, 2.0f);
    box11.scale = glm::vec3(7.5f, 1.5f, 1.6f);
    box11.rotation = box1.rotation;
    box11.object = new Object(objDir + "cube.obj");
    box11.object->load_to_buffer();

    box12.position = box1.position + glm::vec3(1.0f, -2.5f, 0.5f);
    box12.scale = glm::vec3(7.5f, 1.5f, 1.6f);
    box12.rotation = box1.rotation;
    box12.object = new Object(objDir + "cube.obj");
    box12.object->load_to_buffer();

    box13.position = box1.position + glm::vec3(1.0f, -2.5f, -1.0f);
    box13.scale = glm::vec3(7.5f, 1.5f, 1.6f);
    box13.rotation = box1.rotation;
    box13.object = new Object(objDir + "cube.obj");
    box13.object->load_to_buffer();

    box14.position = box1.position + glm::vec3(1.0f, -2.5f, -2.5f);
    box14.scale = glm::vec3(7.5f, 1.5f, 1.6f);
    box14.rotation = box1.rotation;
    box14.object = new Object(objDir + "cube.obj");
    box14.object->load_to_buffer();

    box15.position = box1.position + glm::vec3(1.0f, -2.5f, -4.0f);
    box15.scale = glm::vec3(7.5f, 1.5f, 1.6f);
    box15.rotation = box1.rotation;
    box15.object = new Object(objDir + "cube.obj");
    box15.object->load_to_buffer();

    box16.position = box1.position + glm::vec3(1.0f, -2.5f, -5.5f);
    box16.scale = glm::vec3(7.5f, 1.5f, 1.6f);
    box16.rotation = box1.rotation;
    box16.object = new Object(objDir + "cube.obj");
    box16.object->load_to_buffer();

    box17.position = box1.position + glm::vec3(1.0f, -2.5f, -7.0f);
    box17.scale = glm::vec3(7.5f, 1.5f, 1.6f);
    box17.rotation = box1.rotation;
    box17.object = new Object(objDir + "cube.obj");
    box17.object->load_to_buffer();

    box18.position = box1.position + glm::vec3(1.0f, -2.5f, -8.5f);
    box18.scale = glm::vec3(7.5f, 1.5f, 1.6f);
    box18.rotation = box1.rotation;
    box18.object = new Object(objDir + "cube.obj");
    box18.object->load_to_buffer();

    box19.position = box1.position + glm::vec3(1.0f, -2.5f, -10.0f);
    box19.scale = glm::vec3(7.5f, 1.5f, 1.6f);
    box19.rotation = box1.rotation;
    box19.object = new Object(objDir + "cube.obj");
    box19.object->load_to_buffer();

    box20.position = box1.position + glm::vec3(1.0f, -2.5f, -11.25f);
    box20.scale = glm::vec3(7.5f, 1.5f, 1.6f);
    box20.rotation = box1.rotation;
    box20.object = new Object(objDir + "cube.obj");
    box20.object->load_to_buffer();

    box21.position = box1.position + glm::vec3(1.0f, -2.5f, -12.5f);
    box21.scale = glm::vec3(7.5f, 1.5f, 1.6f);
    box21.rotation = box1.rotation;
    box21.object = new Object(objDir + "cube.obj");
    box21.object->load_to_buffer();

    boxa.position = box1.position + glm::vec3(1.0f, -2.0f, 11.75f);
    boxa.scale = glm::vec3(6.0f, 1.5f, 1.6f);
    boxa.rotation = box1.rotation;
    boxa.object = new Object(objDir + "cube.obj");
    boxa.object->load_to_buffer();

    boxb.position = box1.position + glm::vec3(1.0f, -2.0f, 10.25f);
    boxb.scale = glm::vec3(6.0f, 1.5f, 1.6f);
    boxb.rotation = box1.rotation;
    boxb.object = new Object(objDir + "cube.obj");
    boxb.object->load_to_buffer();

    boxc.position = box1.position + glm::vec3(1.0f, -2.0f, 7.25f);
    boxc.scale = glm::vec3(6.0f, 1.5f, 1.6f);
    boxc.rotation = box1.rotation;
    boxc.object = new Object(objDir + "cube.obj");
    boxc.object->load_to_buffer();

    boxd.position = box1.position + glm::vec3(1.0f, -2.0f, 5.75f);
    boxd.scale = glm::vec3(6.0f, 1.5f, 1.6f);
    boxd.rotation = box1.rotation;
    boxd.object = new Object(objDir + "cube.obj");
    boxd.object->load_to_buffer();

    boxe.position = box1.position + glm::vec3(1.0f, -2.0f, 4.25f);
    boxe.scale = glm::vec3(6.0f, 1.5f, 1.6f);
    boxe.rotation = box1.rotation;
    boxe.object = new Object(objDir + "cube.obj");
    boxe.object->load_to_buffer();

    boxf.position = box1.position + glm::vec3(1.0f, -2.0f, 1.25f);
    boxf.scale = glm::vec3(6.0f, 1.5f, 1.6f);
    boxf.rotation = box1.rotation;
    boxf.object = new Object(objDir + "cube.obj");
    boxf.object->load_to_buffer();

    boxg.position = box1.position + glm::vec3(1.0f, -2.0f, -0.25f);
    boxg.scale = glm::vec3(6.0f, 1.5f, 1.6f);
    boxg.rotation = box1.rotation;
    boxg.object = new Object(objDir + "cube.obj");
    boxg.object->load_to_buffer();

    boxh.position = box1.position + glm::vec3(1.0f, -2.0f, -3.25f);
    boxh.scale = glm::vec3(6.0f, 1.5f, 1.6f);
    boxh.rotation = box1.rotation;
    boxh.object = new Object(objDir + "cube.obj");
    boxh.object->load_to_buffer();

    boxi.position = box1.position + glm::vec3(1.0f, -2.0f, -4.75f);
    boxi.scale = glm::vec3(6.0f, 1.5f, 1.6f);
    boxi.rotation = box1.rotation;
    boxi.object = new Object(objDir + "cube.obj");
    boxi.object->load_to_buffer();

    boxj.position = box1.position + glm::vec3(1.0f, -2.0f, -6.25f);
    boxj.scale = glm::vec3(6.0f, 1.5f, 1.6f);
    boxj.rotation = box1.rotation;
    boxj.object = new Object(objDir + "cube.obj");
    boxj.object->load_to_buffer();

    boxk.position = box1.position + glm::vec3(1.0f, -2.0f, -9.25f);
    boxk.scale = glm::vec3(6.0f, 1.5f, 1.6f);
    boxk.rotation = box1.rotation;
    boxk.object = new Object(objDir + "cube.obj");
    boxk.object->load_to_buffer();

    boxl.position = box1.position + glm::vec3(1.0f, -2.0f, -10.75f);
    boxl.scale = glm::vec3(6.0f, 1.5f, 1.6f);
    boxl.rotation = box1.rotation;
    boxl.object = new Object(objDir + "cube.obj");
    boxl.object->load_to_buffer();
}

void shader_setup()
{

    // Setup the shader program for each shading method

#if defined(__linux__) || defined(__APPLE__)
    std::string shaderDir = "../../src/shaders/";
#else
    std::string shaderDir = "..\\..\\src\\shaders\\";
#endif

    std::vector<std::string> shadingMethod = {"elmo", "piano"};

    for (int i = 0; i < shadingMethod.size(); i++)
    {
        std::string vpath = shaderDir + shadingMethod[i] + ".vert";
        std::string gpath = shaderDir + shadingMethod[i] + ".geom";
        std::string fpath = shaderDir + shadingMethod[i] + ".frag";

        shader_program_t *shaderProgram = new shader_program_t();
        shaderProgram->create();

        shaderProgram->add_shader(vpath, GL_VERTEX_SHADER);
        std::ifstream gpathFile(gpath);
        if (gpathFile.good())
        {
            shaderProgram->add_shader(gpath, GL_GEOMETRY_SHADER);
        }
        shaderProgram->add_shader(fpath, GL_FRAGMENT_SHADER);
        shaderProgram->link_shader();
        shaderPrograms.push_back(shaderProgram);
    }
}

void cubemap_setup()
{

    // Setup all the necessary things for cubemap rendering
    // Including: cubemap texture, shader program, VAO, VBO

#if defined(__linux__) || defined(__APPLE__)
    std::string cubemapDir = "../../src/asset/texture/skybox/";
    std::string shaderDir = "../../src/shaders/";
#else
    std::string cubemapDir = "..\\..\\src\\asset\\texture\\skybox\\";
    std::string shaderDir = "..\\..\\src\\shaders\\";
#endif

    // setup texture for cubemap
    std::vector<std::string> faces{
        cubemapDir + "right.jpg",
        cubemapDir + "left.jpg",
        cubemapDir + "top.jpg",
        cubemapDir + "bottom.jpg",
        cubemapDir + "front.jpg",
        cubemapDir + "back.jpg"};
    cubemapTexture = loadCubemap(faces);

    // setup shader for cubemap
    std::string vpath = shaderDir + "cubemap.vert";
    std::string fpath = shaderDir + "cubemap.frag";

    cubemapShader = new shader_program_t();
    cubemapShader->create();
    cubemapShader->add_shader(vpath, GL_VERTEX_SHADER);
    cubemapShader->add_shader(fpath, GL_FRAGMENT_SHADER);
    cubemapShader->link_shader();

    glGenVertexArrays(1, &cubemapVAO);
    glGenBuffers(1, &cubemapVBO);
    glBindVertexArray(cubemapVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubemapVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubemapVertices), &cubemapVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
}

void setup()
{

    // Initialize shader model camera light material
    light_setup();
    model_setup();
    shader_setup();
    camera_setup();
    cubemap_setup();
    material_setup();

    // Enable depth test, face culling ...
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    // Debug: enable for debugging
    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback([](  GLenum source, GLenum type, GLuint id, GLenum severity,
    //                             GLsizei length, const GLchar* message, const void* userParam) {

    // std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
    //           << "type = " << type
    //           << ", severity = " << severity
    //           << ", message = " << message << std::endl;
    // }, nullptr);
}

void update()
{

    // Update the heicopter position, camera position, rotation, etc.

    elmo_arm_left.rotation.z += left_arm_move * move_k;
    if (elmo_arm_left.rotation.z > 180.0 || elmo_arm_left.rotation.z < 170.0)
    {
        left_arm_move = -left_arm_move;
    }
    elmo_arm_right.rotation.z += right_arm_move * move_k;
    if (elmo_arm_right.rotation.z > 180.0 || elmo_arm_right.rotation.z < 170)
    {
        right_arm_move = -right_arm_move;
    }

    elmo_mouth_down.position.y -= mouth_down_move * move_k;
    if (elmo_mouth_down.position.y > -4.8 || elmo_mouth_down.position.y < -13.6)
    {
        mouth_down_move = -mouth_down_move;
    }

    elmo_head.rotation.z += head_move;
    if (elmo_head.rotation.z > 10.0 || elmo_head.rotation.z < -5.0)
    {
        head_move = -head_move;
    }

    elmo_chair_model = glm::mat4(1.0f);
    elmo_chair_model = glm::scale(elmo_chair_model, elmo_chair.scale);
    elmo_chair_model = glm::translate(elmo_chair_model, elmo_chair.position);

    elmo_body_model = glm::scale(elmo_chair_model, elmo_body.scale);
    elmo_body_model = glm::translate(elmo_body_model, elmo_body.position);

    elmo_head_model = glm::mat4(1.0f);
    elmo_head_model = glm::translate(elmo_body_model, -elmo_body.position);
    elmo_head_model = glm::scale(elmo_head_model, elmo_head.scale);
    elmo_head_model = glm::rotate(elmo_head_model, glm::radians(elmo_head.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    elmo_head_model = glm::translate(elmo_head_model, elmo_head.position);

    elmo_eye_left_model = glm::scale(elmo_head_model, elmo_eye_left.scale);
    elmo_eye_left_model = glm::translate(elmo_eye_left_model, elmo_eye_left.position);

    elmo_eye_white_left_model = glm::scale(elmo_eye_left_model, elmo_eye_white_left.scale);
    elmo_eye_white_left_model = glm::translate(elmo_eye_white_left_model, elmo_eye_white_left.position);

    elmo_eye_right_model = glm::scale(elmo_head_model, elmo_eye_right.scale);
    elmo_eye_right_model = glm::translate(elmo_eye_right_model, elmo_eye_right.position);

    elmo_eye_white_right_model = glm::scale(elmo_eye_right_model, elmo_eye_white_right.scale);
    elmo_eye_white_right_model = glm::translate(elmo_eye_white_right_model, elmo_eye_white_right.position);

    elmo_nose_model = glm::rotate(elmo_head_model, glm::radians(elmo_nose.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    elmo_nose_model = glm::translate(elmo_nose_model, elmo_nose.position + elmo_body.position);
    elmo_nose_model = glm::scale(elmo_nose_model, elmo_nose.scale);

    elmo_mouth_up_model = glm::scale(elmo_head_model, elmo_mouth_up.scale);
    elmo_mouth_up_model = glm::translate(elmo_mouth_up_model, elmo_mouth_up.position);

    elmo_mouth_down_model = glm::scale(elmo_head_model, elmo_mouth_down.scale);
    elmo_mouth_down_model = glm::translate(elmo_mouth_down_model, elmo_mouth_down.position);

    elmo_arm_left_model = glm::mat4(1.0f);
    elmo_arm_left_model = glm::translate(elmo_arm_left_model, elmo_arm_left.position);
    elmo_arm_left_model = glm::rotate(elmo_arm_left_model, glm::radians(elmo_arm_left.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    elmo_arm_left_model = glm::scale(elmo_arm_left_model, elmo_arm_left.scale);
    elmo_arm_left_model = glm::translate(elmo_arm_left_model, glm::vec3(0.0f, 0.0f, -18.0f));
    elmo_arm_left_model = elmo_arm_left_model * elmo_body_model;

    elmo_arm_right_model = glm::mat4(1.0f);
    elmo_arm_right_model = glm::translate(elmo_arm_right_model, elmo_arm_right.position);
    elmo_arm_right_model = glm::rotate(elmo_arm_right_model, glm::radians(elmo_arm_right.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    elmo_arm_right_model = glm::scale(elmo_arm_right_model, elmo_arm_right.scale);
    elmo_arm_right_model = glm::translate(elmo_arm_right_model, glm::vec3(0.0f, 0.0f, 18.0f));
    elmo_arm_right_model = elmo_arm_right_model * elmo_body_model;

    elmo_leg_left_model = glm::scale(elmo_body_model, elmo_leg_left.scale);
    elmo_leg_left_model = glm::translate(elmo_leg_left_model, elmo_leg_left.position);

    elmo_leg_right_model = glm::scale(elmo_body_model, elmo_leg_right.scale);
    elmo_leg_right_model = glm::translate(elmo_leg_right_model, elmo_leg_right.position);

    box1Model = glm::mat4(1.0f);
    box1Model = glm::scale(box1Model, box1.scale);
    box1Model = glm::translate(box1Model, box1.position);

    box0Model = glm::mat4(1.0f);
    box0Model = glm::scale(box0Model, box0.scale);
    box0Model = glm::translate(box0Model, box0.position);

    box2Model = glm::mat4(1.0f);
    box2Model = glm::scale(box2Model, box2.scale);
    box2Model = glm::translate(box2Model, box2.position);

    box3Model = glm::mat4(1.0f);
    box3Model = glm::scale(box3Model, box3.scale);
    box3Model = glm::translate(box3Model, box3.position);

    box4Model = glm::mat4(1.0f);
    box4Model = glm::scale(box4Model, box4.scale);
    box4Model = glm::translate(box4Model, box4.position);

    box5Model = glm::mat4(1.0f);
    box5Model = glm::scale(box5Model, box5.scale);
    box5Model = glm::translate(box5Model, box5.position);

    box6Model = glm::mat4(1.0f);
    box6Model = glm::scale(box6Model, box6.scale);
    box6Model = glm::translate(box6Model, box6.position);

    box7Model = glm::mat4(1.0f);
    box7Model = glm::scale(box7Model, box7.scale);
    box7Model = glm::translate(box7Model, box7.position);

    box8Model = glm::mat4(1.0f);
    box8Model = glm::scale(box8Model, box8.scale);
    box8Model = glm::translate(box8Model, box8.position);

    box9Model = glm::mat4(1.0f);
    box9Model = glm::scale(box9Model, box9.scale);
    box9Model = glm::translate(box9Model, box9.position);

    box10Model = glm::mat4(1.0f);
    box10Model = glm::scale(box10Model, box10.scale);
    box10Model = glm::translate(box10Model, box10.position);

    box11Model = glm::mat4(1.0f);
    box11Model = glm::scale(box11Model, box11.scale);
    box11Model = glm::translate(box11Model, box11.position);

    box12Model = glm::mat4(1.0f);
    box12Model = glm::scale(box12Model, box12.scale);
    box12Model = glm::translate(box12Model, box12.position);

    box13Model = glm::mat4(1.0f);
    box13Model = glm::scale(box13Model, box13.scale);
    box13Model = glm::translate(box13Model, box13.position);

    box14Model = glm::mat4(1.0f);
    box14Model = glm::scale(box14Model, box14.scale);
    box14Model = glm::translate(box14Model, box14.position);

    box15Model = glm::mat4(1.0f);
    box15Model = glm::scale(box15Model, box15.scale);
    box15Model = glm::translate(box15Model, box15.position);

    box16Model = glm::mat4(1.0f);
    box16Model = glm::scale(box16Model, box16.scale);
    box16Model = glm::translate(box16Model, box16.position);

    box17Model = glm::mat4(1.0f);
    box17Model = glm::scale(box17Model, box17.scale);
    box17Model = glm::translate(box17Model, box17.position);

    box18Model = glm::mat4(1.0f);
    box18Model = glm::scale(box18Model, box18.scale);
    box18Model = glm::translate(box18Model, box18.position);

    box19Model = glm::mat4(1.0f);
    box19Model = glm::scale(box19Model, box19.scale);
    box19Model = glm::translate(box19Model, box19.position);

    box20Model = glm::mat4(1.0f);
    box20Model = glm::scale(box20Model, box20.scale);
    box20Model = glm::translate(box20Model, box20.position);

    box21Model = glm::mat4(1.0f);
    box21Model = glm::scale(box21Model, box21.scale);
    box21Model = glm::translate(box21Model, box21.position);

    boxaModel = glm::mat4(1.0f);
    boxaModel = glm::scale(boxaModel, boxa.scale);
    boxaModel = glm::translate(boxaModel, boxa.position);

    boxbModel = glm::mat4(1.0f);
    boxbModel = glm::scale(boxbModel, boxb.scale);
    boxbModel = glm::translate(boxbModel, boxb.position);

    boxcModel = glm::mat4(1.0f);
    boxcModel = glm::scale(boxcModel, boxc.scale);
    boxcModel = glm::translate(boxcModel, boxc.position);

    boxdModel = glm::mat4(1.0f);
    boxdModel = glm::scale(boxdModel, boxd.scale);
    boxdModel = glm::translate(boxdModel, boxd.position);

    boxeModel = glm::mat4(1.0f);
    boxeModel = glm::scale(boxeModel, boxe.scale);
    boxeModel = glm::translate(boxeModel, boxe.position);

    boxfModel = glm::mat4(1.0f);
    boxfModel = glm::scale(boxfModel, boxf.scale);
    boxfModel = glm::translate(boxfModel, boxf.position);

    boxgModel = glm::mat4(1.0f);
    boxgModel = glm::scale(boxgModel, boxg.scale);
    boxgModel = glm::translate(boxgModel, boxg.position);

    boxhModel = glm::mat4(1.0f);
    boxhModel = glm::scale(boxhModel, boxh.scale);
    boxhModel = glm::translate(boxhModel, boxh.position);

    boxiModel = glm::mat4(1.0f);
    boxiModel = glm::scale(boxiModel, boxi.scale);
    boxiModel = glm::translate(boxiModel, boxi.position);

    boxjModel = glm::mat4(1.0f);
    boxjModel = glm::scale(boxjModel, boxj.scale);
    boxjModel = glm::translate(boxjModel, boxj.position);

    boxkModel = glm::mat4(1.0f);
    boxkModel = glm::scale(boxkModel, boxk.scale);
    boxkModel = glm::translate(boxkModel, boxk.position);

    boxlModel = glm::mat4(1.0f);
    boxlModel = glm::scale(boxlModel, boxl.scale);
    boxlModel = glm::translate(boxlModel, boxl.position);

    camera.rotationY = (camera.rotationY > 360.0) ? 0.0 : camera.rotationY;
    cameraModel = glm::mat4(1.0f);
    cameraModel = glm::rotate(cameraModel, glm::radians(camera.rotationY), camera.up);
    cameraModel = glm::translate(cameraModel, camera.position);
}

void render()
{

    glClearColor(0.0, 255.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Calculate view, projection matrix
    glm::mat4 view = glm::lookAt(glm::vec3(cameraModel[3]), glm::vec3(0.0), camera.up);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

    // pianooo
    shaderPrograms[1]->use();

    shaderPrograms[1]->set_uniform_value("model", box0Model);
    shaderPrograms[1]->set_uniform_value("view", view);
    shaderPrograms[1]->set_uniform_value("projection", projection);
    shaderPrograms[1]->set_uniform_value("skybox", 1);

    shaderPrograms[1]->set_uniform_value("light.position", light.position);
    shaderPrograms[1]->set_uniform_value("light.ambient", light.ambient);
    shaderPrograms[1]->set_uniform_value("light.diffuse", light.diffuse);
    shaderPrograms[1]->set_uniform_value("light.specular", light.specular);

    shaderPrograms[1]->set_uniform_value("material.ambient", material.ambient);
    shaderPrograms[1]->set_uniform_value("material.diffuse", material.diffuse);
    shaderPrograms[1]->set_uniform_value("material.specular", material.specular);
    shaderPrograms[1]->set_uniform_value("material.gloss", material.gloss);

    shaderPrograms[1]->set_uniform_value("camera.position", glm::vec3(cameraModel[3]));

    shaderProgramIndex = 1;
    shaderPrograms[shaderProgramIndex]->set_uniform_value("white", 2);
    box0.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", box2Model);
    box2.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", box3Model);
    box3.object->render();

    shaderPrograms[shaderProgramIndex]->set_uniform_value("white", 0);
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", box4Model);
    box4.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", box5Model);
    box5.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", box6Model);
    box6.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", box7Model);
    box7.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", box8Model);
    box8.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", box9Model);
    box9.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", box10Model);
    box10.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", box11Model);
    box11.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", box12Model);
    box12.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", box13Model);
    box13.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", box14Model);
    box14.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", box15Model);
    box15.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", box16Model);
    box16.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", box17Model);
    box17.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", box18Model);
    box18.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", box19Model);
    box19.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", box20Model);
    box20.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", box21Model);
    box21.object->render();

    shaderPrograms[shaderProgramIndex]->set_uniform_value("white", 1);
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", boxaModel);
    boxa.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", boxbModel);
    boxb.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", boxcModel);
    boxc.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", boxdModel);
    boxd.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", boxeModel);
    boxe.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", boxfModel);
    boxf.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", boxgModel);
    boxg.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", boxhModel);
    boxh.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", boxiModel);
    boxi.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", boxjModel);
    boxj.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", boxkModel);
    boxk.object->render();
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", boxlModel);
    boxl.object->render();

    shaderPrograms[shaderProgramIndex]->set_uniform_value("white", 3);

    // Elmo 不長毛部分
    shaderPrograms[shaderProgramIndex]->set_uniform_value("objectColor", glm::vec3(230.0 / 255.0, 31.0 / 255.0, 34.0 / 255.0));
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", elmo_head_model);
    elmo_head.object->render();

    shaderPrograms[shaderProgramIndex]->set_uniform_value("objectColor", glm::vec3(0.0 / 255.0, 0.0 / 255.0, 0.0 / 255.0));
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", elmo_eye_left_model);
    elmo_head.object->render();

    shaderPrograms[shaderProgramIndex]->set_uniform_value("objectColor", glm::vec3(1.0, 1.0, 1.0));
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", elmo_eye_white_left_model);
    elmo_head.object->render();

    shaderPrograms[shaderProgramIndex]->set_uniform_value("objectColor", glm::vec3(0.0 / 255.0, 0.0 / 255.0, 0.0 / 255.0));
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", elmo_eye_right_model);
    elmo_head.object->render();

    shaderPrograms[shaderProgramIndex]->set_uniform_value("objectColor", glm::vec3(1.0, 1.0, 1.0));
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", elmo_eye_white_right_model);
    elmo_head.object->render();

    shaderPrograms[shaderProgramIndex]->set_uniform_value("objectColor", glm::vec3(255.0 / 255.0, 133.0 / 255.0, 10.0 / 255.0));
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", elmo_nose_model);
    elmo_head.object->render();

    shaderPrograms[shaderProgramIndex]->set_uniform_value("objectColor", glm::vec3(0.0 / 255.0, 0.0 / 255.0, 0.0 / 255.0));
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", elmo_mouth_up_model);
    elmo_head.object->render();

    shaderPrograms[shaderProgramIndex]->set_uniform_value("objectColor", glm::vec3(0.0 / 255.0, 0.0 / 255.0, 0.0 / 255.0));
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", elmo_mouth_down_model);
    elmo_head.object->render();

    shaderPrograms[shaderProgramIndex]->set_uniform_value("objectColor", glm::vec3(83.0 / 255.0, 62.0 / 255.0, 45.0 / 255.0));
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", elmo_chair_model);
    elmo_chair.object->render();

    shaderPrograms[shaderProgramIndex]->set_uniform_value("objectColor", glm::vec3(230.0 / 255.0, 31.0 / 255.0, 34.0 / 255.0));
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", elmo_body_model);
    elmo_chair.object->render();

    shaderPrograms[shaderProgramIndex]->set_uniform_value("objectColor", glm::vec3(204.0 / 255.0, 0.0 / 255.0, 3.0 / 255.0));
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", elmo_arm_left_model);
    elmo_chair.object->render();

    shaderPrograms[shaderProgramIndex]->set_uniform_value("objectColor", glm::vec3(204.0 / 255.0, 0.0 / 255.0, 3.0 / 255.0));
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", elmo_arm_right_model);
    elmo_chair.object->render();

    shaderPrograms[shaderProgramIndex]->set_uniform_value("objectColor", glm::vec3(204.0 / 255.0, 0.0 / 255.0, 3.0 / 255.0));
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", elmo_leg_left_model);
    elmo_chair.object->render();

    shaderPrograms[shaderProgramIndex]->set_uniform_value("objectColor", glm::vec3(204.0 / 255.0, 0.0 / 255.0, 3.0 / 255.0));
    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", elmo_leg_right_model);
    elmo_chair.object->render();

    shaderPrograms[shaderProgramIndex]->set_uniform_value("model", boxaModel);
    boxa.object->render();

    shaderPrograms[shaderProgramIndex]->release();

    // Elmo長毛部分
    // Set matrix for view, projection, model transformation
    if (hasHair > 0)
    {
        shaderPrograms[0]->use();
        shaderPrograms[0]->set_uniform_value("view", view);
        shaderPrograms[0]->set_uniform_value("projection", projection);

        // TODO 1
        // Set uniform value for each shader program

        shaderPrograms[0]->set_uniform_value("lightPos", light.position);
        shaderPrograms[0]->set_uniform_value("lightAmbient", light.ambient);
        shaderPrograms[0]->set_uniform_value("lightDiffuse", light.diffuse);
        shaderPrograms[0]->set_uniform_value("lightSpecular", light.specular);

        shaderPrograms[0]->set_uniform_value("materialAmbient", material.ambient);
        shaderPrograms[0]->set_uniform_value("materialDiffuse", material.diffuse);
        shaderPrograms[0]->set_uniform_value("materialSpecular", material.specular);
        shaderPrograms[0]->set_uniform_value("materialGloss", material.gloss);

        shaderPrograms[0]->set_uniform_value("viewPos", glm::vec3(cameraModel[3]));
        shaderPrograms[0]->set_uniform_value("has_txt", 0);
        // 控制elmo長度、髮量
        shaderPrograms[0]->set_uniform_value("hairLength", hair_length);
        shaderPrograms[0]->set_uniform_value("hairCount", 5);

        // elmo
        shaderProgramIndex = 0;

        shaderPrograms[shaderProgramIndex]->set_uniform_value("objectColor", glm::vec3(230.0 / 255.0, 31.0 / 255.0, 34.0 / 255.0));
        shaderPrograms[shaderProgramIndex]->set_uniform_value("model", elmo_head_model);
        elmo_head.object->render();

        shaderPrograms[shaderProgramIndex]->set_uniform_value("objectColor", glm::vec3(0.0 / 255.0, 0.0 / 255.0, 0.0 / 255.0));
        shaderPrograms[shaderProgramIndex]->set_uniform_value("model", elmo_eye_left_model);
        elmo_head.object->render();

        shaderPrograms[shaderProgramIndex]->set_uniform_value("objectColor", glm::vec3(1.0, 1.0, 1.0));
        shaderPrograms[shaderProgramIndex]->set_uniform_value("model", elmo_eye_white_left_model);
        elmo_head.object->render();

        shaderPrograms[shaderProgramIndex]->set_uniform_value("objectColor", glm::vec3(0.0 / 255.0, 0.0 / 255.0, 0.0 / 255.0));
        shaderPrograms[shaderProgramIndex]->set_uniform_value("model", elmo_eye_right_model);
        elmo_head.object->render();

        shaderPrograms[shaderProgramIndex]->set_uniform_value("objectColor", glm::vec3(1.0, 1.0, 1.0));
        shaderPrograms[shaderProgramIndex]->set_uniform_value("model", elmo_eye_white_right_model);
        elmo_head.object->render();

        shaderPrograms[shaderProgramIndex]->set_uniform_value("objectColor", glm::vec3(255.0 / 255.0, 133.0 / 255.0, 10.0 / 255.0));
        shaderPrograms[shaderProgramIndex]->set_uniform_value("model", elmo_nose_model);
        elmo_head.object->render();

        shaderPrograms[shaderProgramIndex]->set_uniform_value("objectColor", glm::vec3(0.0 / 255.0, 0.0 / 255.0, 0.0 / 255.0));
        shaderPrograms[shaderProgramIndex]->set_uniform_value("model", elmo_mouth_up_model);
        elmo_head.object->render();

        shaderPrograms[shaderProgramIndex]->set_uniform_value("objectColor", glm::vec3(0.0 / 255.0, 0.0 / 255.0, 0.0 / 255.0));
        shaderPrograms[shaderProgramIndex]->set_uniform_value("model", elmo_mouth_down_model);
        elmo_head.object->render();

        shaderPrograms[shaderProgramIndex]->release();
    }

    // TODO 4-2
    // Rendering cubemap environment
    // Hint:
    // 1. All the needed things are already set up in cubemap_setup() function.
    // 2. You can use the vertices in cubemapVertices provided in the header/cube.h
    // 3. You need to set the view, projection matrix.
    // 4. Use the cubemapShader to render the cubemap
    //    (refer to the above code to get an idea of how to use the shader program)

    glm::mat4 cubemap_view = glm::mat4(glm::mat3(view));
    cubemapShader->use();
    cubemapShader->set_uniform_value("view", cubemap_view);
    cubemapShader->set_uniform_value("projection", projection);

    glDepthFunc(GL_LEQUAL);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glBindVertexArray(cubemapVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

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
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Group 8 elmo song", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
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

    // set viewport
    glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_HEIGHT);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Setup texture, model, shader ...e.t.c
    setup();

    // Render loop, main logic can be found in update, render function
    while (!glfwWindowShouldClose(window))
    {
        update();
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// Add key callback
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{

    // The action is one of GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE.
    // Events with GLFW_PRESS and GLFW_RELEASE actions are emitted for every key press.
    // Most keys will also emit events with GLFW_REPEAT actions while a key is held down.
    // https://www.glfw.org/docs/3.3/input_guide.html

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // shader program selection
    if (key == GLFW_KEY_0 && (action == GLFW_REPEAT || action == GLFW_PRESS))
        shaderProgramIndex = 0;
    if (key == GLFW_KEY_1 && (action == GLFW_REPEAT || action == GLFW_PRESS))
        shaderProgramIndex = 1;
    if (key == GLFW_KEY_2 && (action == GLFW_REPEAT || action == GLFW_PRESS))
        shaderProgramIndex = 2;
    if (key == GLFW_KEY_3 && (action == GLFW_REPEAT || action == GLFW_PRESS))
        shaderProgramIndex = 3;
    if (key == GLFW_KEY_4 && (action == GLFW_REPEAT || action == GLFW_PRESS))
        shaderProgramIndex = 4;
    if (key == GLFW_KEY_5 && (action == GLFW_REPEAT || action == GLFW_PRESS))
        shaderProgramIndex = 5;

    // camera movement
    float cameraSpeed = 0.5f;
    if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
        camera.position.z -= 10.0;
    if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
        camera.position.z += 10.0;
    if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
        camera.rotationY -= 10.0;
    if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
        camera.rotationY += 10.0;

    // 控制Elmo是否長毛&毛的長度
    if (key == GLFW_KEY_H && (action == GLFW_REPEAT || action == GLFW_PRESS))
        hasHair *= -1;
    if (key == GLFW_KEY_L && (action == GLFW_REPEAT || action == GLFW_PRESS))
        hair_length += 0.2;
    if (key == GLFW_KEY_K && (action == GLFW_REPEAT || action == GLFW_PRESS))
        if (hair_length >= 1.2)
            hair_length -= 0.2;

    // 控制elmo手和嘴巴動的速度
    if (key == GLFW_KEY_M && (action == GLFW_REPEAT || action == GLFW_PRESS))
        move_k += 0.2;
    if (key == GLFW_KEY_N && (action == GLFW_REPEAT || action == GLFW_PRESS))
        move_k -= 0.2;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

// Loading cubemap texture
unsigned int loadCubemap(vector<std::string> &faces)
{

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        stbi_set_flip_vertically_on_load(false);
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texture;
}

/*
NOTE
- bling phong: AC
- gouraud: AC
- metallic: AC
- glass schlick: AC
- glass empirical: AC

前奏 轉正面
1. 長毛 停
2. 轉90 (側面)
3. 轉90 (背面) 進去手
4. 手 離開手
5. 轉90
6. 長毛
7. 縮毛
8. 快
*/