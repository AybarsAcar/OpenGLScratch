//
//  VertexBuffer.hpp
//  OpenGLFramework
//
//  Created by Aybars Acar on 22/1/21.
//

#ifndef VertexBuffer_hpp
#define VertexBuffer_hpp

#include <stdio.h>

/**
 * Represents the VertexBuffer in the OpenGL
 */
class VertexBuffer
{
private:
//  OpenGL needs some numeric to keep track of objects - this is the actual id used by OpenGL
  unsigned int m_RendererID;
  
public:
  VertexBuffer(const void* data, unsigned int size);
  ~VertexBuffer();
  
  void Bind() const;
  void Unbind() const;
  
};

#endif /* VertexBuffer_hpp */
