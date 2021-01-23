//
//  VertexArray.cpp
//  OpenGLFramework
//
//  Created by Aybars Acar on 23/1/21.
//

#include "VertexArray.hpp"

#include "Renderer.h"

VertexArray::VertexArray()
{
  GLCall(glGenVertexArrays(1, &m_RendererID));
};


VertexArray::~VertexArray()
{
  GLCall(glDeleteVertexArrays(1, &m_RendererID));
};

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
  Bind(); // bind the vertex array
  
  vb.Bind();  // bind the buffer
  
  const auto& elements = layout.GetElements();
  
  unsigned int offset = 0;
  
  for (unsigned int i = 0; i < elements.size(); i++)
  {
    const auto& element = elements[i];
    
    //  we need to enable the vertex attribute with the index you want to enable
    GLCall(glEnableVertexAttribArray(i));
    //  specify the attribute for hte first vertex whihc is at index 0, last 0 is 0 bytes
    GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
    
    offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
  }
}

void VertexArray::Bind() const
{
  GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
  GLCall(glBindVertexArray(0));
}
