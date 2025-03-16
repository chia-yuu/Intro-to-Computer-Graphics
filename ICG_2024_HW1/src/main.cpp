#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <bits/stdc++.h>

#include "./header/Shader.h"
#include "./header/Object.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void drawModel(const string& name, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, int r, int g, int b);
void init();

// settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

// Shader
Shader *shader;

// Objects to display
Object *cube, *cylinder, *I, *C, *G, *tree_up, *tree_btn;

// Helicopter position
int h_x = 0, h_y = 6;

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ICG_2024_HW1", NULL, NULL);
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
    glDepthFunc(GL_LESS);

    // Display loop
	glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_HEIGHT);
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Initialize Object and Shader
    init();

    // render loop
    float angle = 0.0f, h_angle = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        // render
        glClearColor(153/255.0, 204/255.0, 255/255.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Use shader
        shader->use();
        
        
        /*=================== Example of creating model matrix ======================= 
        1. translate
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 1.0f, 0.0f));
        drawModel("Cube",model,view,projection,255, 0, 0);
        
        2. scale
        glm::mat4 model(1.0f);
        model = glm::scale(model, glm::vec3(0.5f, 1.0f, 2.0f)); 
        drawModel("Cube",model,view,projection,255, 0, 0);
        
        3. rotate
        glm::mat4 model(1.0f);
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        drawModel("Cube",model,view,projection,255, 0, 0);
        ==============================================================================*/

        // TODO: Create model, view, and perspective matrix
        glm::mat4 model(1.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 50.0f, 90.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

        // TODO: Helicopter (Body + Connector + Rotor Blades)
        // Body (At least 3 different color cubes)
        glm::mat4 h_body1(1.0f);
        // h_body1 = glm::mat4(1.0f);
        h_body1 = glm::translate(h_body1, glm::vec3(h_x, h_y, 0));
        h_body1 = glm::scale(h_body1, glm::vec3(20, 10, 20));
        drawModel("Cube", h_body1, view, projection, 255, 215, 0);
        glm::mat4 h_body2;
        h_body2 = glm::translate(h_body1, glm::vec3(0.5f, 0.0f, 0.0f));
        h_body2 = glm::scale(h_body2, glm::vec3(0.8f, 0.8f, 0.8f));
        drawModel("Cube", h_body2, view, projection, 255, 222, 173);
        glm::mat4 h_body3;
        h_body3 = glm::translate(h_body1, glm::vec3(1.0f, 0.0f, 0.0f));
        h_body3 = glm::scale(h_body3, glm::vec3(0.3f, 0.6f, 0.6f));
        drawModel("Cube", h_body3, view, projection, 240, 230, 140);

        // Connector (One cube)
        //     - On the body
        //     - Rotate: 5 degrees/frame about +y axis
        glm::mat4 connection = h_body1;
        connection = glm::rotate(connection, h_angle, glm::vec3(0, 1, 0));
        connection = glm::translate(connection, glm::vec3(0, 0.5, 0));
        connection = glm::scale(connection, glm::vec3(0.3, 0.3, 0.3));
        drawModel("Cube", connection, view, projection, 70, 130, 180);
        h_angle += glm::radians(5.0f);

        // Rotor Blades (4 different color cubes)
        //     - On the connector
        //     - Each cubes is offset by 90 degrees
        // You can be creative in design !!
        glm::mat4 blade1, blade2, blade3, blade4;
        blade1 = glm::translate(connection, glm::vec3(1.5, 0.3, 0));
        blade1 = glm::scale(blade1, glm::vec3(3, 0.1, 0.7));
        drawModel("Cube", blade1, view, projection, 100, 149, 237);

        blade2 = glm::translate(connection, glm::vec3(0, 0.3, 1.5));
        blade2 = glm::rotate(blade2, glm::radians(90.0f), glm::vec3(0, 1, 0));
        blade2 = glm::scale(blade2, glm::vec3(3, 0.1, 0.7));
        drawModel("Cube", blade2, view, projection, 147, 112, 219);

        blade3 = glm::translate(connection, glm::vec3(-1.5, 0.3, 0));
        blade3 = glm::rotate(blade3, glm::radians(180.0f), glm::vec3(0, 1, 0));
        blade3 = glm::scale(blade3, glm::vec3(3, 0.1, 0.7));
        drawModel("Cube", blade3, view, projection, 106, 90, 205);

        blade4 = glm::translate(connection, glm::vec3(0, 0.3, -1.5));
        blade4 = glm::rotate(blade4, glm::radians(90.0f), glm::vec3(0, 1, 0));
        blade4 = glm::scale(blade4, glm::vec3(3, 0.1, 0.7));
        drawModel("Cube", blade4, view, projection, 139, 0, 139);

        // TODO: Draw Ground (Scale: (100, 1, 120))
        glm::mat4 ground;
        ground = glm::scale(model, glm::vec3(100, 1, 120));
        drawModel("Cube", ground, view, projection, 144, 238, 144);

        // TODO: Draw Tree1 (Position: (-30, 0, -20) relative to the Ground, Scale: (4, 4, 4))
        glm::mat4 tree1_up, tree1_btn;
        tree1_up = glm::translate(model, glm::vec3(-30, 0, -20));
        tree1_up = glm::scale(tree1_up, glm::vec3(4, 4, 4));
        drawModel("Tree_up", tree1_up, view, projection, 0, 100, 0);
        tree1_btn = glm::translate(model, glm::vec3(-30, 0, -20));
        tree1_btn = glm::scale(tree1_btn, glm::vec3(4, 4, 4));
        drawModel("Tree_down", tree1_btn, view, projection, 128, 0, 0);

        // TODO: Draw Tree2 (Position: (30, 0, 20) relative to the Ground, Scale: (3.5, 3.5, 3.5))
        glm::mat4 tree2_up, tree2_btn;
        tree2_up = glm::translate(model, glm::vec3(30, 0, 20));
        tree2_up = glm::scale(tree2_up, glm::vec3(3.5, 3.5, 3.5));
        drawModel("Tree_up", tree2_up, view, projection, 0, 128, 0);
        tree2_btn = glm::translate(model, glm::vec3(30, 0, 20));
        tree2_btn = glm::scale(tree2_btn, glm::vec3(3.5, 3.5, 3.5));
        drawModel("Tree_down", tree2_btn, view, projection, 165, 42, 42);

        // TODO: Draw C (Position: (-25, 2, 25))
        glm::mat4 char_c(1.0f);
        char_c = glm::translate(char_c, glm::vec3(-25, 2, 25));
        drawModel("C", char_c, view, projection, 255, 182, 193);

        // TODO: Draw I (Position: (-12, 0, 0), Rotate: 0.8 degrees/ frame about +y axis around the C)
        glm::mat4 char_i = char_c;
        char_i = glm::rotate(char_i, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        char_i = glm::translate(char_i, glm::vec3(-12, 0, 0));
        drawModel("I", char_i, view, projection, 255, 182, 193);

        // TODO: Draw G (Position: (12, 0, 0), Rotate: 0.8 degrees/ frame about +y axis around the C)
        glm::mat4 char_g = char_c;
        char_g = glm::rotate(char_g, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        char_g = glm::translate(char_g, glm::vec3(12, 0, 0));
        drawModel("G", char_g, view, projection, 255, 182, 193);

        // TODO: Control rotation
        angle += glm::radians(0.8f);
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// TODO:
// Add key callback
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
        
    // The action is one of GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE. 
    // Events with GLFW_PRESS and GLFW_RELEASE actions are emitted for every key press.
    // Most keys will also emit events with GLFW_REPEAT actions while a key is held down.
    // https://www.glfw.org/docs/3.3/input_guide.html

    // 1. Press W to move the helicopter in the Y direction by 1
    // 2. Press S to move the helicopter in the Y direction by -1 
    // 3. Press D to move the helicopter in the X direction by 1
    // 4. Press A to move the helicopter in the X direction by -1
    // 5. The helicopter cannot fly below the ground.
    // ** Both GLFW_PRESS and GLFW_REPEAT are OK! You can try it out and compare the differences. **
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
    else if(key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        h_y += 1;
    }
    else if(key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        if(h_y > 6){h_y -= 1;}
    }
    else if(key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        h_x += 1;
    }
    else if(key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        h_x -= 1;
    }
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

void drawModel(const string& name, 
               const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, 
               int r, int g, int b){

    shader->set_uniform("projection",projection);
    shader->set_uniform("view",view);
    shader->set_uniform("model",model);
    shader->set_uniform("objectColor", glm::vec3(r/255.0,g/255.0,b/255.0));
    
    if(name == "Cube"){
        cube->draw();
    }else if(name == "Cylinder"){
        cylinder->draw();
    }else if(name == "I"){
        I->draw();
    }else if(name == "C"){
        C->draw();
    }else if(name == "G"){
        G->draw();
    }else if(name == "Tree_down"){
        tree_btn->draw();
    }else if(name == "Tree_up"){
        tree_up->draw();
    }
}

void init(){
    #if defined(__linux__) || defined(__APPLE__)
        string dirShader = "../../src/shaders/";
        string dirAsset = "../../src/asset/";
    #else
        string dirShader = "..\\..\\src\\shaders\\";
        string dirAsset = "..\\..\\src\\asset\\";
    #endif

    string v = dirShader + "easy.vert";
    string f = dirShader + "easy.frag";
    shader = new Shader(v.c_str(),f.c_str());
    cube = new Object(dirAsset + "cube.obj");
    cylinder = new Object(dirAsset + "cylinder.obj");
    I = new Object(dirAsset + "I.obj");
    C = new Object(dirAsset + "C.obj");
    G = new Object(dirAsset + "G.obj");
    tree_up = new Object(dirAsset + "tree_up.obj");
    tree_btn = new Object(dirAsset + "tree_btn.obj");
}