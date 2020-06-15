// working_ut.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jun 15
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
#include "include/model.h"

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

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
    }

    glEnable(GL_DEPTH_TEST);
    // set shader
    gl::VertexShader ov_shader("shaders/model/ov_shader.gls");
    gl::FragmentShader of_shader("shaders/model/of_shader.gls");
    std::vector<gl::Shader*> shaders;
    shaders.push_back(&ov_shader);
    shaders.push_back(&of_shader);
    gl::ShaderProgram obj_program(shaders);
    for (auto& i: shaders) {
        i->Delete();
    }
    // set model
    gl::Model nanoobj("model/nanosuit/nanosuit.obj");

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
        mat4 model(1.0f);
        model = glm::scale(model, vec3(0.3, 0.3, 0.3));

        // draw object
        obj_program.Use();
        obj_program.SetUniform("projection", projection);
        obj_program.SetUniform("view", view);
        obj_program.SetUniform("model", model);
        obj_program.SetUniform("material.shininess", 64.0f);
        obj_program.SetUniform("normal_mat", glm::mat3(glm::transpose(glm::inverse(view * model))));
        // set direction light
        obj_program.SetUniform("dir_light.direction", vec3(0, -1, 0));
        obj_program.SetUniform("dir_light.diffuse", vec3(0.4, 0.4, 0.4));
        obj_program.SetUniform("dir_light.specular", vec3(0.5, 0.5, 0.5));
        obj_program.SetUniform("dir_light.ambient", vec3(0.40, 0.40, 0.40));
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

        nanoobj.Draw(obj_program);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}
