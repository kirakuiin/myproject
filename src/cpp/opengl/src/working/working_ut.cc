// working_ut.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jun 16
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

static gl::Camera camera(glm::vec3(0, 50, 0));

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
    // stbi_set_flip_vertically_on_load(true);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
    }

    gl::Model planet("model/planet/planet.obj");
    gl::Model rock("model/rock/rock.obj");

    // for (int i = 0; i < rock._meshes.size(); ++i) {
    //     unsigned int vao = rock._meshes[i]->_vao;
    //     glBindVertexArray(vao);
    //     size_t vec4size = sizeof(vec4);
    //     glEnableVertexAttribArray(3);
    //     glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4*vec4size, (void*)0);
    //     glEnableVertexAttribArray(4);
    //     glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4*vec4size, (void*)(vec4size));
    //     glEnableVertexAttribArray(5);
    //     glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4*vec4size, (void*)(vec4size*2));
    //     glEnableVertexAttribArray(6);
    //     glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4*vec4size, (void*)(vec4size*3));
    //
    //     glVertexAttribDivisor(3, 1);
    //     glVertexAttribDivisor(4, 1);
    //     glVertexAttribDivisor(5, 1);
    //     glVertexAttribDivisor(6, 1);
    //
    //     glBindVertexArray(0);
    // }

    const unsigned int amount(2000);
    std::shared_ptr<mat4> models(new mat4[amount]);
    srand(glfwGetTime());
    float radius = 20.0;
    float offset = 2.5;
    for (int i = 0; i < amount; ++i) {
        mat4 model(1.0);
        float angle = (float)i / (float)amount * 360.0;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = cos(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.5;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = sin(angle) * radius + displacement;
        model = glm::translate(model, vec3(x, y, z));

        float scale = (rand() % 20) / 100 + 0.05;
        model = glm::scale(model, vec3(scale));

        float rot = (rand() % 360);
        model = glm::rotate(model, rot, vec3(0.4, 0.6, 0.8));

        models.get()[i] = model;
    }

    // set shader
    std::vector<gl::Shader*> shaders;
    gl::VertexShader v_shader("shaders/instancing/shader.vert");
    gl::FragmentShader f_shader("shaders/instancing/shader.frag");
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
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 view = camera.GetViewMatrix();
        mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)gldef::SCR_WIDTH / gldef::SCR_HEIGHT, 0.1f, 100.0f);

        program.Use();
        program.SetUniform("projection", projection);
        program.SetUniform("view", view);
        program.SetUniform("model", mat4(1));
        planet.Draw(program);

        for (int i = 0; i < amount; ++i) {
            program.SetUniform("model", models.get()[i]);
            rock.Draw(program);
        }
        // for(unsigned int i = 0; i < rock._meshes.size(); i++)
        // {
        //     glBindVertexArray(rock._meshes[i]->_vao);
        //     glDrawElementsInstanced(
        //             GL_TRIANGLES, rock._meshes[i]->indices.size(), GL_UNSIGNED_INT, 0, amount
        //             );
        // }

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
