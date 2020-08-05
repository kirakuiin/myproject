// working_ut.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jul 23
// License: GPL.v3

#include <iostream>
#include <gtest/gtest.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "include/commondef.h"
#include "include/frame.h"
#include "include/glm/glm.hpp"
#include "game.h"
#include "gameobj.h"

using std::cout;
using std::endl;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;

// global variables
namespace {
Game breakout(gldef::SCR_WIDTH, gldef::SCR_HEIGHT);
} // namespace

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
inline void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                        int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            breakout.keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            breakout.keys[key] = false;
            breakout.keys_just_rel[key] = true;
        }
    }
}

class WOKUT : public testing::Test {
  public:
    static void SetUpTestCase();
    static void TearDownTestCase() {}
    void SetUp() override {}
    void TearDown() override {}
};

void WOKUT::SetUpTestCase() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER,GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

}

TEST_F(WOKUT, Hello) {
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(gldef::SCR_WIDTH, gldef::SCR_HEIGHT, "Breakout", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, KeyCallback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
    }

    // enable something
    glViewport(0, 0, gldef::SCR_WIDTH, gldef::SCR_HEIGHT);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    breakout.Init();

    // render loop
    // -----------
    float deltaTime = 0;
    float lastFrame = 0;
    while (!glfwWindowShouldClose(window)
           || breakout.state == GameState::GAME_EXIT) {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // input
        breakout.ProcessInput(deltaTime);

        breakout.Update(deltaTime);

        // render
        // ------
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        breakout.Render();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

