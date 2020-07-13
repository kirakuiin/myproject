// normalmap_ut.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jul 13
// License: GPL.v3

#include <iostream>
#include <vector>
#include <map>
#include <gtest/gtest.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include "include/commondef.h"
#include "include/shader.h"
#include "include/frame.h"
#include "include/camera.h"
#include "include/model.h"
#include "include/texture.h"
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"

using std::cout;
using std::endl;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;

static gl::Camera camera(glm::vec3(0, 0, 5));

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
inline void processInput(GLFWwindow *window, float delta)
{
    static float speed = camera.movement_speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.movement_speed = 5*speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        camera.movement_speed = speed;
    }
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(gl::CAMERA_DIRECT::FORWARD, delta);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(gl::CAMERA_DIRECT::BACKWARD, delta);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(gl::CAMERA_DIRECT::LEFT, delta);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(gl::CAMERA_DIRECT::RIGHT, delta);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
inline void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse move. this callback function execute
static void mouse_callback(GLFWwindow*, double x, double y)
{
    static bool first_move = true;
    static float last_x, last_y;
    if (first_move) {
        first_move = false;
    } else {
        float x_offset = x - last_x;
        float y_offset = last_y - y;
        camera.ProcessMouse(x_offset, y_offset);
    }
    last_x = x;
    last_y = y;
}

static void scroll_callback(GLFWwindow* window, double x, double y)
{
    camera.ProcessScroll(y);
}

static void RenderScene(unsigned int vao, gl::ShaderProgram& pro)
{
    glBindVertexArray(vao);
    mat4 view = camera.GetViewMatrix();
    mat4 projection = glm::perspective(
        glm::radians(45.0f),
        (float)gldef::SCR_WIDTH/gldef::SCR_HEIGHT, 1.0f, 75.0f
    );
    pro.Use();
    pro.SetUniform("view", view);
    pro.SetUniform("projection", projection);
    pro.SetUniform("view_pos", camera.position);
    // draw brick wall
    mat4 model(1);
    model = glm::scale(model, vec3(5));
    float t = glfwGetTime();
    model = glm::rotate(model, glm::radians(t), vec3(0, 1, 0));
    pro.SetUniform("islight", 0);
    pro.SetUniform("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // draw light
    model = mat4(1);

    // set light
    vec3 light_pos(6,0,6);
    pro.SetUniform("light_pos", light_pos);

    model = glm::translate(model, light_pos);
    model = glm::scale(model, vec3(0.5));
    pro.SetUniform("model", model);
    pro.SetUniform("islight", 1);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

class NMMUT : public testing::Test {
  public:
    static void SetUpTestCase();
    static void TearDownTestCase() {}
    void SetUp() override {}
    void TearDown() override {}
};

void NMMUT::SetUpTestCase() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER,GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES ,4);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

}

