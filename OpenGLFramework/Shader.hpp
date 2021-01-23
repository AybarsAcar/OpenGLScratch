//
//  Shader.hpp
//  OpenGLFramework
//
//  Created by Aybars Acar on 23/1/21.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <stdio.h>
#include <string>
#include <unordered_map>

/**
 * return this struct as we parse the shader file into 2 different shader programs
 */
struct ShaderProgramSource
{
  std::string VertexSource;
  std::string FragmentSource;
};


class Shader {
private:
  std::string m_FilePath;
  unsigned int m_RendererID;

//  caching synstem for uniforms
  std::unordered_map<std::string, int> m_UniformLocationCache;
  
public:
  Shader(const std::string& filepath);
  ~Shader();
  
//  for the use program on the opengl side
  void Bind() const;
  void Unbind() const;
  
//  set uniforms
  
  /**
   * for our textures
   */
  void SetUniform1i(const std::string &name, int value);
  
  void SetUniform4F(const std::string &name, float v0, float v1, float f2, float f3);
  void SetUniform1F(const std::string &name, float value);
  
private:
  ShaderProgramSource ParseShader(const std::string& filepath);
  unsigned int CompileShader(unsigned int type, const std::string &source);
  unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
  int GetUniformLocation(const std::string &name);
};

#endif /* Shader_hpp */
