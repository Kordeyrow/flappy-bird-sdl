#include "OpenGLDebugger.h"

void OpenGLDebugger::GlCLearErrors()
{
    while (glGetError() != GL_NO_ERROR);
}

bool OpenGLDebugger::GLErrorLog(const char* function, const char* file, int line)
{
    while (GLenum  error = glGetError())
    {
        std::cout << "[OPENGL ERROR] - " << error << std::endl;
        std::cout << "\nfile: " << file << "\nfunction: " << function << "\nline: " << line << std::endl;
        return false;
    }
    return true;
}