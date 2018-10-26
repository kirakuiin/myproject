MESSAGE(STATUS "sdl2 is used")
ADD_DEFINITIONS(-D__USING_SDL2__)

IF(APPLE)
    INCLUDE_DIRECTORIES(~/Library/SDL2.framework/Headers)
    INCLUDE_DIRECTORIES(~/Library/SDL2_image.framework/Headers)
    INCLUDE_DIRECTORIES(~/Library/SDL2_ttf.framework/Headers)
    INCLUDE_DIRECTORIES(~/Library/SDL2_mixer.framework/Headers)

    SET(CMAKE_FRAMEWORK_PATH ~/Library)
    # 包含头文件路径和链接库
    FIND_LIBRARY(SDL2_LIB SDL2)
    FIND_LIBRARY(SDL2_IMAGE_LIB SDL2_image)
    FIND_LIBRARY(SDL2_TTF_LIB SDL2_ttf)
    FIND_LIBRARY(SDL2_MIXER_LIB SDL2_mixer)

    MARK_AS_ADVANCED (SDL2_LIB SDL2_IMAGE_LIB SDL2_TTF_LIB SDL2_MIXER_LIB)
    SET(LINK_SDL2_LIBS  ${SDL2_LIB}
                        ${SDL2_IMAGE_LIB}
                        ${SDL2_TTF_LIB}
                        ${SDL2_MIXER_LIB})
ENDIF(APPLE)
