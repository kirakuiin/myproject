// pointshadow.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jul 11
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

static void RenderScene(unsigned int vao, gl::ShaderProgram& pro, bool is_real)
{
    glBindVertexArray(vao);
    mat4 view = camera.GetViewMatrix();
    mat4 projection = glm::perspective(
        glm::radians(45.0f),
        (float)gldef::SCR_WIDTH/gldef::SCR_HEIGHT, 1.0f, 75.0f
    );
    // draw skybox
    pro.Use();
    pro.SetUniform("view", view);
    pro.SetUniform("projection", projection);
    pro.SetUniform("view_pos", camera.position);
    pro.SetUniform("reverse", 0);
    if (is_real) {
        mat4 model(1);
        model = scale(model, vec3(20, 20, 20));
        mat3 normal_mat = mat3(glm::transpose(glm::inverse(model)));
        pro.SetUniform("model", model);
        pro.SetUniform("normal_mat", normal_mat);
        pro.SetUniform("reverse", 1);
        glDisable(GL_CULL_FACE);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        pro.SetUniform("reverse", 0);
        glEnable(GL_CULL_FACE);
    }
    // draw cube
    mat4 model(1);
    model = glm::translate(model, vec3(5));
    mat3 normal_mat = mat3(glm::transpose(glm::inverse(model)));
    pro.SetUniform("model", model);
    pro.SetUniform("normal_mat", normal_mat);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = mat4(1);
    model = glm::translate(model, vec3(0, -12, 0));
    model = glm::scale(model, vec3(5));
    normal_mat = mat3(glm::transpose(glm::inverse(model)));
    pro.SetUniform("model", model);
    pro.SetUniform("normal_mat", normal_mat);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

class POSUT : public testing::Test {
  public:
    static void SetUpTestCase();
    static void TearDownTestCase() {}
    void SetUp() override {}
    void TearDown() override {}
};

void POSUT::SetUpTestCase() {
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

TEST_F(POSUT, HelloPointShadow) {
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
        -1, -1, -1, 0, 0, -1, 0.0f, 0.0f,
        -1,  1, -1, 0, 0, -1, 0.0f, 1.0f,
        1,  1, -1,  0, 0, -1, 1.0f, 1.0f,
        1,  1, -1,  0, 0, -1, 1.0f, 1.0f,
        1, -1, -1,  0, 0, -1, 1.0f, 0.0f,
        -1, -1, -1, 0, 0, -1, 0.0f, 0.0f,

        -1, -1,  1, 0, 0, 1, 0.0f, 0.0f,
        1, -1,  1,  0, 0, 1, 1.0f, 0.0f,
        1,  1,  1,  0, 0, 1, 1.0f, 1.0f,
        1,  1,  1,  0, 0, 1, 1.0f, 1.0f,
        -1,  1,  1, 0, 0, 1, 0.0f, 1.0f,
        -1, -1,  1, 0, 0, 1, 0.0f, 0.0f,

        -1,  1,  1, -1, 0, 0, 1.0f, 1.0f,
        -1,  1, -1, -1, 0, 0, 0.0f, 1.0f,
        -1, -1, -1, -1, 0, 0, 0.0f, 0.0f,
        -1, -1, -1, -1, 0, 0, 0.0f, 0.0f,
        -1, -1,  1, -1, 0, 0, 1.0f, 0.0f,
        -1,  1,  1, -1, 0, 0, 1.0f, 1.0f,

        1,  1,  1, 1, 0, 0, 1.0f, 1.0f,
        1, -1,  1, 1, 0, 0, 1.0f, 0.0f,
        1, -1, -1, 1, 0, 0, 0.0f, 0.0f,
        1, -1, -1, 1, 0, 0, 0.0f, 0.0f,
        1,  1, -1, 1, 0, 0, 0.0f, 1.0f,
        1,  1,  1, 1, 0, 0, 1.0f, 1.0f,

        -1, -1, -1, 0, -1, 0, 0.0f, 1.0f,
        1, -1, -1,  0, -1, 0, 1.0f, 1.0f,
        1, -1,  1,  0, -1, 0, 1.0f, 0.0f,
        1, -1,  1,  0, -1, 0, 1.0f, 0.0f,
        -1, -1,  1, 0, -1, 0, 0.0f, 0.0f,
        -1, -1, -1, 0, -1, 0, 0.0f, 1.0f,

        -1,  1, -1, 0, 1, 0, 0.0f, 1.0f,
        -1,  1,  1, 0, 1, 0, 0.0f, 0.0f,
        1,  1,  1,  0, 1, 0, 1.0f, 0.0f,
        1,  1,  1,  0, 1, 0, 1.0f, 0.0f,
        1,  1, -1,  0, 1, 0, 1.0f, 1.0f,
        -1,  1, -1, 0, 1, 0, 0.0f, 1.0f
    };

    unsigned c_vao, c_vbo;
    glGenVertexArrays(1, &c_vao);
    glGenBuffers(1, &c_vbo);
    glBindVertexArray(c_vao);
    glBindBuffer(GL_ARRAY_BUFFER, c_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float),
                          (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float),
                          (void*)(6*sizeof(float)));

    // set shader
    gl::VertexShader vs_shader("shaders/pointshadow/shadow.vert");
    gl::GeometryShader gs_shader("shaders/pointshadow/shadow.geom");
    gl::FragmentShader fs_shader("shaders/pointshadow/shadow.frag");
    std::vector<gl::Shader*> shaders;
    shaders.push_back(&vs_shader);
    shaders.push_back(&gs_shader);
    shaders.push_back(&fs_shader);
    gl::ShaderProgram sprogram(shaders);
    for (auto& i: shaders) {
        i->Delete();
    }
    shaders.clear();

    gl::VertexShader vr_shader("shaders/pointshadow/real.vert");
    gl::FragmentShader fr_shader("shaders/pointshadow/real.frag");
    shaders.push_back(&vr_shader);
    shaders.push_back(&fr_shader);
    gl::ShaderProgram rprogram(shaders);
    for (auto& i: shaders) {
        i->Delete();
    }
    shaders.clear();

    // load textures
    // -------------
    gl::Texture2D t_cube;
    t_cube.LoadImage("images/texture/container.jpg");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, t_cube.texture);

    // set light
    const float sw(1024), sh(1024);
    vec3 lightpos = vec3(0);
    float aspect = sw/sh;
    float near = 1.0f;
    float far = 75.0f;
    mat4 shadow_project = glm::perspective(glm::radians(90.0f), aspect, near,
                                           far);
    std::vector<mat4> shadow_trans;
    shadow_trans.push_back(shadow_project*
        glm::lookAt(lightpos, lightpos + vec3(1, 0, 0), vec3(0, -1, 0))
    );
    shadow_trans.push_back(shadow_project*
        glm::lookAt(lightpos, lightpos + vec3(-1, 0, 0), vec3(0, -1, 0))
    );
    shadow_trans.push_back(shadow_project*
        glm::lookAt(lightpos, lightpos + vec3(0, 1, 0), vec3(0, 0, 1))
    );
    shadow_trans.push_back(shadow_project*
        glm::lookAt(lightpos, lightpos + vec3(0, -1, 0), vec3(0, 0, -1))
    );
    shadow_trans.push_back(shadow_project*
        glm::lookAt(lightpos, lightpos + vec3(0, 0, 1), vec3(0, -1, 0))
    );
    shadow_trans.push_back(shadow_project*
        glm::lookAt(lightpos, lightpos + vec3(0, 0, -1), vec3(0, -1, 0))
    );
    rprogram.Use();
    rprogram.SetUniform("light_pos", lightpos);
    rprogram.SetUniform("far_plane", far);
    rprogram.SetUniform("tex", 0);
    rprogram.SetUniform("depth_map", 1);
    sprogram.Use();
    sprogram.SetUniform("light_pos", lightpos);
    sprogram.SetUniform("far_plane", far);
    for (int i = 0; i < 6; ++i) {
        sprogram.SetUniform(("shadow_mat[" + std::to_string(i) + "]").c_str(),
                            shadow_trans[i]);
    }

    // enable something
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    // glEnable(GL_MULTISAMPLE);

    // bind frame
    gl::Framebuffer shadow_frame;
    std::shared_ptr<gl::ColorAttachment> ca(nullptr);
    std::shared_ptr<gl::CubeShadowAttachment> da(
        new gl::CubeShadowAttachment(sw, sh)
    );
    shadow_frame.Attach(ca, da);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, da->id);

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
        glViewport(0, 0, sw, sh);
        glBindFramebuffer(GL_FRAMEBUFFER, shadow_frame.id);
        glClear(GL_DEPTH_BUFFER_BIT);
        // 使用正面剔除避免阴影失真, 为了避免使用bias,
        // 不要渲染不产生阴影的物体到阴影贴图里.
        glCullFace(GL_FRONT);
        RenderScene(c_vao, sprogram, false);
        glCullFace(GL_BACK);
        glViewport(0, 0, gldef::SCR_WIDTH, gldef::SCR_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        RenderScene(c_vao, rprogram, true);

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

