// camera.cc -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jun 08
// License: GPL.v3

#include "include/camera.h"
#include <iostream>

using glm::vec3;
using glm::mat4;
using std::cout;
using std::endl;

namespace gl {

// Camera implement
Camera::Camera(vec3 pos, vec3 up,float y, float p)
    : position(pos), world_up(up), front(0.0f, -1.0f, 0.0f), yaw(y), pitch(p),
      movement_speed(CAM_SPEED),
      mouse_sensitivity(CAM_SENSITIVITY),
      zoom(CAM_ZOOM) {
    UpdateCameraVectors();
}

mat4 Camera::GetViewMatrix() {
    //return = glm::lookAt(position, position+front, up);
    mat4 trans(1.0f);
    trans[3][0] = -position.x;
    trans[3][1] = -position.y;
    trans[3][2] = -position.z;
    vec3 z_axis = -front;
    vec3 x_axis = right;
    vec3 y_axis = up;
    mat4 rotate(1.0f);
    rotate[0][0] = x_axis.x;
    rotate[1][0] = x_axis.y;
    rotate[2][0] = x_axis.z;
    rotate[0][1] = y_axis.x;
    rotate[1][1] = y_axis.y;
    rotate[2][1] = y_axis.z;
    rotate[0][2] = z_axis.x;
    rotate[1][2] = z_axis.y;
    rotate[2][2] = z_axis.z;
    auto res = rotate * trans;
    return res;
}

void Camera::ProcessKeyboard(CAMERA_DIRECT direction, float delta) {
    float velocity = movement_speed * delta;
    switch (direction) {
        case CAMERA_DIRECT::FORWARD:
            position += front * velocity;
            break;
        case CAMERA_DIRECT::BACKWARD:
            position -= front * velocity;
            break;
        case CAMERA_DIRECT::LEFT:
            position -= right * velocity;
            break;
        case CAMERA_DIRECT::RIGHT:
            position += right * velocity;
            break;
    }
}

void Camera::ProcessMouse(float x_offset, float y_offset,
                          bool constrain_pitch) {
    pitch += y_offset*mouse_sensitivity;
    yaw += x_offset*mouse_sensitivity;
    if (constrain_pitch) {
        if (pitch > 89.0f) {
            pitch = 89.0f;
        } else if (pitch < -89.0f) {
            pitch = -89.0f;
        }
    }
    UpdateCameraVectors();
}

void Camera::ProcessScroll(float y_offset) {
    zoom -= y_offset;
    if (zoom < 1.0f) {
        zoom = 1.0f;
    }
    if (zoom > 45.0f) {
        zoom = 45.0f;
    }
}

void Camera::UpdateCameraVectors() {
    front.x = cos(glm::radians(pitch))*cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch))*sin(glm::radians(yaw));
    front = glm::normalize(front);
    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}
} // namespace gl
