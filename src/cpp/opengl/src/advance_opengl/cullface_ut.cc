// cullface_ut.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jun 17
// License: GPL.v3

#include <iostream>
#include <vector>
#include <map>
#include <gtest/gtest.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "include/commondef.h"
#include "include/shader.h"
#include "include/camera.h"
#include "include/texture.h"
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "include/model.h"

using std::cout;
using std::endl;
using glm::vec3;
using glm::vec4;
using glm::mat4;

static gl::Camera camera(glm::vec3(0, -0.5, 2));

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
inline void processInput(GLFWwindow *window, float delta)
{
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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

class CUFUT : public testing::Test {
  public:
    static void SetUpTestCase();
    static void TearDownTestCase() {}
    void SetUp() override {}
    void TearDown() override {}
};

void CUFUT::SetUpTestCase() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER,GL_TRUE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

}

TEST_F(CUFUT, HelloCullFace) {
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

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
    }

    glEnable(GL_DEPTH_TEST);
    // glDepthMask(GL_FALSE);
    glDepthFunc(GL_LESS);

    /* 设置六方体坐标的心得:
           六方体有六面, 我们每次设置只考虑相对的两面,
           假设我们站在前面, 由于默认是逆时针序为正面, 因此设置前面为逆时针序,
           这样opengl就会正面显示出来, 然后将后面设置为顺序即可。
     */
    float cube_vertices[] = {
        // back
        -0.5, 0.5, -0.5, 0, 1,
        -0.5, -0.5, -0.5, 0, 0,
        0.5, -0.5, -0.5, 1, 0,
        0.5, -0.5, -0.5, 1, 0,
        0.5, 0.5, -0.5, 1, 1,
        -0.5, 0.5, -0.5, 0, 1,
        // front
        -0.5, 0.5, 0.5, 1, 1,
        0.5, 0.5, 0.5, 0, 1,
        0.5, -0.5, 0.5, 0, 0,
        0.5, -0.5, 0.5, 0, 0,
        -0.5, -0.5, 0.5, 1, 0,
        -0.5, 0.5, 0.5, 1, 1,
        // left
        -0.5, 0.5, 0.5, 0, 1,
        -0.5, -0.5, 0.5, 0, 0,
        -0.5, -0.5, -0.5, 1, 0,
        -0.5, -0.5, -0.5, 1, 0,
        -0.5, 0.5, -0.5, 1, 1,
        -0.5, 0.5, 0.5, 0, 1,
        // right
        0.5, 0.5, 0.5, 1, 1,
        0.5, 0.5, -0.5, 0, 1,
        0.5, -0.5, -0.5, 0, 0,
        0.5, -0.5, -0.5, 0, 0,
        0.5, -0.5, 0.5, 1, 0,
        0.5, 0.5, 0.5, 1, 1,
        // bottom
        -0.5, -0.5, -0.5, 0, 1,
        -0.5, -0.5, 0.5, 0, 0,
        0.5, -0.5, 0.5, 1, 0,
        0.5, -0.5, 0.5, 1, 0,
        0.5, -0.5, -0.5, 1, 1,
        -0.5, -0.5, -0.5, 0, 1,
        // top
        -0.5, 0.5, -0.5, 1, 1,
        0.5, 0.5, -0.5, 0, 1,
        0.5, 0.5, 0.5, 0, 0,
        0.5, 0.5, 0.5, 0, 0,
        -0.5, 0.5, 0.5, 1, 0,
        -0.5, 0.5, -0.5, 1, 1,
    };
    float plane_vertices[] = {
        -5.0f, -0.5f,  -5.0f,  0.0f, 2.0f,
        -5.0f, -0.5f, 5.0f,  0.0f, 0.0f,
        5.0f, -0.5f, 5.0f,  2.0f, 0.0f,
        5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        5.0f, -0.5f,  -5.0f,  2.0f, 2.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f
    };

    // cube VAO
    unsigned int c_vao, c_vbo;
    glGenVertexArrays(1, &c_vao);
    glGenBuffers(1, &c_vbo);
    glBindVertexArray(c_vao);
    glBindBuffer(GL_ARRAY_BUFFER, c_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), &cube_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    // plane VAO
    unsigned int p_vao, p_vbo;
    glGenVertexArrays(1, &p_vao);
    glGenBuffers(1, &p_vbo);
    glBindVertexArray(p_vao);
    glBindBuffer(GL_ARRAY_BUFFER, p_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), &plane_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    // set shader
    gl::VertexShader v_shader("shaders/cullface/shader.vert");
    gl::FragmentShader f_shader("shaders/cullface/shader.frag");
    std::vector<gl::Shader*> shaders;
    shaders.push_back(&v_shader);
    shaders.push_back(&f_shader);
    gl::ShaderProgram program(shaders);
    for (auto& i: shaders) {
        i->Delete();
    }
    // load textures
    // -------------
    gl::Texture2D t_cube;
    t_cube.LoadImage("images/texture/awesomeface.png");
    gl::Texture2D t_plane;
    t_plane.LoadImage("images/texture/container.jpg");

    // shader configuration
    // --------------------
    program.Use();
    program.SetUniform("tex", 0);


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

        // enable cull face
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program.Use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)gldef::SCR_WIDTH / gldef::SCR_HEIGHT, 0.1f, 100.0f);
        program.SetUniform("view", view);
        program.SetUniform("projection", projection);
        // cubes
        glBindVertexArray(c_vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, t_cube.texture);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        program.SetUniform("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        program.SetUniform("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // floor
        glDisable(GL_CULL_FACE);
        glBindVertexArray(p_vao);
        glBindTexture(GL_TEXTURE_2D, t_plane.texture);
        program.SetUniform("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}
