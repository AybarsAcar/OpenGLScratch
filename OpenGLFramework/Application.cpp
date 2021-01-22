//
//  Application.cpp
//  OpenGLFramework
//
//  Created by Aybars Acar on 21/1/21.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

/**
 * return this struct as we parse the shader file into 2 different shader programs
 */
struct ShaderProgramSource
{
  std::string VertexSource;
  std::string FragmentSource;
};


/**
 * read in the files with c++
 * C file API is faster to read files though
 */
static ShaderProgramSource ParseShader(const std::string &filepath)
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
static unsigned int CompileShader(unsigned int type, const std::string &source)
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
static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
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

int main(void)
{
  GLFWwindow* window;
  
  /* Initialize the library */
  if (!glfwInit())
    return -1;
  
  //  tell glfw we want to create this context with the call profile
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  
  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }
  
  //  Create the openGl context
  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  
  glfwSwapInterval(1);  // 1 is the default frame rate 60hz etc
  
  //  initialise glew
  if (glewInit() != GLEW_OK) std::cout << "Error" << std::endl;
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "Supported GLSL version is " << (char *)glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  
  
  {
    //  draw a square with 2 trieangles
    float positions[] =
    {
      -0.5f,  -0.5f,    // 0
      0.5f,  -0.5f,    // 1
      0.5f,   0.5f,    // 2
      -0.5f,   0.5f,    // 3
    };
    
    //  indices buffer
    unsigned int indices[] =
    {
      0, 1, 2,      // triangle 1
      2, 3, 0,      // triangle 2
    };
    
    
    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));
    
    
    //  create the vertex buffer - our constructor automatically binds it as well
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    
    //  we need to enable the vertex attribute with the index you want to enable
    GLCall(glEnableVertexAttribArray(0));
    //  specify the attribute for hte first vertex whihc is at index 0, last 0 is 0 bytes
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
    
    //  define the index buffer
    IndexBuffer ib(indices, 6);
    
    //  read in the shaders
    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
    
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    
    //  bind the shader
    GLCall(glUseProgram(shader));
    
    //  Pass down the colour from the CPU side to our shader
    int location = glGetUniformLocation(shader, "u_Color");
    ASSERT(location != -1);
    
    //  unbind everything - we have to do this each time we are changing the draw geometry
    GLCall(glBindVertexArray(0));
    GLCall(glUseProgram(0));
    ib.Unbind();
    vb.Unbind();
    
    float redChannel = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
      /* Render here */
      GLCall(glClear(GL_COLOR_BUFFER_BIT));
      
      //    bind them back
      GLCall(glUseProgram(shader));
      
      //    pass down the colour dynamically
      glUniform4f(location, redChannel, 0.3f, 0.8f, 1.0f);
      
      GLCall(glBindVertexArray(vao));
      
      //    bind it back using our class
      ib.Bind();
      
      //    draw the triangle specified - draw call
      GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
      
      //    Animate the colour
      if (redChannel > 1.0f)
      {
        increment = -0.05f;
      }
      else if (redChannel < 0.0f)
      {
        increment = 0.05f;
      }
      redChannel += increment;
      
      glEnd();
      
      
      /* Swap front and back buffers */
      glfwSwapBuffers(window);
      
      /* Poll for and process events */
      glfwPollEvents();
    }
    
    glDeleteProgram(shader);
  }
  
  glfwTerminate();
  
  return 0;
}
