// working_ut.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jul 23
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
inline void processInput(GLFWwindow *window, float delta, gl::ShaderProgram& p)
{
    static float speed = camera.movement_speed;
    static float ssao = 1;
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
        ssao = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        ssao = 0;
    }
    p.Use();
    p.SetUniform("use_ssao", ssao);
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

static void RenderScene(unsigned int vao, gl::Model& mod, gl::ShaderProgram& pro)
{
    mat4 view = camera.GetViewMatrix();
    mat4 projection = glm::perspective(
        glm::radians(45.0f),
        (float)gldef::SCR_WIDTH/gldef::SCR_HEIGHT, 0.1f, 75.0f
    );
    pro.Use();
    pro.SetUniform("view", view);
    pro.SetUniform("projection", projection);
    // draw cube
    mat4 model(1);
    model = glm::translate(model, vec3(0, 0, -5));
    model = glm::rotate(model, glm::radians(-90.0f), vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(180.0f), vec3(0, 0, 1));
    pro.SetUniform("model", model);
    mod.Draw(pro);

    model = mat4(1);
    model = glm::translate(model, vec3(0, -1.4, 1));
    model = glm::scale(model, vec3(20, 20, 20));
    pro.SetUniform("model", model);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
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
    glfwWindowHint(GLFW_SAMPLES ,4);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

}

