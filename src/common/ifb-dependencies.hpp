#ifndef IFB_DEPENDENCIES_HPP
#define IFB_DEPENDENCIES_HPP

#ifndef IFB_DEPENDENCIES_EXCLUDE_ALL

#ifndef IFB_DEPENDENCIES_EXCLUDE_IMGUI
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_win32.h>
#endif //IFB_DEPENDENCIES_EXCLUDE_IMGUI

#ifndef IFB_DEPENDENCIES_EXCLUDE_STB
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif //IFB_DEPENDENCIES_EXCLUDE_IMGUI

#ifndef IFB_DEPENDENCIES_EXCLUDE_GLEW
#include <Windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/wgl.h>
#include <GL/glu.h>
#endif //IFB_DEPENDENCIES_EXCLUDE_GLEW

#endif //IFB_DEPENDENCIES_EXCLUDE_ALL

#endif //IFB_DEPENDENCIES_HPP