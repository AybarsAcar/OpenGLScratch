//
//  VertexBuffer.cpp
//  OpenGLFramework
//
//  Created by Aybars Acar on 22/1/21.
//

#include "VertexBuffer.hpp"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
  GLCall(glGenBuffers(1, &m_RendererID)); // gives us back an id
//  select that bufffer
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
//  specify the data
//  put the data into the buffer
  GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
  GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
