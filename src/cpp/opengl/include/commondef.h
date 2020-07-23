// commondef.h -
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Jun 04
// License: GPL.v3

#ifndef __OPENGL_INCLUDE_COMMONDEF_H__
#define __OPENGL_INCLUDE_COMMONDEF_H__

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>

#include "include/glm/glm.hpp"

using std::cout;
using std::endl;
using std::ifstream;
using std::stringstream;
using std::string;

namespace gldef {

// screen size
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const unsigned int INFO_LEN = 512;

inline void PrintVec(glm::vec2 vect) {
    cout<<"("<<vect.x<<", "<<vect.y<<")"<<endl;
}

inline void PrintVec(glm::vec3 vect) {
    cout<<"("<<vect.x<<", "<<vect.y<<", "<<vect.z<<")"<<endl;
}

inline void PrintVec(glm::vec4 vect) {
    cout<<"("<<vect.x<<", "<<vect.y<<", "<<vect.z<<", "<<vect.w<<")"<<endl;
}

} // namespace gldef

#endif // __OPENGL_INCLUDE_COMMONDEF_H__
