//
//  Renderer.h
//  OpenGLFramework
//
//  Created by Aybars Acar on 22/1/21.
//

#ifndef Renderer_h
#define Renderer_h

#include <GL/glew.h>

// the macros for OpenGL debugging that runs our functions
// this creates a debugger
#define ASSERT(x) if (!(x)) asm{int 3};

#if DEBUG
  #define GLCall(x) GLClearError();\
      x;\
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
  #define GLCall(x) x
#endif


/**
 * clear all the errors
 */
void GLClearError();

bool GLLogCall(const char *function, const char *file,  int line);


#endif /* Renderer_h */
