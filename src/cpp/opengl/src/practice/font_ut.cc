// font_ut.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jul 26
// License: GPL.v3

#include <iostream>
#include <random>
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
#include "include/font.h"

using std::cout;
using std::endl;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;

static gl::Camera camera(glm::vec3(0, 10, 10));

static std::vector<vec3> light_posis;
static std::vector<vec3> light_colors;

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

class FOTUT : public testing::Test {
  public:
    static void SetUpTestCase();
    static void TearDownTestCase() {}
    void SetUp() override {}
    void TearDown() override {}
};

void FOTUT::SetUpTestCase() {
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

TEST_F(FOTUT, HelloFont) {
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

    // enable something
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);
    // glEnable(GL_MULTISAMPLE);
    gl::Font menlo(gldef::SCR_WIDTH, gldef::SCR_HEIGHT);
    menlo.Load("images/fonts/NewYork.ttf");

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
        // 几何计算
        glClearColor(0, 1, 1, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        menlo.Draw("hello world!", vec2(250, 200));
        menlo.Draw("hello world!", vec2(250, 300));
        menlo.Draw("hello world!", vec2(250, 400));

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

