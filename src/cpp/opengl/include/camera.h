// camera.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jun 08
// License: GPL.v3

#ifndef __OPENGL_INCLUDE_CAMERA_H__
#define __OPENGL_INCLUDE_CAMERA_H__

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace gl {

enum class CAMERA_DIRECT {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};

// 摄像机类的默认参数
const float CAM_YAW         = -90.0f;
const float CAM_PITCH       = 0.0f;
const float CAM_SPEED       = 2.5f;
const float CAM_SENSITIVITY = 0.1f;
const float CAM_ZOOM        = 45.0f;

// 摄像机类，用来获得将世界坐标转换为视野坐标的变换矩阵
class Camera {
  public:
    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float y = CAM_YAW,
           float p = CAM_PITCH);

    // returns the view matrix
    glm::mat4 GetViewMatrix();

    // process input received from any keyboard-like input system
    void ProcessKeyboard(CAMERA_DIRECT direction, float delta);

    // process input received from a mouse input system
    void ProcessMouse(float x_offset, float y_offset,
                      bool constrain_pitch = true);

    // process input received from a mouse scroll-wheel event
    void ProcessScroll(float y_offset);

    // camera attributes
    glm::vec3 position;
    glm::vec3 world_up;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    // euler angles
    float yaw;
    float pitch;
    // camera options
    float movement_speed;
    float mouse_sensitivity;
    float zoom;
  private:
    void UpdateCameraVectors();
};

} // namespace gl
#endif // __OPENGL_INCLUDE_CAMERA_H__
