#pragma once
#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <Definitions.h>
#include <iostream>
//#define ASSERT(x) if (!(x)) __debugbreak();
#define ASSERT(x) if (!(x)) ;
#define DebugLog(x) OpenGLDebugger::GlCLearErrors();\
    x;\
    ASSERT(OpenGLDebugger::GLErrorLog(#x, __FILE__, __LINE__))
static class OpenGLDebugger {
public:
    static void GlCLearErrors();
    static bool GLErrorLog(const char* function, const char* file, int line);
private:
};