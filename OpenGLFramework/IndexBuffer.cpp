//
//  IndexBuffer.cpp
//  OpenGLFramework
//
//  Created by Aybars Acar on 22/1/21.
//

#include "IndexBuffer.hpp"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count)
: m_Count(count)
{
  ASSERT(sizeof(unsigned int) == sizeof(GLuint));
  
  GLCall(glGenBuffers(1, &m_RendererID)); // gives us back an id
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
  GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
