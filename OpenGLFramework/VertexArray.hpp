//
//  VertexArray.hpp
//  OpenGLFramework
//
//  Created by Aybars Acar on 23/1/21.
//

#ifndef VertexArray_hpp
#define VertexArray_hpp

#include <stdio.h>

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

/**
 *
 */
class VertexArray {
private:
  unsigned int m_RendererID;
  
public:
  VertexArray();
  ~VertexArray();
  
  void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
  
  void Bind() const;
  void Unbind() const;
  
};


#endif /* VertexArray_hpp */
