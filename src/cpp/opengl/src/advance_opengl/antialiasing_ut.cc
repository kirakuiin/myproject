// antialiasing_ut.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jun 24
// License: GPL.v3

#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <time.h>
#include <gtest/gtest.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include "include/commondef.h"
#include "include/shader.h"
#include "include/camera.h"
#include "include/frame.h"
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

static gl::Camera camera(glm::vec3(15, 0, 40));

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

class ANAUT : public testing::Test {
  public:
    static void SetUpTestCase();
    static void TearDownTestCase() {}
    void SetUp() override {}
    void TearDown() override {}
};

void ANAUT::SetUpTestCase() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER,GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

}

TEST_F(ANAUT, HelloAntiAliasing) {
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

    float vertices[] = {
        0, 0, 0, 1, 0, 0,
        1, 0, 0, 0, 1, 0,
        0.5, 0, -0.866, 0, 0, 1,
        0.5, 0.8164, -0.2886, 1, 1, 0,
    };
    float tex_v[] = {
        -1, -1, 0, 0, 0,
        1, -1, 0, 1, 0,
        1, 1, 0, 1, 1,
        1, 1, 0, 1, 1,
        -1, 1, 0, 0, 1,
        -1, -1, 0, 0, 0,
    };

    int indexs[] = {
        0, 3, 1,
        0, 3, 2,
        1, 3, 2,
        0, 2, 1,
    };
    // set vertex
    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glBindVertexArray(0);

    unsigned int fvao, fvbo;
    glGenVertexArrays(1, &fvao);
    glGenBuffers(1, &fvbo);
    glBindVertexArray(fvao);
    glBindBuffer(GL_ARRAY_BUFFER, fvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tex_v), tex_v, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glBindVertexArray(0);

    // set shader
    std::vector<gl::Shader*> shaders;
    gl::VertexShader v_shader("shaders/antialiasing/shader.vert");
    gl::FragmentShader f_shader("shaders/antialiasing/shader.frag");
    shaders.push_back(&v_shader);
    shaders.push_back(&f_shader);
    gl::ShaderProgram program(shaders);
    shaders.clear();

    gl::VertexShader vf_shader("shaders/antialiasing/fshader.vert");
    gl::FragmentShader ff_shader("shaders/antialiasing/fshader.frag");
    shaders.push_back(&vf_shader);
    shaders.push_back(&ff_shader);
    gl::ShaderProgram fprogram(shaders);
    shaders.clear();

    // enable something
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    // set framebuffer
    gl::Framebuffer mfb;
    std::shared_ptr<gl::ColorAttachment> ca(
        new gl::MultisampleColorAttachment(gldef::SCR_WIDTH, gldef::SCR_WIDTH, 4)
    );
    std::shared_ptr<gl::DepthStencilAttachment> da(
        new gl::MultisampleDepthStencilAttachment(gldef::SCR_WIDTH, gldef::SCR_WIDTH, 4)
    );
    mfb.Attach(ca, da);
    gl::Framebuffer fb;
    ca.reset(new gl::TextureColorAttachment(gldef::SCR_WIDTH, gldef::SCR_HEIGHT));
    da.reset(new gl::TextureDepthStencilAttachment(gldef::SCR_WIDTH, gldef::SCR_HEIGHT));
    fb.Attach(ca, da);

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
        glBindFramebuffer(GL_FRAMEBUFFER, mfb.id);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        mat4 view = camera.GetViewMatrix();
        mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)gldef::SCR_WIDTH / gldef::SCR_HEIGHT, 0.1f, 100.0f);
        mat4 model(1);
        model = glm::scale(model, vec3(30, 30, 30));

        program.Use();
        program.SetUniform("projection", projection);
        program.SetUniform("view", view);
        program.SetUniform("model", model);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

        // transform bit
        glBindFramebuffer(GL_READ_FRAMEBUFFER, mfb.id);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb.id);
        glBlitFramebuffer(0, 0, gldef::SCR_WIDTH, gldef::SCR_HEIGHT,
                          0, 0, gldef::SCR_WIDTH, gldef::SCR_HEIGHT,
                          GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1, 1, 1, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        fprogram.Use();
        glBindVertexArray(fvao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fb.color->id);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

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
