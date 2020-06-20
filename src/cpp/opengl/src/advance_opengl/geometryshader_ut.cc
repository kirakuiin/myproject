// geometryshader_ut.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jun 20
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

class GSHUT : public testing::Test {
  public:
    static void SetUpTestCase();
    static void TearDownTestCase() {}
    void SetUp() override {}
    void TearDown() override {}
};

void GSHUT::SetUpTestCase() {
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

TEST_F(GSHUT, HelloGeometryShader) {
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

    float point_vertices[] = {
        // back
        -1, 1, -1, 0, 0, 1, 0, 0, -1,
        -1, -1, -1, 0, 0, 1, 0, 0, -1,
        1, -1, -1, 0, 0, 1, 0, 0, -1,
        1, -1, -1, 0, 0, 1, 0, 0, -1,
        1, 1, -1, 0, 0, 1, 0, 0, -1,
        -1, 1, -1, 0, 0, 1, 0, 0, -1,
        // front
        -1, 1, 1, 1, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 0, 0, 0, 0, 1,
        1, -1, 1, 1, 0, 0, 0, 0, 1,
        1, -1, 1, 1, 0, 0, 0, 0, 1,
        -1, -1, 1, 1, 0, 0, 0, 0, 1,
        -1, 1, 1, 1, 0, 0, 0, 0, 1,
        // left
        -1, 1, 1, 1, 0, 0, -1, 0, 0,
        -1, -1, 1, 1, 0, 0, -1, 0, 0,
        -1, -1, -1, 1, 0, 0, -1, 0, 0,
        -1, -1, -1, 1, 0, 0, -1, 0, 0,
        -1, 1, -1, 1, 0, 0, -1, 0, 0,
        -1, 1, 1, 1, 0, 0, -1, 0, 0,
        // right
        1, 1, 1, 0, 0, 1, 1, 0, 0,
        1, 1, -1, 0, 0, 1, 1, 0, 0,
        1, -1, -1, 0, 0, 1, 1, 0, 0,
        1, -1, -1, 0, 0, 1, 1, 0, 0,
        1, -1, 1, 0, 0, 1, 1, 0, 0,
        1, 1, 1, 0, 0, 1, 1, 0, 0,
        // bottom
        -1, -1, -1, 0, 1, 0, 0, -1, 0,
        -1, -1, 1, 0, 1, 0, 0, -1, 0,
        1, -1, 1, 0, 1, 0, 0, -1, 0,
        1, -1, 1, 0, 1, 0, 0, -1, 0,
        1, -1, -1, 0, 1, 0, 0, -1, 0,
        -1, -1, -1, 0, 1, 0, 0, -1, 0,
        // top
        -1, 1, -1, 0, 1, 0, 0, 1, 0,
        1, 1, -1, 0, 1, 0, 0, 1, 0,
        1, 1, 1, 0, 1, 0, 0, 1, 0,
        1, 1, 1, 0, 1, 0, 0, 1, 0,
        -1, 1, 1, 0, 1, 0, 0, 1, 0,
        -1, 1, -1, 0, 1, 0, 0, 1, 0,
    };

    // cube VAO
    unsigned int p_vao, p_vbo;
    glGenVertexArrays(1, &p_vao);
    glGenBuffers(1, &p_vbo);
    glBindVertexArray(p_vao);
    glBindBuffer(GL_ARRAY_BUFFER, p_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point_vertices), &point_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                          (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                          (void*)(6*sizeof(float)));

    // set shader
    std::vector<gl::Shader*> shaders;
    gl::VertexShader v_shader("shaders/geometryshader/shader.vert");
    gl::FragmentShader f_shader("shaders/geometryshader/shader.frag");
    gl::GeometryShader g_shader("shaders/geometryshader/shader.geom");
    shaders.push_back(&g_shader);
    shaders.push_back(&v_shader);
    shaders.push_back(&f_shader);
    gl::ShaderProgram program(shaders);
    shaders.clear();

    // enable something
    glEnable(GL_DEPTH_TEST);

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
        mat4 model(1.0);

        program.Use();
        program.SetUniform("projection", projection);
        program.SetUniform("view", view);
        program.SetUniform("model", model);
        glBindVertexArray(p_vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &p_vao);
    glDeleteBuffers(1, &p_vbo);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}
