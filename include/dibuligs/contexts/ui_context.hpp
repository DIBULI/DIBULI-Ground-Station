#ifndef UI_CONTEXT_HPP
#define UI_CONTEXT_HPP

#include <iostream>
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "implot_internal.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#if defined(__APPLE__)
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include "dibuligs/contexts/icontext.hpp"

#ifdef __APPLE__
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#endif

class UIContext: public IContext {
public:
  UIContext();
  ~UIContext();

  void pre_load() override;
  void post_run() override;

  GLFWwindow* window;

  // imgui properties
  ImGuiIO io;
};

#endif /* UI_CONTEXT_HPP */
