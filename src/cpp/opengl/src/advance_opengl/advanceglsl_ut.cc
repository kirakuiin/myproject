// advanceglsl_ut.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jun 18
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
#include "include/camera.h"
#include "include/model.h"
#include "include/texture.h"
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"

using std::cout;
using std::endl;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;

static gl::Camera camera(glm::vec3(0, 0, 4));

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

class AGLUT : public testing::Test {
  public:
    static void SetUpTestCase();
    static void TearDownTestCase() {}
    void SetUp() override {}
    void TearDown() override {}
};

void AGLUT::SetUpTestCase() {
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

TEST_F(AGLUT, HelloAdvanceGlsl) {
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
    // stbi_set_flip_vertically_on_load(true);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
    }

    float cube_vertices[] = {
        // back
        -1, 1, -1,
        -1, -1, -1,
        1, -1, -1,
        1, -1, -1,
        1, 1, -1,
        -1, 1, -1,
        // front
        -1, 1, 1,
        1, 1, 1,
        1, -1, 1,
        1, -1, 1,
        -1, -1, 1,
        -1, 1, 1,
        // left
        -1, 1, 1,
        -1, -1, 1,
        -1, -1, -1,
        -1, -1, -1,
        -1, 1, -1,
        -1, 1, 1,
        // right
        1, 1, 1,
        1, 1, -1,
        1, -1, -1,
        1, -1, -1,
        1, -1, 1,
        1, 1, 1,
        // bottom
        -1, -1, -1,
        -1, -1, 1,
        1, -1, 1,
        1, -1, 1,
        1, -1, -1,
        -1, -1, -1,
        // top
        -1, 1, -1,
        1, 1, -1,
        1, 1, 1,
        1, 1, 1,
        -1, 1, 1,
        -1, 1, -1,
    };

    // cube VAO
    unsigned int c_vao, c_vbo;
    glGenVertexArrays(1, &c_vao);
    glGenBuffers(1, &c_vbo);
    glBindVertexArray(c_vao);
    glBindBuffer(GL_ARRAY_BUFFER, c_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), &cube_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // set shader
    std::vector<gl::Shader*> shaders;
    gl::VertexShader rv_shader("shaders/advanceglsl/cube.vert");
    gl::FragmentShader rf_shader("shaders/advanceglsl/red.frag");
    shaders.push_back(&rv_shader);
    shaders.push_back(&rf_shader);
    gl::ShaderProgram red(shaders);
    shaders.clear();

    gl::VertexShader gv_shader("shaders/advanceglsl/cube.vert");
    gl::FragmentShader gf_shader("shaders/advanceglsl/green.frag");
    shaders.push_back(&gv_shader);
    shaders.push_back(&gf_shader);
    gl::ShaderProgram green(shaders);
    shaders.clear();

    gl::VertexShader bv_shader("shaders/advanceglsl/cube.vert");
    gl::FragmentShader bf_shader("shaders/advanceglsl/blue.frag");
    shaders.push_back(&bv_shader);
    shaders.push_back(&bf_shader);
    gl::ShaderProgram blue(shaders);

    // set uniform buffer
    int index = 0;
    gl::BindProgramBlockToIndex(red, "Matrices", index);
    gl::BindProgramBlockToIndex(blue, "Matrices", index);
    gl::BindProgramBlockToIndex(green, "Matrices", index);
    auto ubo = gl::BindUniformBufferToIndex(sizeof(mat4)*2, index);

    // enable something
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

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
        glClearColor(0.5, 0.5, 0.5, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 view = camera.GetViewMatrix();
        mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)gldef::SCR_WIDTH / gldef::SCR_HEIGHT, 0.1f, 100.0f);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), glm::value_ptr(projection));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindVertexArray(c_vao);
        mat4 model(1);
        red.Use();
        model = glm::translate(model, vec3(0, 0.5, 0));
        model = glm::scale(model, vec3(0.2));
        red.SetUniform("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        blue.Use();
        model = mat4(1.0);
        model = glm::translate(model, vec3(-0.5, -0.5, 0));
        model = glm::scale(model, vec3(0.2));
        blue.SetUniform("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        green.Use();
        model = mat4(1.0);
        model = glm::translate(model, vec3(0.5, -0.5, 0));
        model = glm::scale(model, vec3(0.2));
        green.SetUniform("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &c_vao);
    glDeleteBuffers(1, &c_vbo);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}
