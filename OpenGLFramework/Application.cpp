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
  
//  initialise glew
  if (glewInit() != GLEW_OK) std::cout << "Error" << std::endl;
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "Supported GLSL version is " << (char *)glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  

  
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
  
  
//  define the vertex buffer
  unsigned int buffer;
  glGenBuffers(1, &buffer); // gives us back an id
  
//  select that bufffer
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  
//  specify the data
//  put the data into the buffer
//  6 * sizeof(floa) == sizeof(positions)
  glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);
  
//  we need to enable the vertex attribute with the index you want to enable
  glEnableVertexAttribArray(0);
//  specify the attribute for hte first vertex whihc is at index 0, last 0 is 0 bytes
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
  
//  define the index buffer
  unsigned int indexBuffer;
  glGenBuffers(1, &indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
  
  
//  read in the shaders
  ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

  unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
  
//  bind the shader
  glUseProgram(shader);
  
  
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);
    
//    draw the triangle specified - draw call
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    
    glEnd();
    
    
    /* Swap front and back buffers */
    glfwSwapBuffers(window);
    
    /* Poll for and process events */
    glfwPollEvents();
  }
  
  glDeleteProgram(shader);
  
  glfwTerminate();
  
  return 0;
}
