// hdr_ut.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jul 14
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

static gl::Camera camera(glm::vec3(0, 0, 0));

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
inline void processInput(GLFWwindow *window, float delta, gl::ShaderProgram& p)
{
    static float speed = camera.movement_speed;
    static float exposure = 1;
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
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        if (exposure >= 0.2) {
            exposure -= 0.1;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        exposure += 0.1;
    }
    p.Use();
    p.SetUniform("exposure", exposure);
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
        (float)gldef::SCR_WIDTH/gldef::SCR_HEIGHT, 0.1f, 75.0f
    );
    pro.Use();
    pro.SetUniform("view", view);
    pro.SetUniform("projection", projection);
    pro.SetUniform("view_pos", camera.position);
    // draw wall
    mat4 model(1);
    model = glm::translate(model, vec3(0, 0, -15));
    model = glm::scale(model, vec3(5, 5, 40));
    pro.SetUniform("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

class HDRUT : public testing::Test {
  public:
    static void SetUpTestCase();
    static void TearDownTestCase() {}
    void SetUp() override {}
    void TearDown() override {}
};

void HDRUT::SetUpTestCase() {
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

TEST_F(HDRUT, HelloHDR) {
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

    // set vertex
    float cube_vertices[] = {
        // positions        //normal  // texture Coords
        -0.5f, -0.5f, -0.5f, 0, 0, 1, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0, 0, 1, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0, 0, 1, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0, 0, 1, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0, 0, 1, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0, 0, 1, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 0, 0, -1, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0, 0, -1, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0, 0, -1, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0, 0, -1, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0, 0, -1, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0, 0, -1, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, 1, 0, 0, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1, 0, 0, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1, 0, 0, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1, 0, 0, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 1, 0, 0, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1, 0, 0, 1.0f, 1.0f,

        0.5f,  0.5f,  0.5f, -1, 0, 0, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f, -1, 0, 0, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, -1, 0, 0, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, -1, 0, 0, 0.0f, 0.0f,
        0.5f,  0.5f, -0.5f, -1, 0, 0, 0.0f, 1.0f,
        0.5f,  0.5f,  0.5f, -1, 0, 0, 1.0f, 1.0f,

        -0.5f, -0.5f, -0.5f, 0, 1, 0, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0, 1, 0, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0, 1, 0, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0, 1, 0, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0, 1, 0, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0, 1, 0, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, 0, -1, 0, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0, -1, 0, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0, -1, 0, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0, -1, 0, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0, -1, 0, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0, -1, 0, 0.0f, 1.0f
    };

    unsigned vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float),
                          (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float),
                          (void*)(6*sizeof(float)));

    float plane_vertices[] = {
        -1, 1, 0,  0, 1,
        -1, -1, 0, 0, 0,
        1, -1, 0,  1, 0,
        1, -1, 0,  1, 0,
        1, 1, 0,   1, 1,
        -1, 1, 0,  0, 1,
    };
    unsigned pvao, pvbo;
    glGenVertexArrays(1, &pvao);
    glGenBuffers(1, &pvbo);
    glBindVertexArray(pvao);
    glBindBuffer(GL_ARRAY_BUFFER, pvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), plane_vertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float),
                          (void*)(3*sizeof(float)));

    // set shader
    gl::VertexShader v_shader("shaders/hdr/hdr.vert");
    gl::FragmentShader f_shader("shaders/hdr/hdr.frag");
    std::vector<gl::Shader*> shaders;
    shaders.push_back(&v_shader);
    shaders.push_back(&f_shader);
    gl::ShaderProgram hp(shaders);
    for (auto& i: shaders) {
        i->Delete();
    }
    shaders.clear();

    gl::VertexShader vt_shader("shaders/hdr/texture.vert");
    gl::FragmentShader ft_shader("shaders/hdr/texture.frag");
    shaders.push_back(&vt_shader);
    shaders.push_back(&ft_shader);
    gl::ShaderProgram tp(shaders);
    for (auto& i: shaders) {
        i->Delete();
    }
    shaders.clear();

    // bind frame
    gl::Framebuffer fb;
    std::shared_ptr<gl::ColorAttachment> ca(
        new gl::FloatColorAttachment(gldef::SCR_WIDTH, gldef::SCR_HEIGHT, GL_RGB16F)
    );
    std::shared_ptr<gl::DepthStencilAttachment> da(
        new gl::RenderDepthStencilAttachment(gldef::SCR_WIDTH, gldef::SCR_HEIGHT)
    );
    fb.Attach(ca, da);

    // texture
    gl::Texture2D wall;
    wall.LoadImage("images/texture/toy_box.png");

    // set light
    std::vector<vec3> light_posis;
    light_posis.push_back(vec3(0, 0, -34));
    light_posis.push_back(vec3(1, 0, -10));
    light_posis.push_back(vec3(0, 1, -15));
    light_posis.push_back(vec3(-1, -1, -20));
    std::vector<vec3> light_colors;
    light_colors.push_back(vec3(100, 100, 100));
    light_colors.push_back(vec3(0.3, 0, 0));
    light_colors.push_back(vec3(0.0, 0, 0.3));
    light_colors.push_back(vec3(0.0, 0.3, 0));
    hp.Use();
    for (int i = 0; i < 4; ++i) {
        hp.SetUniform("light_posis["+std::to_string(i)+"]", light_posis[i]);
        hp.SetUniform("light_colors["+std::to_string(i)+"]", light_colors[i]);
    }

    // enable something
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);
    // glEnable(GL_MULTISAMPLE);

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
        processInput(window, deltaTime, tp);

        // render
        // ------
        glClearColor(0, 0, 0, 0);
        glEnable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_FRAMEBUFFER, fb.id);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, wall.texture);
        RenderScene(vao, hp);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        tp.Use();
        tp.SetUniform("tex", 0);
        glBindVertexArray(pvao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fb.color->id);
        glDrawArrays(GL_TRIANGLES, 0, 6);

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

