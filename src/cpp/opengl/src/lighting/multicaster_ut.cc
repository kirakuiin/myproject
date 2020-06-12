// multicaster_ut.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jun 09
// License: GPL.v3

#include <iostream>
#include <vector>
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

class MCSUT : public testing::Test {
  public:
    static void SetUpTestCase();
    static void TearDownTestCase() {}
    void SetUp() override {}
    void TearDown() override {}
};

void MCSUT::SetUpTestCase() {
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

TEST_F(MCSUT, HelloMultiCaster) {
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
    // set shader
    gl::VertexShader ov_shader("shaders/multicaster/ov_shader.gls");
    gl::VertexShader lv_shader("shaders/multicaster/lv_shader.gls");
    gl::FragmentShader of_shader("shaders/multicaster/of_shader.gls");
    gl::FragmentShader lf_shader("shaders/multicaster/lf_shader.gls");
    std::vector<gl::Shader*> shaders;
    shaders.push_back(&ov_shader);
    shaders.push_back(&of_shader);
    gl::ShaderProgram obj_program(shaders);
    for (auto& i: shaders) {
        i->Delete();
    }
    shaders.clear();
    shaders.push_back(&lv_shader);
    shaders.push_back(&lf_shader);
    gl::ShaderProgram light_program(shaders);
    for (auto& i: shaders) {
        i->Delete();
    }

    float vertexs[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    vec3 cubes[] = {
        vec3( 0.0f,  0.0f, -0.0f),
        vec3( 2.0f,  5.0f, -15.0f),
        vec3(-1.5f, -2.2f, -2.5f),
        vec3(-3.8f, -2.0f, -12.3f),
        vec3( 2.4f, -0.4f, -3.5f),
        vec3(-1.7f,  3.0f, -7.5f),
        vec3( 1.3f, -2.0f, -2.5f),
        vec3( 1.5f,  2.0f, -2.5f),
        vec3( 1.5f,  0.2f, -1.5f),
        vec3(-1.3f,  1.0f, -1.5f),
    };

    const int light_nums = 4;
    vec3 point_positions[] = {
        vec3(0.7, 0.2, 2.0),
        vec3(2.3, -3.3, -4.0),
        vec3(-4.0, 2.0, -12.0),
        vec3(0.0, 0.0, -3.0),
    };
    vec3 point_colors[] = {
        vec3(1.0, 1.0, 1.0),
        vec3(1.0, 0.0, 0.0),
        vec3(0.0, 1.0, 0.0),
        vec3(0.0, 0.0, 1.0),
    };
    float point_color_ambients[] = {
        0.05f, 0.05f, 0.05f, 0.05f,
    };

    // set texture
    gl::Texture2D text1;
    text1.LoadImage("images/texture/container2.png");
    gl::Texture2D text2;
    text2.LoadImage("images/texture/container2_specular.png");

    unsigned int vao[2], vbo;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(2, vao);

    // set object vertexs
    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8,
                          (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float)*8,
                          (void*)(sizeof(float)*6));
    glEnableVertexAttribArray(2);

    // set light vertexs
    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)0);
    glEnableVertexAttribArray(0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, text1.texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, text2.texture);

    // render loop
    // -----------
    float delta_time = 0;
    float last_time = 0;
    while (!glfwWindowShouldClose(window))
    {
        float cur_time = glfwGetTime();
        delta_time = cur_time - last_time;
        last_time = cur_time;
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // input
        // -----
        processInput(window, delta_time);

        // get transform matrix
        mat4 view = camera.GetViewMatrix();
        mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(camera.zoom),
                                      (float)gldef::SCR_WIDTH/gldef::SCR_HEIGHT,
                                      0.1f, 100.0f);

        // draw light
        light_program.Use();
        light_program.SetUniform("projection", projection);
        light_program.SetUniform("view", view);
        glBindVertexArray(vao[1]);
        for (int i = 0; i < light_nums; ++i) {
            mat4 model(1.0f);
            model = glm::translate(model, point_positions[i]);
            model = glm::scale(model, vec3(0.2f));
            light_program.SetUniform("light_color", point_colors[i]);
            light_program.SetUniform("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // draw object
        obj_program.Use();
        obj_program.SetUniform("material.diffuse", 0);
        obj_program.SetUniform("material.specular", 1);
        obj_program.SetUniform("material.shininess", 64.0f);
        obj_program.SetUniform("projection", projection);
        obj_program.SetUniform("view", view);
        // set direction light
        obj_program.SetUniform("dir_light.direction", vec3(0, -1, 0));
        obj_program.SetUniform("dir_light.diffuse", vec3(0.4, 0.4, 0.4));
        obj_program.SetUniform("dir_light.specular", vec3(0.5, 0.5, 0.5));
        obj_program.SetUniform("dir_light.ambient", vec3(0.05, 0.05, 0.05));
        // set point light
        for (int i = 0; i < light_nums; ++i) {
            std::string prefix("point_lights[");
            prefix += (std::to_string(i) + std::string("]."));
            obj_program.SetUniform(prefix+std::string("position"),
                                   vec3(view*vec4(point_positions[i], 1.0)));
            obj_program.SetUniform(prefix+std::string("diffuse"),
                                   point_colors[i]);
            obj_program.SetUniform(prefix+std::string("specular"),
                                   point_colors[i]);
            obj_program.SetUniform(prefix+std::string("ambient"),
                                   point_color_ambients[i]*point_colors[i]);
            obj_program.SetUniform(prefix+std::string("constant"),
                                   1.0f);
            obj_program.SetUniform(prefix+std::string("linear"),
                                   0.09f);
            obj_program.SetUniform(prefix+std::string("quadratic"),
                                   0.032f);
        }
        // set spotlight
        obj_program.SetUniform("spot_light.position", vec3(0, 0, 0));
        obj_program.SetUniform("spot_light.direction", vec3(view * vec4(camera.front, 0)));
        obj_program.SetUniform("spot_light.diffuse", vec3(1, 1, 1));
        obj_program.SetUniform("spot_light.specular", vec3(1, 1, 1));
        obj_program.SetUniform("spot_light.ambient", vec3(0.0, 0.0, 0.0));
        obj_program.SetUniform("spot_light.cutoff", glm::cos(glm::radians(12.5f)));
        obj_program.SetUniform("spot_light.outoff", glm::cos(glm::radians(17.5f)));
        obj_program.SetUniform("spot_light.constant", 1.0f);
        obj_program.SetUniform("spot_light.linear", 0.09f);
        obj_program.SetUniform("spot_light.quadratic", 0.032f);

        glBindVertexArray(vao[0]);
        for (auto& cube: cubes) {
            mat4 model(1.0f);
            model = glm::translate(model, cube);
            model = glm::rotate(model, (float)glm::radians(30.0), vec3(0, 0, 1));
            obj_program.SetUniform("model", model);
            glm::mat3 nor_mat = glm::mat3(glm::transpose(glm::inverse(view*model)));
            obj_program.SetUniform("normal_mat", nor_mat);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(2, vao);
    glDeleteBuffers(1, &vbo);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}
