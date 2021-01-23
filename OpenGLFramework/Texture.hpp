//
//  Texture.hpp
//  OpenGLFramework
//
//  Created by Aybars Acar on 23/1/21.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <stdio.h>
#include <string>
#include "Renderer.h"

class Texture {
private:
  unsigned int m_RenderID;
  std::string m_FilePath;
  unsigned char* m_LocalBuffer;   // local storage for texture
  int m_Width, m_Height, m_BPP;   // BPP == Bits Per Picture
  
public:
  Texture(const std::string &path);
  ~Texture();
  
  /**
   * optional parameter allows you to specify the slot you want to bind the texture to
   * because we have the ability to bind more than 1 texture bind
   * typically there are 32 texture slots
   */
  void Bind(unsigned int slot = 0) const;
  
  void Unbind() const;
  
  inline int GetWidth() const { return m_Width; }
  inline int GetHeight() const { return m_Height; }
  
  
};

#endif /* Texture_hpp */
