// easy_engine.h - 通用头文件
// Version: 1.0
// Author: Wang Zhuowei wang.zhuowei@foxmail.com
// Copyright: (c) wang.zhuowei@foxmail.com All rights reserved.
// Last Change: 2020 Aug 25
// License: GPL.v3
// 通过预定义来设定引入那些easy engine头文件
//   通过 #define __EASY_ENGINE_ALL__ 来包含全部头文件
//     通过 #define __EASY_ENGINE_COMMON__ 来包含公共模块
//     通过 #define __EASY_ENGINE_GRAPHICS__ 来包含图形模块
//     通过 #define __EASY_ENGINE_OPENGL__ 来包含OPENGL模块
//     通过 #define __EASY_ENGINE_UTILITY__ 来包含工具模块
//     通过 #define __EASY_ENGINE_PHYSICE__ 来包含物理模块

#ifndef __CPP_EASY_ENGINE_INCLUDE_EASY_ENGINE_H__
#define __CPP_EASY_ENGINE_INCLUDE_EASY_ENGINE_H__

#ifdef __EASY_ENGINE_ALL__

#define __EASY_ENGINE_COMMON__    // 引入公共模块
#define __EASY_ENGINE_GRAPHICS__  // 引入图形模块
#define __EASY_ENGINE_OPENGL__    // 引入OPENGL模块
#define __EASY_ENGINE_UTILITY__   // 引入工具模块
#define __EASY_ENGINE_PHYSICS__   // 引入物理模块
#define __EASY_ENGINE_WIDGET__    // 引入部件模块
#define __EASY_ENGINE_AUDIO__     // 引入音频模块

#endif  // __EASY_ENGINE_ALL__

#ifdef __EASY_ENGINE_COMMON__
#include "common/const.h"
#include "common/math.h"
#include "common/timer.h"
#endif  // __EASY_ENGINE_COMMON__

#ifdef __EASY_ENGINE_GRAPHICS__
#include "graphics/animation.h"
#include "graphics/camera.h"
#include "graphics/font.h"
#include "graphics/particle.h"
#include "graphics/sprite.h"
#endif  // __EASY_ENGINE_GRAPHICS__

#ifdef __EASY_ENGINE_OPENGL__
#include "opengl/framebuffer.h"
#include "opengl/shader.h"
#include "opengl/texture.h"
#endif  // __EASY_ENGINE_OPENGL__

#ifdef __EASY_ENGINE_UTILITY__
#include "utility/config.h"
#include "utility/format.h"
#include "utility/log.h"
#endif  // __EASY_ENGINE_UTILITY__

#ifdef __EASY_ENGINE_WIDGET__
#include "widget/button.h"
#include "widget/container.h"
#include "widget/cursor.h"
#include "widget/window.h"
#endif  // __EASY_ENGINE_WIDGET__

#ifdef __EASY_ENGINE_PHYSICS__
#include "physics/collision.h"
#endif  // __EASY_ENGINE_PHYSICS__

#ifdef __EASY_ENGINE_AUDIO__
#include "audio/soundengine.h"
#endif  // __EASY_ENGINE_AUDIO__

#endif  // __CPP_EASY_ENGINE_INCLUDE_EASY_ENGINE_H__
