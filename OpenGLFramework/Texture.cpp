//
//  Texture.cpp
//  OpenGLFramework
//
//  Created by Aybars Acar on 23/1/21.
//

#include "Texture.hpp"
#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string &path)
: m_RenderID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
//  load the image
  stbi_set_flip_vertically_on_load(1);
  
//  4 is desired channel - represents rgba which has 4 channels
//  red channel, green channel, blue channel, alpha channel
  m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
  
//  Load the texture
  GLCall(glGenTextures(1, &m_RenderID));
  
//  bind the texture
  GLCall(glBindTexture(GL_TEXTURE_2D, m_RenderID));
  
//  set up settings
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));        // S -> s and t are like x and y for textures
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));        // T
  
//  Give OpenGL the data we read
  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
  
  GLCall(glBindTexture(GL_TEXTURE_2D, 0));
  
  if (m_LocalBuffer)
  {
//    if its successfully loaded onto the GPU, free up the memory in the CPU
    stbi_image_free(m_LocalBuffer);
  }
}

Texture::~Texture()
{
  GLCall(glDeleteTextures(1, &m_RenderID));
}

void Texture::Bind(unsigned int slot) const
{
//  specify a texture slot
//  GL_TEXTURE0 to GL_TEXTURE31
//  we can add them up since they are consecutive
  GLCall(glActiveTexture(GL_TEXTURE0 + slot));
  
  GLCall(glBindTexture(GL_TEXTURE_2D, m_RenderID));
}

void Texture::Unbind() const
{
  GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
