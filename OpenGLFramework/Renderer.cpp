//
//  Renderer.cpp
//  OpenGLFramework
//
//  Created by Aybars Acar on 22/1/21.
//

#include <stdio.h>
#include "Renderer.h"
#include <iostream>

/**
 * clear all the errors
 */
void GLClearError()
{
  while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char *function, const char *file,  int line)
{
//  runs until error variable is not 0
  while (GLenum error =  glGetError())
  {
    std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ": " << line << std::endl;
    return false;
  }
  return true;
}