TEST_F(NMMUT, HelloNormalMap) {
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(gldef::SCR_WIDTH, gldef::SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    stbi_set_flip_vertically_on_load(true);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
    }

    // calc tangent
    /*
     *  [tx, ty, tz]             1           [ dv2, -dv1 ]   [e1x, e1y, e1z]
     *  [          ]  =  ----------------- * [           ] * [             ]
     *  [bx, by, bz]     du1*dv2 - du2*dv1   [ -du2, du1 ]   [e2x, e2y, e2z]
     */
    vec3 pos1(-1.0f, 1.0f, 0.0f);
    vec3 pos2(-1.0f, -1.0f, 0.0f);
    vec3 pos3(1.0f, -1.0f, 0.0f);
    vec3 pos4(1.0f, 1.0f, 0.0f);
    vec2 uv1(0.0f, 1.0f);
    vec2 uv2(0.0f, 0.0f);
    vec2 uv3(1.0f, 0.0f);
    vec2 uv4(1.0f, 1.0f);
    vec3 nm(0.0f, 0.0f, 1.0f);

    vec3 edge1 = pos2 - pos1;
    vec3 edge2 = pos3 - pos1;
    vec2 deltauv1 = uv2 - uv1;
    vec2 deltauv2 = uv3 - uv1;

    float f = 1.0f/(deltauv1.x*deltauv2.y - deltauv2.x*deltauv1.y);
    vec3 tangent1;
    tangent1.x = f * (deltauv2.y * edge1.x - deltauv1.y * edge2.x);
    tangent1.y = f * (deltauv2.y * edge1.y - deltauv1.y * edge2.y);
    tangent1.y = f * (deltauv2.y * edge1.z - deltauv1.y * edge2.z);
    tangent1 = glm::normalize(tangent1);
    vec3 bitangent1;
    bitangent1.x = f * (deltauv1.x * edge2.x - deltauv2.x * edge1.x);
    bitangent1.y = f * (deltauv1.x * edge2.y - deltauv2.x * edge1.y);
    bitangent1.z = f * (deltauv1.x * edge2.z - deltauv2.x * edge1.z);
    bitangent1 = glm::normalize(bitangent1);

    vec3 edge3 = pos3 - pos1;
    vec3 edge4 = pos4 - pos1;
    vec2 deltauv3 = uv3 - uv1;
    vec2 deltauv4 = uv4 - uv1;

    f = 1.0f/(deltauv3.x*deltauv4.y - deltauv4.x*deltauv3.y);
    vec3 tangent2;
    tangent2.x = f * (deltauv4.y * edge3.x - deltauv3.y * edge4.x);
    tangent2.y = f * (deltauv4.y * edge3.y - deltauv3.y * edge4.y);
    tangent2.y = f * (deltauv4.y * edge3.z - deltauv3.y * edge4.z);
    tangent2 = glm::normalize(tangent2);
    vec3 bitangent2;
    bitangent2.x = f * (deltauv3.x * edge4.x - deltauv4.x * edge3.x);
    bitangent2.y = f * (deltauv3.x * edge4.y - deltauv4.x * edge3.y);
    bitangent2.z = f * (deltauv3.x * edge4.z - deltauv4.x * edge3.z);
    bitangent2 = glm::normalize(bitangent2);
    vec3 tangent = tangent1;
    vec3 bitangent = bitangent1;

    // set vertex
    float brick_vertices[] = {
        // positions //normal // texture Coords // tangent // bitangent
        -1, -1,  0,  nm.x, nm.y, nm.z, 0.0f, 0.0f,
        tangent.x, tangent.y, tangent.z, bitangent.x, bitangent.y, bitangent.z,
         1, -1,  0,  nm.x, nm.y, nm.z, 1.0f, 0.0f,
        tangent.x, tangent.y, tangent.z, bitangent.x, bitangent.y, bitangent.z,
         1,  1,  0,  nm.x, nm.y, nm.z, 1.0f, 1.0f,
        tangent.x, tangent.y, tangent.z, bitangent.x, bitangent.y, bitangent.z,
         1,  1,  0,  nm.x, nm.y, nm.z, 1.0f, 1.0f,
        tangent.x, tangent.y, tangent.z, bitangent.x, bitangent.y, bitangent.z,
        -1,  1,  0,  nm.x, nm.y, nm.z, 0.0f, 1.0f,
        tangent.x, tangent.y, tangent.z, bitangent.x, bitangent.y, bitangent.z,
        -1, -1,  0,  nm.x, nm.y, nm.z, 0.0f, 0.0f,
        tangent.x, tangent.y, tangent.z, bitangent.x, bitangent.y, bitangent.z,
    };

    unsigned vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(brick_vertices), brick_vertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14*sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14*sizeof(float),
                          (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14*sizeof(float),
                          (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14*sizeof(float),
                          (void*)(8*sizeof(float)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14*sizeof(float),
                          (void*)(11*sizeof(float)));

    // set shader
    gl::VertexShader v_shader("shaders/normalmap/shader.vert");
    gl::FragmentShader f_shader("shaders/normalmap/shader.frag");
    std::vector<gl::Shader*> shaders;
    shaders.push_back(&v_shader);
    shaders.push_back(&f_shader);
    gl::ShaderProgram program(shaders);
    for (auto& i: shaders) {
        i->Delete();
    }
    shaders.clear();

    // load textures
    // -------------
    gl::Texture2D brick;
    brick.LoadImage("images/texture/brickwall.jpg");
    gl::Texture2D normal;
    normal.LoadImage("images/texture/brickwall_normal.jpg");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, brick.texture);
    // stbi_set_flip_vertically_on_load(false);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normal.texture);
    program.Use();
    program.SetUniform("brick", 0);
    program.SetUniform("normal", 1);

    // enable something
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    // bind frame

    // render loop
    // -----------
    float deltaTime = 0;
    float lastFrame = 0;
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window, deltaTime);

        // render
        // ------
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        RenderScene(vao, program);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