TEST_F(WOKUT, Hello) {
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
        -0.5f,  0, -0.5f, 0, 1, 0, 0.0f, 1.0f,
        -0.5f,  0,  0.5f, 0, 1, 0, 0.0f, 0.0f,
        0.5f,  0,  0.5f,  0, 1, 0, 1.0f, 0.0f,
        0.5f,  0,  0.5f,  0, 1, 0, 1.0f, 0.0f,
        0.5f,  0, -0.5f,  0, 1, 0, 1.0f, 1.0f,
        -0.5f,  0, -0.5f, 0, 1, 0, 0.0f, 1.0f
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

    // model load
    gl::Model nanosuit("model/nanosuit/nanosuit.obj");

    // set shader
    gl::VertexShader v_shader("shaders/ssao/gbuffer.vert");
    gl::FragmentShader f_shader("shaders/ssao/gbuffer.frag");
    std::vector<gl::Shader*> shaders;
    shaders.push_back(&v_shader);
    shaders.push_back(&f_shader);
    gl::ShaderProgram gp(shaders);
    for (auto& i: shaders) {
        i->Delete();
    }
    shaders.clear();

    gl::VertexShader vt_shader("shaders/ssao/texture.vert");
    gl::FragmentShader ft_shader("shaders/ssao/texture.frag");
    shaders.push_back(&vt_shader);
    shaders.push_back(&ft_shader);
    gl::ShaderProgram tp(shaders);
    for (auto& i: shaders) {
        i->Delete();
    }
    shaders.clear();

    gl::VertexShader vs_shader("shaders/ssao/ssao.vert");
    gl::FragmentShader fs_shader("shaders/ssao/ssao.frag");
    shaders.push_back(&vs_shader);
    shaders.push_back(&fs_shader);
    gl::ShaderProgram sp(shaders);
    for (auto& i: shaders) {
        i->Delete();
    }
    shaders.clear();

    gl::VertexShader vf_shader("shaders/ssao/flur.vert");
    gl::FragmentShader ff_shader("shaders/ssao/flur.frag");
    shaders.push_back(&vf_shader);
    shaders.push_back(&ff_shader);
    gl::ShaderProgram fp(shaders);
    for (auto& i: shaders) {
        i->Delete();
    }
    shaders.clear();

    // bind frame
    gl::Framebuffer gb;
    // position frame
    std::shared_ptr<gl::ColorAttachment> posa(
        new gl::FloatColorAttachment(gldef::SCR_WIDTH, gldef::SCR_HEIGHT, GL_RGB16F, 0)
    );
    // normal frame
    std::shared_ptr<gl::ColorAttachment> nora(
        new gl::FloatColorAttachment(gldef::SCR_WIDTH, gldef::SCR_HEIGHT, GL_RGB16F, 1)
    );
    // albedospec frame
    std::shared_ptr<gl::ColorAttachment> colora(
        new gl::TextureColorAttachment(gldef::SCR_WIDTH, gldef::SCR_HEIGHT, 2)
    );
    // depth frame
    std::shared_ptr<gl::DepthStencilAttachment> da(
        new gl::RenderDepthStencilAttachment(gldef::SCR_WIDTH, gldef::SCR_HEIGHT)
    );
    gb.Attach(posa, da);
    gb.Attach(nora, nullptr);
    gb.Attach(colora, nullptr);

    // ssao frame
    gl::Framebuffer sb;
    std::shared_ptr<gl::ColorAttachment> ssaoa(
        new gl::FloatColorAttachment(gldef::SCR_WIDTH, gldef::SCR_HEIGHT, GL_RGB16F, 0)
    );
    sb.Attach(ssaoa, nullptr);

    // flur frame
    gl::Framebuffer fb;
    std::shared_ptr<gl::ColorAttachment> flura(
        new gl::FloatColorAttachment(gldef::SCR_WIDTH, gldef::SCR_HEIGHT, GL_RGB16F, 0)
    );
    fb.Attach(flura, nullptr);

    // set light
    light_posis.push_back(vec3(-10, 10, 10));
    light_colors.push_back(vec3(0, 0, 80));

    // ssao setting
    // 法线半球
    std::uniform_real_distribution<float> random_floats(0.0, 1.0);
    std::default_random_engine generator;
    std::vector<vec3> ssao_kernel;
    for (int i = 0; i < 64; ++i) {
        vec3 sample(
            random_floats(generator) * 2.0 - 1.0,
            random_floats(generator) * 2.0 - 1.0,
            random_floats(generator)    // 让z限制在0-1之间, 保证为半球
        );
        sample = glm::normalize(sample);
        sample *= random_floats(generator);
        float scale = i/64.0;
        scale = 0.1 + scale*scale*(1-0.1);  // 接近原点
        sample *= scale;
        ssao_kernel.push_back(sample);
    }
    // 核心转动
    std::vector<vec3> ssao_noise;
    for (int i = 0; i < 16; ++i) {
        vec3 noise(
            random_floats(generator) * 2.0 - 1.0,
            random_floats(generator) * 2.0 - 1.0,
            0.0f
        );
        ssao_noise.push_back(noise);
    }
    sp.Use();
    mat4 projection = glm::perspective(
            glm::radians(45.0f),
            (float)gldef::SCR_WIDTH/gldef::SCR_HEIGHT, 0.1f, 75.0f
            );
    sp.SetUniform("projection", projection);
    for (int i = 0; i < 64; ++i) {
        sp.SetUniform("samples[" + std::to_string(i) + "]", ssao_kernel[i]);
    }
    unsigned int noise_tex;
    glGenTextures(1, &noise_tex);
    glBindTexture(GL_TEXTURE_2D, noise_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT,
                 &ssao_noise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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
        // 几何计算
        glClearColor(0, 0, 0, 0);
        glEnable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_FRAMEBUFFER, gb.id);
        unsigned int attachments[] = {GL_COLOR_ATTACHMENT0,
                                      GL_COLOR_ATTACHMENT1,
                                      GL_COLOR_ATTACHMENT2};
        glDrawBuffers(3, attachments);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        RenderScene(vao, nanosuit, gp);

        // ssao
        glBindFramebuffer(GL_FRAMEBUFFER, sb.id);
        glClear(GL_COLOR_BUFFER_BIT);
        for (int i = 0; i < 2; ++i) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, gb.colors[i]->id);
        }
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, noise_tex);
        sp.Use();
        sp.SetUniform("g_position", 0);
        sp.SetUniform("g_normal", 1);
        sp.SetUniform("tex_noise", 2);
        glBindVertexArray(pvao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // 模糊
        glBindFramebuffer(GL_FRAMEBUFFER, fb.id);
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ssaoa->id);
        fp.Use();
        fp.SetUniform("ssao", 0);
        glBindVertexArray(pvao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // 光照计算
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (int i = 0; i < 3; ++i) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, gb.colors[i]->id);
        }
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, flura->id);
        tp.Use();
        tp.SetUniform("g_position", 0);
        tp.SetUniform("g_normal", 1);
        tp.SetUniform("g_albedospec", 2);
        tp.SetUniform("ssao", 3);
        mat4 view = camera.GetViewMatrix();
        float constant = 1.0;
        float linear = 0.7;
        float quadratic = 1.8;
        tp.SetUniform("light.position", vec3(view * vec4(light_posis[0], 1)));
        tp.SetUniform("light.color", light_colors[0]);
        tp.SetUniform("light.constant", constant);
        tp.SetUniform("light.linear", linear);
        tp.SetUniform("light.quadratic", quadratic);
        glBindVertexArray(pvao);
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

