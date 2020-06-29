// blackhole_ut.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jun 29
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

static gl::Camera camera(glm::vec3(0, 0, 15));

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

std::shared_ptr<float> generate_sphere(int r, size_t u_num, size_t v_num) {
    assert(u_num>=3);
    assert(v_num>=2);
    float u=glm::radians(180.0f/u_num), v=glm::radians(360.0f/v_num);
    std::shared_ptr<float> ptr(new float[((u_num-1)*2+2) * (v_num+1) * 3]);
    auto arr = ptr.get();
    int index = 0;
    // top
    for (int i = 0; i <= v_num; ++i) {
        arr[index] = 0;         // x
        arr[index+1] = r;       // y
        arr[index+2] = 0;       // z
        index += 3;
        arr[index] = r*sin(u)*cos(i*v);
        arr[index+1] = r*cos(u);
        arr[index+2] = r*sin(u)*sin(i*v);
        index += 3;
    }
    // middle
    for (int i = 1; i < u_num-1; ++i) {
        for (int j = 0; j <= v_num; ++j) {
            arr[index] = r*sin(i*u)*cos(j*v);
            arr[index+1] = r*cos(i*u);
            arr[index+2] = r*sin(i*u)*sin(j*v);
            index += 3;
            arr[index] = r*sin((i+1)*u)*cos(j*v);
            arr[index+1] = r*cos((i+1)*u);
            arr[index+2] = r*sin((i+1)*u)*sin(j*v);
            index += 3;
        }
    }
    // bottom
    for (int i = 0; i <= v_num; ++i) {
        arr[index] = r*sin((u_num-1)*u)*cos(i*v);
        arr[index+1] = r*cos((u_num-1)*u);
        arr[index+2] = r*sin((u_num-1)*u)*sin(i*v);
        index += 3;
        arr[index] = 0;
        arr[index+1] = -r;
        arr[index+2] = 0;
        index += 3;
    }
    return ptr;
}

class BLHUT : public testing::Test {
  public:
    static void SetUpTestCase();
    static void TearDownTestCase() {}
    void SetUp() override {}
    void TearDown() override {}
};

void BLHUT::SetUpTestCase() {
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

TEST_F(BLHUT, HelloBlackHole) {
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

    size_t u = 30, v = 30;
    auto blackhole = generate_sphere(1, u, v);
    size_t holesize = ((u-1)*2+2) * (v+1) * 3;
    const float* hole_vertices = blackhole.get();

    // blackhole VAO
    unsigned int h_vao, h_vbo;
    glGenVertexArrays(1, &h_vao);
    glGenBuffers(1, &h_vbo);
    glBindVertexArray(h_vao);
    glBindBuffer(GL_ARRAY_BUFFER, h_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*holesize, hole_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // cube VAO
    unsigned int c_vao, c_vbo;
    glGenVertexArrays(1, &c_vao);
    glGenBuffers(1, &c_vbo);
    glBindVertexArray(c_vao);
    glBindBuffer(GL_ARRAY_BUFFER, c_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // set shader
    gl::VertexShader v_shader("shaders/blackhole/box.vert");
    gl::FragmentShader f_shader("shaders/blackhole/box.frag");
    std::vector<gl::Shader*> shaders;
    shaders.push_back(&v_shader);
    shaders.push_back(&f_shader);
    gl::ShaderProgram b_program(shaders);
    for (auto& i: shaders) {
        i->Delete();
    }

    shaders.clear();
    gl::VertexShader vb_shader("shaders/blackhole/blackhole.vert");
    gl::FragmentShader fb_shader("shaders/blackhole/blackhole.frag");
    shaders.push_back(&vb_shader);
    shaders.push_back(&fb_shader);
    gl::ShaderProgram h_program(shaders);
    for (auto& i: shaders) {
        i->Delete();
    }

    shaders.clear();
    gl::VertexShader vl_shader("shaders/blackhole/lens.vert");
    gl::FragmentShader fl_shader("shaders/blackhole/lens.frag");
    shaders.push_back(&vl_shader);
    shaders.push_back(&fl_shader);
    gl::ShaderProgram l_program(shaders);
    for (auto& i: shaders) {
        i->Delete();
    }

    // load textures
    // -------------
    gl::TextureCube t_universe;
    std::vector<std::string> paths;
    paths.push_back("images/texture/universe/right.png");
    paths.push_back("images/texture/universe/left.png");
    paths.push_back("images/texture/universe/top.png");
    paths.push_back("images/texture/universe/bottom.png");
    paths.push_back("images/texture/universe/front.png");
    paths.push_back("images/texture/universe/back.png");
    t_universe.LoadImages(paths);

    // enable something
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

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
        glClearColor(1, 1, 1, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        mat4 view = camera.GetViewMatrix();
        mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)gldef::SCR_WIDTH / gldef::SCR_HEIGHT, 0.1f, 100.0f);
        mat4 model(1.0f);

        // draw hole
        glDepthFunc(GL_ALWAYS);
        glStencilFunc(GL_ALWAYS, 1, 0xff);
        glStencilMask(0xff);
        h_program.Use();
        model = mat4(1.0f);
        model = glm::scale(model, vec3(3));
        h_program.SetUniform("model", model);
        h_program.SetUniform("view", view);
        h_program.SetUniform("projection", projection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(t_universe.dimension, t_universe.texture);
        glBindVertexArray(h_vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, holesize/3);

        // draw lens
        glStencilFunc(GL_NOTEQUAL, 1, 0xff);
        l_program.Use();
        model = mat4(1.0f);
        l_program.SetUniform("model", glm::scale(model, vec3(10)));
        l_program.SetUniform("view", view);
        l_program.SetUniform("projection", projection);
        l_program.SetUniform("normal_mat",
                             mat3(glm::transpose(glm::inverse(model))));
        l_program.SetUniform("vpos", camera.position);
        l_program.SetUniform("origin", vec3(0));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(t_universe.dimension, t_universe.texture);
        l_program.SetUniform("universe", 0);
        glBindVertexArray(h_vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, holesize/3);

        // draw cube
        glStencilFunc(GL_ALWAYS, 0, 0xff);
        glStencilMask(0xff);
        b_program.Use();
        model = mat4(1.0f);
        b_program.SetUniform("model", model);
        b_program.SetUniform("view", mat4(mat3(view)));
        b_program.SetUniform("projection", projection);
        glDepthFunc(GL_LEQUAL);
        glBindVertexArray(c_vao);
        glActiveTexture(GL_TEXTURE0);
        b_program.SetUniform("universe", 0);
        glBindTexture(t_universe.dimension, t_universe.texture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);

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

