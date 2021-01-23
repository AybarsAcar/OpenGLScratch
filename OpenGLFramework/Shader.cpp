//
//  Shader.cpp
//  OpenGLFramework
//
//  Created by Aybars Acar on 23/1/21.
//

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>

#include "Shader.hpp"
#include "Renderer.h"


Shader::Shader(const std::string& filepath)
: m_FilePath(filepath), m_RendererID(0)
{
  //  read in the shaders
  ShaderProgramSource source = ParseShader(filepath);
  
  m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
  GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
  GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
  GLCall(glUseProgram(0));
}

/*************************** UNIFORM FUNCTIONS START ***************************/


void Shader::SetUniform4F(const std::string &name, float v0, float v1, float v2, float v3)
{
  GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1F(const std::string &name, float value)
{
  GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform1i(const std::string &name, int value)
{
  GLCall(glUniform1i(GetUniformLocation(name), value));
}

/*************************** UNIFORM FUNCTIONS END ***************************/

int Shader::GetUniformLocation(const std::string &name)
{
  if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
  {
//    if cached return the cached value
    return m_UniformLocationCache[name];
  }
  
  GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
  
  if (location == -1)
  {
    std::cout << "Warning: uniform (" << name << ") doesnt exist" << std::endl;
  }

//   cache it
  m_UniformLocationCache[name] = location;
  
  
  return location;
}

/**
 * read in the files with c++
 * C file API is faster to read files though
 */
ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
  std::ifstream stream(filepath);
  
  enum class ShaderType
  {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
  };
  
  std::string line;
  std::stringstream ss[2];  // one for vertex shader and one for fragment shader
  auto type = ShaderType::NONE;
  while (getline(stream, line))
  {
    if (line.find("#shader") != std::string::npos)
    {
      if (line.find("vertex") != std::string::npos)
      {
        // set mode to vertex
        type = ShaderType::VERTEX;
      }
      else if (line.find("fragment") != std::string::npos)
      {
        // set mode to fragment
        type = ShaderType::FRAGMENT;
      }
    }
    else
    {
      ss[(int)type] << line << "\n";
    }
  }
  
  //  set the first one to VertexSource and second one to FragmentSource
  return {ss[0].str(), ss[1].str()};
}


/**
 * Abstract the compiling shader logic to avoid repetitios code
 * source string must not be nullptr
 */
unsigned int Shader::CompileShader(unsigned int type, const std::string &source)
{
  unsigned int id = glCreateShader(type);
  
  //  get a pointer to the source data
  const char* src = source.c_str();   // &source[0]
  
  glShaderSource(id, 1, &src, nullptr);
  
  glCompileShader(id);
  
  //  Error handling
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  
  if (result == GL_FALSE)
  {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char *message = new char[length];
    glGetShaderInfoLog(id, length, &length, message);
    
    std::cout << "Failed to compile the " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
    std::cout << message << std::endl;
    
    delete [] message;
    
    glDeleteShader(id); // because compilation did not work
    return 0;
  }
  
  return id;
}

/**
 * Shader function
 * to render both shaders
 */
unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
  //  create a shader program
  unsigned int program = glCreateProgram();
  
  //  create 2 shader obj
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
  
  //  link the shaders
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  
  //  link and validate the program
  glLinkProgram(program);
  glValidateProgram(program);
  
  //  delete your shaders since they are now linked to the program
  glDeleteShader(vs);
  glDeleteShader(fs);
  
  return program;
}
