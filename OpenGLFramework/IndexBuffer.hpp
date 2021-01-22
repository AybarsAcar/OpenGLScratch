//
//  IndexBuffer.hpp
//  OpenGLFramework
//
//  Created by Aybars Acar on 22/1/21.
//

#ifndef IndexBuffer_hpp
#define IndexBuffer_hpp

#include <stdio.h>

// size refers to the bytes
// count means element count in this project

class IndexBuffer
{
private:
  unsigned int m_RendererID;
  unsigned int m_Count;     // to know how many indices it has
  
public:
  IndexBuffer(const unsigned int *data, unsigned int count);
  ~IndexBuffer();
  
  void Bind() const;
  void Unbind() const;
  
  inline unsigned int GetCount() const { return m_Count; }
};

#endif /* IndexBuffer_hpp */
